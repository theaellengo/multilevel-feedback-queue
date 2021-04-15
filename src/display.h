#include "process.h"
#include <math.h>
#include <stdio.h>
#include <windows.h>

#ifndef DISPLAY
#define DISPLAY

void printprocess(Process process[], int n, Process queue[], int m, float awt);
void printgnatt(Process process[], int n);
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

void printgnatt(Process process[], int n)
{
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  printlabel("\nGnatt Chart:\n");
  for (int i = 0; i < n; i++) {
    if (i == 0 && process[i].start != 0) printf("[///] ");
    if (i > 0 && process[i - 1].completion != process[i].start) printf("[///] ");
    SetConsoleTextAttribute(hConsole, 15 * 16);
    printf(" %d", process[i].start);
    printf("%*c", (process[i].completion - process[i].start) / 2, ' ');
    SetConsoleTextAttribute(hConsole, 15 * 16 + 4);
    printf(" P%d ", process[i].pid);
    SetConsoleTextAttribute(hConsole, 15 * 16);
    printf("%*c", (process[i].completion - process[i].start) / 2, ' ');
    printf("%d ", process[i].completion);
    SetConsoleTextAttribute(hConsole, 15);
    printf(" ");
  }
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