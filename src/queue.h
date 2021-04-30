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

Queue initqueue(int qid);
void enqueue(Queue* q, Process* p);
Process* dequeue(Queue* q);
void sortqbypriority(Queue q[], int x);
void printqueue(Queue q);

Queue initqueue(int qid)
{
  Queue q;
  q.qid = qid;
  q.priority = 0;
  q.timequant = 0;
  q.head = NULL;
  q.tail = NULL;
  return q;
}

int isempty(Queue q)
{
  if (q.head == NULL) return 1;
  return 0;
}

void enqueue(Queue* q, Process* p)
{
  p->next = NULL;
  if (q->tail != NULL) q->tail->next = p;
  if (q->head == NULL) q->head = p;
  q->tail = p;
}

Process* dequeue(Queue* q)
{
  Process* temp = q->head;
  q->head = q->head->next;
  if (q->head == NULL) q->tail = NULL;
  temp->next = NULL;
  return temp;
}

void sortqueuebypriority(Queue q[], int x)
{
  Queue temp;
  for (int i = 0; i < x; i++)
    for (int j = i + 1; j < x; j++)
      if (q[i].priority > q[j].priority) {
        temp = q[i];
        q[i] = q[j];
        q[j] = temp;
      }
}

void printqueue(Queue q)
{
  Process* curr = q.head;
  printf("Q[%d]\t", q.qid);
  while (curr != NULL) {
    printf("P%d: %d -> %d\n", curr->pid, curr->start, curr->completion);
    curr = curr->next;
  }
}

#endif