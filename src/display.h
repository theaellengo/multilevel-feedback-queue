#include "process.h"
#include "queue.h"
#include <math.h>
#include <stdio.h>
#include <windows.h>

#ifndef DISPLAY
#define DISPLAY

void printprocess2(Process process[], int n, Process queue[], int m, float awt);
void printgnatt(Queue gnatt);
void printlabel(char* str);
int getnumdigits(int num);

void printprocess(Queue q[], Queue io, Process p[], int x, int y, float awt)
{
  sortbypid(p, y);
  printlabel("\n\nList of Processes\n");
  for (int i = 0; i < y; i++) {

    Queue temp = initqueue(-1);
    int count = 0;
    printf("------------------------------------------------------------\n");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
    printf("Process %d    Queue     Start    End    Waiting    Turnaround\n", p[i].pid);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

    for (int j = 0; j < x + 1; j++) {
      Process* curr = q[j].head;

      while (curr != NULL) {
        if (curr->pid == p[i].pid) {
          if (j == x) curr->qid = -1;
          enqueue(&temp, pcopy(curr));
          count++;
        }
        curr = curr->next;
      }
    }

    Process temparr[count];
    for (int k = 0; k < count; k++) {
      if (temp.head != NULL) temparr[k] = *dequeue(&temp);
    }

    sortbystart(temparr, count);
    for (int k = 0; k < count; k++) {
      enqueue(&temp, &temparr[k]);
    }

    Process* curr = temp.head;
    float waiting = 0;
    float turnaround = 0;
    while (curr != NULL) {
      if (curr->pid == p[i].pid) {
        if (curr->qid != -1) {
          printf("%*c%d%*c", 13, ' ', curr->qid, 8 - getnumdigits(curr->qid), ' ');
        } else {
          printf("%*cIO%*c", 13, ' ', 7, ' ');
        }
        printf("%d%*c", curr->start, 8 - getnumdigits(curr->start), ' ');
        printf("%d%*c", curr->completion, 6 - getnumdigits(curr->completion), ' ');
        printf("%d%*c", curr->waiting, 10 - getnumdigits(curr->waiting), ' ');
        printf("%d\n", curr->turnaround);
      }
      waiting = curr->waiting;
      turnaround = curr->turnaround;
      curr = curr->next;
    }
    printf("\nWaiting Time: %.1f", waiting);
    printf("\nTurnaround Time: %.1f\n", turnaround);
  }

  printf("------------------------------------------------------------");
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
  printf("\nAverage Waiting Time: %.1f\n\n", awt);
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}

void printgnatt(Queue gnatt)
{
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
  if (gnatt.qid != -1) {
    printf("\nQueue %d\n", gnatt.qid);
  } else {
    printf("\nIO\n", gnatt.qid);
  }
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
  Process* curr = gnatt.head;
  Process* last = gnatt.head;
  while (curr != NULL) {
    if (last == curr) {
      if (curr->start != 0) printf("[///] ");
    } else if (last->completion < curr->start) {
      printf("[///] ");
    }
    SetConsoleTextAttribute(hConsole, 15 * 16);
    printf(" %d", curr->start);
    printf("%*c", (curr->completion - curr->start) / 2, ' ');
    SetConsoleTextAttribute(hConsole, 15 * 16 + 4);
    printf(" P%d ", curr->pid);
    printf("%*c", (curr->completion - curr->start) / 2, ' ');
    SetConsoleTextAttribute(hConsole, 15 * 16);
    printf("%d ", curr->completion);
    SetConsoleTextAttribute(hConsole, 15);
    printf(" ");
    last = curr;
    curr = curr->next;
  }
  printf("\n");
}

void printlabel(char* str)
{
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
  printf("%s", str);
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}

int getnumdigits(int num)
{
  if (num < 0) num *= -10;
  return (num == 0) ? 0 : floor(log10(num));
}

#endif