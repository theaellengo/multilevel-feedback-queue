#include "display.h"
#include "process.h"
#include <math.h>
#include <stdio.h>

void rr(Process process[], int n, int timeslice)
{
  // calculate size of queue (amount of context switches)
  int qsize = 0;
  for (int i = 0; i < n; i++) {
    qsize += floor(process[i].burst / timeslice);
    qsize += (process[i].burst % timeslice != 0 ? 1 : 0);
  }

  // init values
  Process queue[qsize];
  int clock = 0, idx = 0, last = -1;
  float awt = 0;

  // while all slices not in queue
  while (idx < qsize) {
    int sum = 0; // total execution time
    int flag = 0;

    sortbyarrival(process, n);
    printf("\n");
    for (int i = 0; i < n; i++) {
      // if process has arrived and has not finsihed execution
      if (process[i].arrtime <= 0 && process[i].exectime > 0) {
        flag = 1;
        queue[idx] = process[i];

        // if remaining burst time > timeslice, execute process for timeslice units
        int exectime = (queue[idx].exectime > timeslice) ? timeslice : queue[idx].exectime;
        setprocess(&queue[idx], &clock, exectime);

        // if process is done executing, add process waiting time to awt
        if (queue[idx].exectime <= 0) awt += queue[idx].waiting;

        sum += exectime;
        process[i].exectime -= exectime;
        process[i].arrtime = exectime;
        last = process[i].pid;

        // add to end of queue
        Process temp = process[i];
        for (int j = i; j < n - 1; j++)
          process[j] = process[j + 1];
        process[n - 1] = temp;

        idx++;
        break;
      }
    }
    if (flag == 0) {
      clock++;
      sum++;
    }
    // subtract execution time from all processes
    for (int i = 0; i < n; i++) {
      process[i].arrtime -= sum;
    }
  }

  awt /= n;
  printgnatt(queue, idx);
  printprocess(process, n, queue, idx, awt);
}
