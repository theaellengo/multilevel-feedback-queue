// include libraries
#include "mlfq.h"
#include "process.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

/** Function Declarations **/
void getprocess(FILE* fp, Process* process);
void getqueues(FILE* fp, Queue* queue);
int checkprocesses(Process process[], int n);
int checkqueues(Queue queue[], int n);

int main()
{
  char filename[FILENAME_MAX];
  FILE* fp;

  printf("Enter Filename: ");
  gets(filename);

  // open and read file
  fp = fopen(filename, "r");
  if (fp == NULL) {
    printf("%s not found.\n", filename);
    exit(EXIT_FAILURE);
  }

  // get xys (num queues, num processes, priority boost)
  int xys[3];
  fscanf(fp, "%d %d %d", &xys[0], &xys[1], &xys[2]);
  if (xys[2] == 0)
    xys[2] = 1;

  // get x queues
  Queue queue[xys[0]];
  for (int i = 0; i < xys[0]; i++) {
    getqueues(fp, &queue[i]);
  }

  // get y processes
  Process process[xys[1]];
  for (int i = 0; i < xys[1]; i++) {
    getprocess(fp, &process[i]);
  }

  // close file
  fclose(fp);

  if (xys[0] < 2 || xys[0] > 5)
    printf("There are too many/few number of queues (2, 5).\n");
  else if (xys[1] < 3 || xys[1] > 100)
    printf("Number of processes should be in range (3, 100).\n");
  else if (xys[2] < 0)
    printf("Priority boost period should not be negative\n");
  else if (checkprocesses(process, xys[1])) {
  } else if (checkqueues(queue, xys[0])) {
  } else {
    sortqueuebypriority(queue, xys[0]);
    mlfq(queue, xys[0], process, xys[1], xys[2]);
  }

  return 0;
}

// scans processes from file & initializes values
void getprocess(FILE* fp, Process* process)
{
  if (fscanf(fp, "%d", &process->pid) == EOF || fscanf(fp, "%d", &process->arrival) == EOF || fscanf(fp, "%d", &process->burst) == EOF || fscanf(fp, "%d", &process->iobt) == EOF || fscanf(fp, "%d", &process->iofreq) == EOF) {
    printf("Number of listed values is less than the specfied amount.\n");
    exit(EXIT_FAILURE);
  } else {
    process->exectime = process->burst;
    process->arrtime = process->arrival;
    process->next = NULL;
    process->ready = 0;
    process->exectq = 0;
  }
}

// scans queues from file & initZ values
void getqueues(FILE* fp, Queue* queue)
{
  if (fscanf(fp, "%d", &queue->qid) == EOF || fscanf(fp, "%d", &queue->priority) == EOF || fscanf(fp, "%d", &queue->timequant) == EOF) {
    printf("Number of values is less than the specfied amount.\n");
    exit(EXIT_FAILURE);
  } else {
    queue->head = NULL;
    queue->tail = NULL;
  }
}

int checkprocesses(Process process[], int n)
{
  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++)
      if (process[i].pid == process[j].pid) {
        printf("PIDs should not be the same.\n");
        return 1;
      }
    if (process[i].pid < 0 || process[i].arrival < 0 || process[i].iobt < 0 || process[i].iofreq < 0 || process[i].burst < 0) {
      printf("Process values should not be negative.\n");
      return 1;
    }
  }
  return 0;
}

int checkqueues(Queue queue[], int n)
{
  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++)
      if (queue[i].qid == queue[j].qid || queue[i].priority == queue[j].priority) {
        printf("Neither QIDs nor priorities should have the same values.\n");
        return 1;
      }
    if (queue[i].qid < 0 || queue[i].priority < 0 || queue[i].timequant < 0) {
      printf("Queue values should not be negative.\n");
      return 1;
    }
  }
  return 0;
}
