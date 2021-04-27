#include "process.h"
#include "queue.h"
#include <math.h>
#include <stdio.h>
#include <windows.h>

#ifndef DISPLAY
#define DISPLAY

void printprocess(Process process[], int n, Process queue[], int m, float awt);
void printgnatt(Queue gnatt);
void printlabel(char* str);
int getnumdigits(int num);

void printprocess(Process process[], int n, Process queue[], int m, float awt)
{
  sortbypid(process, n);
  printlabel("\n\nProcess    Start    End    Waiting    Turnaround\n");
  for (int i = 0; i < n; i++) {
    int flag = 0;
    printf("--------------------------------------------------\n");
    printf("%d%*c", process[i].pid, 10 - getnumdigits(process[i].pid), ' ');
    for (int j = 0; j < m; j++) {
      if (process[i].pid == queue[j].pid) {
        if (flag == 1) printf("%*c", 11 - getnumdigits(process[i].pid) + getnumdigits(process[i].pid), ' ');
        printf("%d%*c", queue[j].start, 8 - getnumdigits(queue[j].start), ' ');
        printf("%d%*c", queue[j].completion, 6 - getnumdigits(queue[j].completion), ' ');
        printf("%d%*c", queue[j].waiting, 10 - getnumdigits(queue[j].waiting), ' ');
        printf("%d\n", queue[j].turnaround);
        flag = 1;
      }
    }
  }
  printf("--------------------------------------------------");
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
  printf("\nAverage Waiting Time: %.1f\n\n", awt);
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}

void printgnatt(Queue gnatt)
{
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
  printf("\nQueue: %d\n", gnatt.qid);
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
  Process* curr = gnatt.head;
  Process* last = gnatt.head;
  while (curr != NULL) {
    if (last == curr) {
      if (curr->start != 0) printf("[///] ");
    } else if (last->completion != curr->start) {
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
  return (num == 0) ? 0 : floor(log10(num));
}

#endif