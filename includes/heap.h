
#include "codexion.h"

typedef struct s_heap_node
{
    t_coder     *coder;
    long long   arrival_time;
    long long   deadline;
}               t_heap_node;

typedef struct s_heap
{
    t_heap_node *nodes;
    int         capacity;
    int         size;
}               t_heap;
