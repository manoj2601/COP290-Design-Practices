#ifndef EVENTQUEUE_H
#define EVENTQUEUE_H



typedef struct EventQueue {
    Node *head;
    int size;
} EventQueue;


void EventQueue_init (EventQueue *);
bool is_empty(EventQueue *);
void enQueueEvent(EventQueue *, Event);
Event deQueueEvent(EventQueue *);
int EventQueue_total(EventQueue *);
void printEventQueue(EventQueue *);



// void enQueue(tellerline *, struct costumer);
// struct costumer deQueue(tellerline *);

// static void queue_resize(tellerline *, int);
// void queue_free(tellerline *);

#endif