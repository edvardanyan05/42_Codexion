*This project has been created as part of the 42 curriculum by edmvarda.*

# Codexion

## Description

Codexion is a concurrency simulation written in C with POSIX threads.
It is a variation of the classic *dining philosophers* problem:

- `N` coders sit around a circular table, with one USB dongle between each pair
  of neighbours (so there are `N` dongles).
- To **compile**, a coder needs both dongles: the one on the left and the one on the right.
- After compiling, the coder releases the dongles, then **debugs**, then **refactors**,
  and then tries to compile again.
- A released dongle cannot be taken again until its **cooldown** has elapsed.
- When several coders want the same dongle, the dongle decides who gets it using
  a **scheduler**: `fifo` (first request first) or `edf` (earliest burnout deadline first).
- If a coder does not *start* compiling within `time_to_burnout` ms of the start of
  their last compile (or of the simulation), they **burn out** and the simulation stops.
- The simulation also stops when every coder has compiled at least
  `number_of_compiles_required` times.

The goal is to synchronize every coder (one thread each) and a monitor thread
safely. There must be no data races, no deadlocks and no starvation, burnout must
be detected precisely, and the logs must never be mixed up.

## Instructions

### Compilation

```sh
make        # builds ./codexion
make clean  # removes object files
make fclean # removes object files and the binary
make re     # full rebuild
```

The project compiles with `cc -Wall -Wextra -Werror -pthread`.

### Execution

