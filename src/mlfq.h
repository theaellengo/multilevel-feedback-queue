#include "display.h"
#include "process.h"
#include "queue.h"
#include "rr.h"
#include <math.h>
#include <stdio.h>

void mlfq(Queue queue[], int x, Process process[], int y, int pboost)
{
  int clock = 0, pb = pboost, rpro = y, executing = 1;
  float awt = 0;

  // inititalize queues
  Queue ready = initqueue(-1);
  Queue io = initqueue(-1);
  Queue gnatt[x];
  for (int i = 0; i < x; i++) {
    gnatt[i] = initqueue(queue[i].qid);
  }

  sortbyarrival(process, y);

  // start clock
  while (executing) {
    int flag = 0, sum = 0;

    int i = 0;
    while (i != x) {

      // check for newly arrived processes
      int newpro = 0;
      for (int i = 0; i < y; i++) {
        if (process[i].arrival <= clock && process[i].ready == 0) {
          rpro = rpro - 1, newpro = 1, process[i].ready = 1;
          enqueue(&ready, &process[i]);
        }
      }

      // add newly arrived processes to highest priority queue
      while (ready.head != NULL) {
        enqueue(&queue[0], dequeue(&ready));
      }

      if (queue[i].head != NULL) {
        flag = 1;

        rr(queue[i], &gnatt[i], &clock, &sum, &pb);

        if (pb <= 0) {
          for (int j = i; j < x; j++) {
            while (queue[j].head != queue[j].tail)
              enqueue(&ready, dequeue(&queue[j]));
            enqueue(&ready, dequeue(&queue[j]));
          }
          pb += pboost;
          newpro = 1;
        }

        else {
          int qidx = (i == x - 1) ? i : i + 1;
          if (queue[i].head->exectime > 0)
            enqueue(&queue[qidx], dequeue(&queue[i]));
          else
            dequeue(&queue[i]);
        }

        break;
      } else {
        i++;
      }

      // if there are no processes in current queues
      if (isempty(queue[i]) && rpro == 0 && isempty(ready)) {
        executing = 0;
      }

      // if a new process arrives
      if (newpro == 1) {
        break;
      }
    }

    // if no process executed at current cpu time
    if (flag == 0) {
      pb--;
      sum++;
      clock++;
    }

    // subtract exec time of current cycle from arrival
    for (int i = 0; i < y; i++) {
      process[i].arrtime -= sum;
    }
  }

  for (int i = 0; i < x; i++) {
    printgnatt(gnatt[i]);
  }
}