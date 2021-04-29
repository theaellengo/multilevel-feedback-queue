#include "display.h"
#include "io.h"
#include "process.h"
#include "queue.h"
#include "rr.h"
#include <stdio.h>

void mlfq(Queue queue[], int x, Process process[], int y, int pboost)
{
  int clock = 0, pb = pboost, rpro = y, run = 1, pdone = 0;
  float awt = 0;

  // inititalize queues
  Queue ready = initqueue(-1);
  Queue io = initqueue(-1);
  Queue gnatt[x + 1];
  for (int i = 0; i < x; i++) {
    gnatt[i] = initqueue(queue[i].qid);
  }

  sortbyarrival(process, y);

  // start clock
  while (run) {
    int flag = 0, sum = 0, i = 0;
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

        // execute round robin
        rr(queue[i], &gnatt[i], &clock, &sum, &pb, 0);

        if (ioburst(queue, gnatt, &io, i, x, &clock, &sum, &pb, &pdone)) {
          break;
        }

        // process burst
        if (pb <= 0) {
          // move the last executing process to tail of queue
          if (queue[i].head != NULL)
            enqueue(&queue[i], dequeue(&queue[i]));

          // move everything to the ready queue
          for (int j = i; j < x; j++) {
            while (queue[j].head != queue[j].tail)
              enqueue(&ready, dequeue(&queue[j]));
            if (queue[j].head != NULL)
              enqueue(&ready, dequeue(&queue[j]));
          }

          // update value of time left before next boost
          pb += pboost;

          // there are new processes in the ready queue
          newpro = 1;
        }

        else {
          // move executing process to the next quque
          int qidx = (i == x - 1) ? i : i + 1;
          if (queue[i].head->exectime > 0)
            enqueue(&queue[qidx], dequeue(&queue[i]));
          else
            dequeue(&queue[i]);
        }

        break;
      } else {
        // increment index of queue if no procces in current queue
        i++;
      }

      // if a new process arrived
      if (newpro == 1) {
        break;
      }

      // if there are no processes left, stop clock
      if (isempty(queue[i]) && rpro == 0 && isempty(ready)) {
        run = 0;
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

  for (int i = 0; i < x; i++)
    printgnatt(gnatt[i]);
  printgnatt(io);
}
