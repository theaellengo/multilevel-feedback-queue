/***************************************************************
Name: Go, Thea Ellen
      Chong, Dianne
Group: 22
Section: S12
***************************************************************/

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

  // get x queues
  Queue queue[xys[0]];
  for (int i = 0; i < xys[0]; i++)
    getqueues(fp, &queue[i]);

  // get y processes
  Process process[xys[1]];
  for (int i = 0; i < xys[1]; i++)
    getprocess(fp, &process[i]);

  // close file
  fclose(fp);

  if (xys[0] < 2 || xys[0] > 5)
    printf("There are too many number of queues (2, 5).\n");
  else if (xys[1] < 3 || xys[1] > 100)
    printf("Number of processes should be in range (3, 100).\n");
  else if (xys[2] < 0)
    printf("Priority boost period should not be negative\n");
  else if (checkprocesses(process, xys[1])) {
  } else if (checkqueues(queue, xys[0])) {
  } else {
    mlfq(queue, xys[0], process, xys[1], xys[2]);
  }
  return 0;
}

// scans processes from file & initializes values
void getprocess(FILE* fp, Process* process)
{
  if (feof(fp)) {
    printf("Number of listed processes is less than the specfied amount.\n");
    exit(EXIT_FAILURE);
  } else {
    fscanf(fp, "%d", &process->pid);
    fscanf(fp, "%d", &process->arrival);
    fscanf(fp, "%d", &process->burst);
    fscanf(fp, "%d", &process->iobt);
    fscanf(fp, "%d", &process->iofreq);
    process->exectime = process->burst;
    process->arrtime = process->arrival;
  }
}

// scans queues from file
void getqueues(FILE* fp, Queue* queue)
{
  if (feof(fp)) {
    printf("Number of listed queues is less than the specfied amount.\n");
    exit(EXIT_FAILURE);
  } else {
    fscanf(fp, "%d", &queue->qid);
    fscanf(fp, "%d", &queue->priority);
    fscanf(fp, "%d", &queue->timequant);
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
    if (process[i].pid < 0) {
      printf("PIDs should not be negative.\n");
      return 1;
    }
    if (process[i].arrival < 0) {
      printf("Arrival should not be negative.\n");
      return 1;
    }
    if (process[i].iobt < 0) {
      printf("IO burst time should not be negative.\n");
      return 1;
    }
    if (process[i].iofreq < 0) {
      printf("Frequency of IO burst should not be negative.\n");
      return 1;
    }
    if (process[i].burst < 0) {
      printf("Burst should not be negative.\n");
      return 1;
    }
  }
  return 0;
}

int checkqueues(Queue queue[], int n)
{
  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++)
      if (queue[i].qid == queue[j].qid) {
        printf("QIDs should not be the same.\n");
        return 1;
      }
    if (queue[i].qid < 0) {
      printf("QIDs should not be negative.\n");
      return 1;
    }
    if (queue[i].priority < 0) {
      printf("Priority should not be negative.\n");
      return 1;
    }
    if (queue[i].timequant < 0) {
      printf("Time quantum should not be negative.\n");
      return 1;
    }
  }
  return 0;
}