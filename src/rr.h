#include "display.h"
#include "limits.h"
#include "process.h"
#include <math.h>
#include <stdio.h>

void rr(Queue q, Queue* gnatt, Queue* io, int* clock, int* sum, int* pb, int execfor)
{
  int exec;
  if (execfor == 0) {
    exec = (q.timequant < q.head->exectime) ? q.timequant : q.head->exectime;
    if (exec > q.head->iofreq && q.head->iofreq != 0) exec = q.head->iofreq;
  } else {
    exec = (q.timequant < q.head->exectime) ? q.timequant : q.head->exectime;
    exec = (execfor < exec) ? execfor : exec;
  }

  Process* temp = pcopy(q.head);
  setprocess(temp, clock, exec);
  if (exec > 0) enqueue(gnatt, temp);

  q.head->exectime -= exec;
  *sum += exec;
  *clock += exec;
  *pb -= exec;
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