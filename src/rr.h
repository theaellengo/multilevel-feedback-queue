#include "display.h"
#include "process.h"
#include <math.h>
#include <stdio.h>

void rr(Queue q, Queue* gnatt, int* clock, int* sum, int* pb)
{
  int exec = (q.timequant < q.head->exectime) ? q.timequant : q.head->exectime;

  Process* temp = pcopy(q.head);
  setprocess(temp, clock, exec);
  if (exec > 0) enqueue(gnatt, temp);

  q.head->exectime -= exec;
  *sum += exec;
  *clock += exec;
  *pb -= exec;
}