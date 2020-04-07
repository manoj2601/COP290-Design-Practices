#include <stdio.h>
#include <stdlib.h>

#include "tellerline.h"
#define MAX_SIZE atoi("100")
// void vector_init(vector *v)
void tellerline_init (tellerline *t)
{
    // v.capacity = VECTOR_INIT_CAPACITY;
    t->total = 0;
    t->front = -1;
    t->rear = -1;
    t->items = malloc(MAX_SIZE);
}

void enQueue(tellerline *t, struct costumer c)
{
    t->total = t->total + 1;
    if(t->rear == MAX_SIZE-1)
        {
            printf("\nQueue is full\n");
            queue_resize(t, MAX_SIZE*2);
            // vector_resize(v, v->capacity * 2);
        }
    if((t->front) == -1)
        (t->front) = 0;
    (t->rear)++;
    (t->items)[(t->rear)] = c;
    printf("\ninserted;\n");
}

int queue_total(tellerline *v)
{
    return v->total;
}

struct costumer deQueue(tellerline *t)
{
    if((t->front) == -1)
    {
        printf("\nQueue is empty\n");
        struct costumer ret = {0};
        return ret;
    }
    else
    {
        t->total--;
        // printf("\nDeleted : %d", (v->items)[front]);
        (t->front)++;
        struct costumer ret = (t->items)[(t->front)-1];
        if((t->front) > (t->rear))
            (t->front) = (t->rear) = -1;
        return ret;
    }
}

static void queue_resize(tellerline *v, int capacity)
{
    #ifdef DEBUG_ON
    printf("queue_resize: %d to %d\n", v->capacity, capacity);
    #endif

    struct costumer *items = realloc(v->items, capacity);
    // if (items) {
        v->items = items;
        // v->capacity = capacity;
    // }
}

void queue_free(tellerline *v)
{
    free(v->items);
}