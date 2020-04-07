#ifndef TELLERLINE_H
#define TELLERLINE_H

struct costumer
{
	float arrTime; //random
};


typedef struct tellerline {
    struct costumer *items;
    // int capacity;
    int front;
    int rear;
    int total;
} tellerline;

void tellerline_init (tellerline *);
void enQueue(tellerline *, struct costumer);
struct costumer deQueue(tellerline *);
int queue_total(tellerline *);
static void queue_resize(tellerline *, int);
void queue_free(tellerline *);

#endif