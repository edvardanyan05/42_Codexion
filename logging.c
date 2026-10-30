#include "codexion.h"

void print_status(t_sim *sim, int id, const char *status)
{
    long long timestamp;

    pthread_mutex_lock(&sim->print_lock);
    pthread_mutex_lock(&sim->sim_lock);
    if (sim->stop_simulation && strcmp(status, "burned out") != 0)
    {
        pthread_mutex_unlock(&sim->sim_lock);
        pthread_mutex_unlock(&sim->print_lock);
        return;
    }
    pthread_mutex_unlock(&sim->sim_lock);
    timestamp = get_time_in_ms() - sim->start_time;
    printf("%lld %d %s\n", timestamp, id, status);
    pthread_mutex_unlock(&sim->print_lock);
}
