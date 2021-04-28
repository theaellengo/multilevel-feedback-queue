#include "display.h"
#include "limits.h"
#include "process.h"
#include <math.h>
#include <stdio.h>

void rr(Queue q, Queue* gnatt, Queue* io, int* clock, int* sum, int* pb, int execfor)
{
  printf("\nQ[%d]\n", q.priority);
  int exec;
  if (execfor == 0) {
    exec = (q.timequant < q.head->exectime) ? q.timequant : q.head->exectime;
    if (exec > q.head->iofreq && q.head->iofreq != 0) exec = q.head->iofreq;
  } else {
    exec = execfor;
  }

  //if (*clock + exec > interruptat) exec = interruptat - *clock;

  Process* temp = pcopy(q.head);
  setprocess(temp, clock, exec);
  if (exec > 0) enqueue(gnatt, temp);
  //printf("P%d:%d %d %d %d %d\n", temp->pid, temp->exectime, temp->start, temp->completion, temp->waiting, temp->turnaround);

  q.head->exectime -= exec;
  *sum += exec;
  *clock += exec;
  *pb -= exec;

  /*
  temp = pcopy(q.head);
  setprocess(temp, clock, q.head->iobt);
  if (q.head->iobt > 0 && q.head->exectime > 0) {
    *interruptat = temp->completion;
    enqueue(io, temp);
    enqueue(&q, dequeue(&q));
    clock += q.head->iobt;

    exec = q.head->iobt;
    temp = pcopy(q.head);
    setprocess(temp, clock, exec);
    if (exec > 0) enqueue(gnatt, temp);
  } else {
    *interruptat = INT_MAX;
  }
  */
}

void iob(Queue q, Queue* io, int* clock)
{
  Process* temp = pcopy(q.head);
  setprocess(temp, clock, q.head->iobt);
  if (q.head->iobt > 0 && q.head->exectime > 0) {
    enqueue(io, temp);
    enqueue(&q, dequeue(&q));
  }
}