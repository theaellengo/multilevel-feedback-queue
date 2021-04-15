#include "process.h"
#include <stdio.h>

#ifndef QUEUE
#define QUEUE

/** Struct of Queue**/
typedef struct queue {
  int qid;
  int priority;
  int timequant;
  //  Process processes[];
} Queue;

#endif