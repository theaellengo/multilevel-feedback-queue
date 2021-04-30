#include "display.h"
#include "process.h"
#include "queue.h"
#include <stdio.h>

#ifndef RR
#define RR

void rr(Queue q, Queue* gnatt, int* clock, int* sum, int* pb, int execfor)
{
  int exec = q.head->exectime;
  if (q.timequant < exec) exec = q.timequant;

  // get the minimum execution time
  if (execfor == 0) {
    if (q.head->exectq <= 0) q.head->exectq = exec;
  } else {
    if (execfor < exec) exec = execfor;
  }
  if (exec > q.head->iofreq && q.head->iofreq > 0) exec = q.head->iofreq;

  q.head->exectime -= exec;
  q.head->exectq -= exec;
  q.head->qid = q.qid;

  // add copy of process to current queue
  Process* temp = pcopy(q.head);
  setprocess(temp, clock, exec);
  if (exec > 0) enqueue(gnatt, temp);

  // update values
  *sum += exec;
  *clock += exec;
  *pb -= exec;
}

#endif