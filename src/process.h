#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef PROCESS
#define PROCESS

/** Process Struct **/
typedef struct process {
  int pid;
  int arrival, arrtime;
  int burst, exectime;
  int iobt, iofreq;
  int start;
  int completion;
  int waiting;
  int turnaround;
  int ready;
  struct process* next;
} Process;

/** Function Declarations **/
Process* pcopy(Process* process);
void setprocess(Process* process, int* clock, int exectime);
void sortbyarrival(Process process[], int n);
void sortbypid(Process process[], int n);

void setprocess(Process* process, int* clock, int exectime)
{
  process->start = *clock;
  process->completion = process->start + exectime;
  process->turnaround = process->completion - process->arrival;
  process->waiting = process->turnaround - (process->burst - process->exectime);
}

// copies values of process into another array
Process* pcopy(Process* p)
{
  Process* temp = (Process*)malloc(sizeof(Process));
  temp->pid = p->pid;
  temp->arrival = p->arrival;
  temp->burst = p->burst;
  temp->arrtime = p->arrtime;
  temp->exectime = p->exectime;
  temp->next = NULL;
  return temp;
}

// sorts processes by arrival time
void sortbyarrival(Process process[], int n)
{
  Process temp;
  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++) {
      if (process[i].arrtime > process[j].arrtime) {
        temp = process[i];
        process[i] = process[j];
        process[j] = temp;
      }
    }
  }
}

void sortbypid(Process process[], int n)
{
  Process temp;
  for (int i = 0; i < n; i++)
    for (int j = i + 1; j < n; j++)
      if (process[i].pid > process[j].pid) {
        temp = process[i];
        process[i] = process[j];
        process[j] = temp;
      }
}

#endif