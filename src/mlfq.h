#include "display.h"
#include "process.h"
#include "queue.h"
#include "rr.h"
#include <math.h>
#include <stdio.h>

/*
  Rule 1: If Priority(A) > Priority(B), A runs (B doesn’t). 
  Rule 2: If Priority(A) = Priority(B), A & B run in RR
  Rule 3: When a job enters the system, it is placed at the highest priority (the topmost 
  queue). 
  Rule 4: Once a job uses up its time quantum at a given level (regardless of how many 
  times it has given up the CPU e.g. during an IO burst), its priority is reduced (i.e., it 
  moves down one queue). 
  Rule 5: After some time period S, move all the jobs in the system to the topmost queue. 
  This is what we refer to as a priority boost.
*/

void mlfq(Queue queue[], int x, Process process[], int y, int pboost)
{
  int clock = 0, rpro = y, sum = 0;
  float awt = 0;
  Queue temp = { .qid = -1, .priority = 0, .timequant = 0 };

  sortbyarrival(process, y);

  // add ready processes to highest priority queue
  for (int j = 0; j < y; j++)
    if (process[j].arrtime <= 0 && process[j].exectime > 0)
      enqueue(&temp, &process[j]);

  while (rpro > 0 && clock < 100) {
    int flag = 0;

    // iterate through all the queues
    for (int i = 0; i < x; i++) {

      // if arrived, add proccess from temp to queue[0]

      // if queue is not empty
      if (queue[i].head != NULL) {
        flag = 1, sum = 0;

        printf("\nCLOCK: %d\t", clock);
        rr(queue[i], queue[i].timequant, &clock, &sum);

        // move to next queue
        while (i != x - 1 && queue[i].head != NULL) {
          if (queue[i].head->exectime > 0) {
            enqueue(&queue[i + 1], queue[i].head);
          }
          *dequeue(&queue[i]);
        }

        break;
      } else {
        sum = 1;
      }
    }
    for (int j = 0; j < y; j++) {
      process[j].arrtime -= sum;
    }
    if (flag == 0) {
      clock++;
    }
  }

  printf("Q[%d]: %d\n", temp.priority, getqsize(temp));
}

/*
int qsize = getqsize(queue[0]);
  Process p[qsize];
  for (int i = 0; i < qsize; i++)
    p[i] = *dequeue(&queue[0]);

  printf("\nQ:%d\t", getqsize(queue[0]));
  for (int i = 0; i < qsize; i++) {
    printf("P%d\t", p[i].pid);
  }

  // Sample Code
  Process curr = *queue[0].head;
  while ((curr.next) != NULL) {
    printf("%d\t", curr.pid);
    curr = *curr.next;
  }
  enqueue(&queue[1], dequeue(&queue[0]));
  enqueue(&queue[1], dequeue(&queue[0]));
  enqueue(&queue[1], dequeue(&queue[0]));
  enqueue(&queue[1], dequeue(&queue[0]));
  printf("\n");
  curr = *queue[1].head;
  while ((curr.next) != NULL) {
    printf("%d\t", curr.pid);
    curr = *curr.next;
  }
  printf("\n");
  if (queue[2].head != NULL) {
    curr = *queue[2].head;
    printf("\nEMPTY QUEUE\n");
    if (curr.next != NULL) {
      printf("%d\t", curr);
      curr = *curr.next;
    }
  }
*/