```sh
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug \
           time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

All 8 arguments are mandatory. The first 7 must be non-negative integers
(at most `2147483647`), `number_of_coders` must be at least `1`, and `scheduler`
must be exactly `fifo` or `edf`. Any invalid input prints an error on `stderr`
and the program exits with status `1`.

Examples:

```sh
./codexion 4 410 200 100 100 5 0 edf    # nobody should burn out
./codexion 4 310 200 100 100 5 0 fifo   # a coder burns out
./codexion 1 800 200 200 200 5 0 fifo   # single coder: takes one dongle, burns out at 800
./codexion 5 800 200 200 200 7 50 edf   # with a 50 ms dongle cooldown
```

Each state change is logged as `timestamp_in_ms coder_id message`, where
`message` is one of `has taken a dongle`, `is compiling`, `is debugging`,
`is refactoring` or `burned out`.

### Project structure

| File | Role |
|---|---|
| `main.c` | Entry point: parse, init, start threads, join, cleanup |
| `parsing.c` | Argument validation |
| `init.c`, `init_helpers.c` | Allocation and mutex/cond initialization |
| `simulation.c` | Coder thread routine (compile → debug → refactor) |
| `monitor.c` | Monitor thread: burnout and completion detection |
| `dongle.c` | Taking and releasing dongles (queue, cooldown, arbitration) |
| `heap.c`, `heap_helpers.c` | Custom binary min-heap (priority queue) |
| `logging.c` | Serialized output |
| `time.c`, `utils.c` | Time helpers and small utilities |
| `cleanup.c` | Destroys every mutex/cond and frees all memory |

## Blocking cases handled

### Deadlock prevention (Coffman's conditions)

A deadlock requires all four Coffman conditions at once: mutual exclusion,
hold and wait, no preemption, and **circular wait**. The first three are part
of the problem itself: a dongle is exclusive, a coder holds one while waiting
for the other, and dongles cannot be taken away. So we break **circular wait**.

Every dongle has an id (`1..N`) and each coder always takes the **lower id first**
(resource hierarchy, `take_both_dongles` in `dongle.c`). Coder `N` sits between
dongle `N` and dongle `1`, so they request dongle `1` first, just like coder `1`.
A cycle of coders each waiting on their neighbour therefore cannot form.

### Starvation prevention and fair arbitration

Each dongle owns a **waiting queue implemented as a binary min-heap**
(no standard library priority queue). A coder requesting a dongle is pushed
into its heap, and may only take the dongle when it is the **head of the heap**.
A late arrival can never "steal" the dongle from someone who should be served first.

- `fifo`: nodes are ordered by a **ticket**, a per-dongle counter incremented
  on every request. This is the exact arrival order, so there are never ties
  (millisecond timestamps would often be equal).
- `edf`: nodes are ordered by `deadline = last_compile_start + time_to_burnout`.
  **Tie-breaker:** equal deadlines are ordered by ticket, so the order is
  always fully deterministic.

With `edf`, the coder closest to burning out is always served first. This
guarantees liveness whenever the parameters are feasible.

### Dongle cooldown

When a dongle is released, `cooldown_until = now + dongle_cooldown` is stored.
A dongle is available only if it has no owner, the requester is at the head of
its queue, **and** `now >= cooldown_until`. If only the cooldown is missing,
the head coder sleeps with `pthread_cond_timedwait` until exactly `cooldown_until`.
It does not miss the end of the cooldown, and it does not busy-wait.

### Single coder

With one coder there is a single dongle (left == right). The coder takes it,
can never get a second one, and waits until the monitor reports the burnout.

### Precise burnout detection

A dedicated **monitor thread** checks every coder about every 0.5 ms.
`last_compile_time` is read under the coder's mutex. When
`now - last_compile_time >= time_to_burnout`, it sets the end flag and prints
`burned out`. This happens well within the 10 ms limit. All sleeps use small
`usleep(500)` steps based on `gettimeofday`, so they are accurate.

### Clean termination

When the simulation ends, the monitor **broadcasts on every dongle's condition
variable**, so coders blocked in `pthread_cond_wait` wake up, see the end flag
and exit. Long sleeps (`ft_usleep`) also check the end flag and return early.
All threads are joined, then every mutex and condition variable is destroyed
and all memory is freed. Cleanup also handles a partial initialization.

### Log serialization

Every message goes through `print_status`, which holds `print_lock` for the
whole `printf`, so two lines can never be interleaved. Once the simulation
has ended, every message except the final `burned out` is suppressed, so
nothing is printed after a burnout.

## Thread synchronization mechanisms

| Primitive | Protects / role |
|---|---|
| `dongle.lock` (`pthread_mutex_t`) | Dongle state: `owner_id`, `cooldown_until`, `next_ticket`, its heap |
| `dongle.cond` (`pthread_cond_t`) | Waiting queue of the dongle: coders sleep on it until they may take it |
| `coder.coder_lock` (`pthread_mutex_t`) | `last_compile_time` and `compile_count`, shared between a coder and the monitor |
| `sim.sim_lock` (`pthread_mutex_t`) | The `simulation_ended` flag, read by every thread |
| `sim.print_lock` (`pthread_mutex_t`) | Standard output, so log lines never interleave |

### How a coder takes a dongle

1. Lock `dongle.lock`, get a ticket, push its node into the dongle's heap.
2. While (the dongle has an owner **or** the coder is not the heap head **or** the
   cooldown is running) and the simulation is not over, wait on `dongle.cond`.
   `pthread_cond_wait` atomically releases the mutex while sleeping.
3. Pop itself from the heap, set `owner_id`, log `has taken a dongle`, unlock.

Releasing a dongle locks `dongle.lock`, clears the owner, sets the cooldown and
calls `pthread_cond_broadcast` so the waiters re-check their condition.

### Coder ↔ monitor communication

- **Coder → monitor:** when a coder starts compiling, it updates `last_compile_time`
  and `compile_count` while holding `coder_lock`. The monitor reads them under the
  same mutex, so it never sees a half-written value.
- **Monitor → coders:** the monitor sets `simulation_ended` under `sim_lock`.
  Every coder loop, every sleep and every dongle wait checks it through
  `is_simulation_ended()`, which takes the same lock. The monitor then
  broadcasts every `dongle.cond` while holding each dongle mutex. A coder
  checks the flag and starts waiting in one step under the dongle mutex,
  so the wake-up cannot be lost.

### Race condition examples prevented

- Two neighbours taking the same dongle at once is impossible: `owner_id` is
  only checked and set while `dongle.lock` is held.
- The monitor reading `last_compile_time` while the coder writes it is
  prevented by `coder_lock`.
- Printing after the simulation ended is prevented: `print_status` checks
  `simulation_ended` while holding `print_lock`, and the monitor sets the flag
  before printing `burned out`.

### Lock ordering

Locks are always taken in a consistent order, so the locks themselves cannot deadlock:
`coder_lock → sim_lock`, `coder_lock → print_lock → sim_lock`,
`dongle.lock → print_lock → sim_lock`, `dongle.lock → sim_lock`.
No thread takes them in the opposite order.

## Resources

- [POSIX Threads Programming (LLNL)](https://hpc-tutorials.llnl.gov/posix/)
- `man pthread_create`, `man pthread_mutex_lock`, `man pthread_cond_wait`,
  `man pthread_cond_timedwait`, `man gettimeofday`
- [Dining philosophers problem (Wikipedia)](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
- [Coffman conditions / Deadlock (Wikipedia)](https://en.wikipedia.org/wiki/Deadlock_(computer_science))
- [Earliest Deadline First scheduling (Wikipedia)](https://en.wikipedia.org/wiki/Earliest_deadline_first_scheduling)
- [Binary heap (Wikipedia)](https://en.wikipedia.org/wiki/Binary_heap)

### AI usage

An AI assistant (Claude) was used for the following:

- **Code review** of the existing implementation against the subject. It found
  compilation errors, the swapped argument order, the monitor sleeping too long,
  the lost cooldown wake-up and the missing wake-up at the end of the simulation.
- **Fixing those issues:** the ticket-based tie-breaker in the heap, the timed
  wait for the cooldown, waking all dongles at the end, and making init/cleanup
  safe after a partial failure.
- **Writing** `main.c`, the `Makefile` and a first draft of this README.

Every change was reviewed, tested and understood before being kept.
