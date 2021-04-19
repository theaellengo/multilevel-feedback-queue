#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef PROCESS
#define PROCESS

/** Struct of Process**/
typedef struct process {
  int pid;
  int arrival;
  int burst;
  int arrtime; // arrival time left
  int exectime; // execution time left
  int iobt; // IO burst time
  int iofreq; // IO burst frequency
  int start; // start time
  int completion; // end time
  int waiting;
  int turnaround;
  struct process* next;
} Process;

/** Function Declarations **/
void setprocess(Process* process, int* clock, int exectime);
void sortbyarrival(Process process[], int n);
void sortbyburst(Process process[], int n);
void sortbypid(Process process[], int n);

void setprocess(Process* process, int* clock, int exectime)
{
  process->start = *clock;
  process->exectime -= exectime;
  process->completion = process->start + exectime;
  // turnaround = completion time - arrival time
  process->turnaround = process->completion - process->arrival;
  // waiting = (tunraround - burst) || (start - arrival)
  process->waiting = process->turnaround - (process->burst - process->exectime);
  *clock += exectime;
}

// sorts processes by arrival time
void sortbyarrival(Process process[], int n)
{
  Process temp;
  for (int i = 0; i < n; i++)
    for (int j = i + 1; j < n; j++) {
      if (process[i].arrtime > process[j].arrtime) {
        temp = process[i];
        process[i] = process[j];
        process[j] = temp;
      }
    }
}

// sort processes by remaining burst time
void sortbyburst(Process process[], int n)
{
  Process temp;
  for (int i = 0; i < n; i++)
    for (int j = i + 1; j < n; j++) {
      if (process[i].exectime > process[j].exectime) {
        temp = process[i];
        process[i] = process[j];
        process[j] = temp;
      }
      // if burst times are equal, sort by arrival time
      if (process[i].exectime == process[j].exectime) {
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