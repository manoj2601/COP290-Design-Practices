#ifndef TELLERLINE_H
#define TELLERLINE_H
#include "allstructs.h"





void tellerline_init (tellerline *);
void enQueue(tellerline *, struct costumer);
struct costumer deQueue(tellerline *);
int queue_total(tellerline *);
static void queue_resize(tellerline *, int);
void queue_free(tellerline *);

#endif