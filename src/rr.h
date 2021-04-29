#include "display.h"
#include "limits.h"
#include "process.h"
#include <math.h>
#include <stdio.h>

void rr(Queue q, Queue* gnatt, int* clock, int* sum, int* pb, int execfor)
{
  int exec = q.head->exectime;
  if (q.timequant < exec) exec = q.timequant;

  // get the minimum execution time
  if (execfor == 0) {
    if (q.head->exectq <= 0) q.head->exectq = exec;
    if (exec > q.head->iofreq && q.head->iofreq > 0) exec = q.head->iofreq;
  } else {
    if (execfor < exec) exec = execfor;
  }

  // add copy of process to current queue
  Process* temp = pcopy(q.head);
  setprocess(temp, clock, exec);
  if (exec > 0) enqueue(gnatt, temp);

  // update values
  q.head->exectime -= exec;
  q.head->exectq -= exec;
  *sum += exec;
  *clock += exec;
  *pb -= exec;
}

int execio(Queue q[], Queue gnatt[], Queue* io, int i, int x, int* clock, int* sum, int* pb, int* pdone)
{
  // if process not done executig and has io burst
  if (q[i].head->exectime > 0
      && q[i].head->iobt > 0
      && q[i].head->iofreq <= q[i].timequant) {

    // add process to io queue if execution time is more than 0
    Process* temp = pcopy(q[i].head);
    setprocess(temp, clock, q[i].head->iobt);
    if (temp->completion - temp->start > 0) enqueue(io, temp);

    // if pdone is 0, process just started executing in current queue
    if (*pdone == 0) {

      // process in io queue
      Process* ioexec = q[i].head;

      // index of queue that will be executing during ioexec's time in io queue
      int qidx = (i == x - 1 || q[i].head->next != NULL) ? i : i + 1;
      if (q[qidx].head == ioexec) { enqueue(&q[qidx], dequeue(&q[qidx])); }
      if (q[qidx].head != NULL) {
        // continue executing processes while io is still happening
        while (*clock < temp->completion && ioexec->exectq > 0) {
          rr(q[qidx], &gnatt[qidx], clock, sum, pb, temp->completion - *clock);
          execio(q, gnatt, io, qidx, x, clock, sum, pb, pdone);
          int nqidx = (i == x - 1) ? i : i + 1;
          enqueue(&q[nqidx], dequeue(&q[qidx]));
        }
      }

      // process time in current queue finished
      *pdone = 1;
      return 1;
    }
  }

  // process time in current queue not yet done
  *pdone = 0;
  return 0;
}