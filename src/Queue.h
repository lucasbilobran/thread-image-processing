#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>

typedef struct Node
{
    void *data;
    struct Node *next;
}node;

typedef struct QueueList
{
    int sizeOfQueue;
    size_t memSize;
    node *head;
    node *tail;
}Queue;

void queueInitialize(Queue *q, size_t memSize);
int enqueue(Queue *, const void *);
void dequeue(Queue *, void *);
void clearQueue(Queue *);
int isQueueEmpty(Queue *);

#endif 