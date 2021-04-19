#include "process.h"
#include <stdio.h>
#include <stdlib.h>

#ifndef QUEUE
#define QUEUE

/** Struct of Queue**/
typedef struct queue {
  int qid;
  int priority;
  int timequant;
  Process* head;
  Process* tail;
} Queue;

int enqueue(Queue* q, Process* p);
Process* dequeue(Queue* q);
int getqsize(Queue q);
void sortqbypriority(Queue q[], int x);

int enqueue(Queue* q, Process* p)
{
  if (q->head == NULL)
    q->head = p;
  if (q->tail != NULL) {
    q->tail->next = p;
  }
  q->tail = p;
  q->tail->next = NULL;
  return 1;
}

Process* dequeue(Queue* q)
{
  Process* temp = q->head;
  q->head = q->head->next;
  if (q->head == NULL) {
    q->tail = NULL;
    return NULL;
  }
  return temp;
}

// returns size of queues
int getqsize(Queue q)
{
  int count = 0;
  if (q.head != NULL) {
    Process* curr = q.head;
    while (curr != NULL) {
      count++;
      curr = curr->next;
    }
  }
  return count;
}

// sort queues by priority (descending)
void sortqbypriority(Queue q[], int x)
{
  Queue temp;
  for (int i = 0; i < x; i++)
    for (int j = i + 1; j < x; j++)
      if (q[i].priority < q[j].priority) {
        temp = q[i];
        q[i] = q[j];
        q[j] = temp;
      }
}

#endif