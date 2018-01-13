/* Copyright 2017 Rose-Hulman Institute of Technology

 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include "./threadProblemSolver.h"
#include "./equalRoots.h"
#include "./realRoots.h"
#include "./complexRoots.h"

Coeffs quadCoefs[NUM_THREADS] = {{1, -10, -75},  {1, 2, 1}, {1, -10, 75}};

/* gcc compilation command:  

   gcc -ansi -Wall -pthread -g realRoots.c equalRoots.c complexRoots.c
       threadProblemSolver.c -lm -o threadProblemSolver
*/

int main(int argc, char *argv[]) {
  ThreadData *latestThread;
  char command;
  char line[MAX_LINE_LENGTH];

  latestThread = NULL;

  printf("Enter 'r' to create a new thread that computes real distinct "
         "roots for a quadratic equation.\n");
  printf("Enter 'e' to create a new thread that computes roots for a "
         "quadratic equation with equal roots.\n");
  printf("Enter 'c' to create a new thread that computes complex roots "
         "for a quadratic equation.\n");
  printf("Enter 'k' to cancel the most recently started thread.\n");
  printf("Enter 'q' to terminate the application.\n");

  while (1) {
    if (fgets(line, MAX_LINE_LENGTH, stdin) != NULL) {
      command = line[0];
    }
    if (command == 'e') {
      createThread(command, &latestThread);
    } else if (command == 'q') { /* quit program */
      return 0;
    } else {
      printf("This command is not supported.\n");
    }
  }
  return 0;
}

void createThread(char command, ThreadData **latestThread) {
  ThreadData *newThread;
  pthread_t current_tid;

  newThread = (ThreadData *)malloc(sizeof(ThreadData));
  newThread->previousThread = *latestThread;
  newThread->type = 'e';
  strncpy(newThread->name, "EQUAL", 6);

  printf("Create %s!\n", newThread->name);
  *latestThread = newThread;

  (*latestThread)->shouldRun = 1;
  pthread_create(&current_tid, NULL, runner, *latestThread);
  (*latestThread)->tid = current_tid;
}

void *runner(void *param) {
  ThreadData *data = (ThreadData *) param;

  printf("Hello from thread %s!\n", data->name);

  while (data->shouldRun) {
    if (data->type == 'e') {
      findEqualRoots(quadCoefs[EQUAL].a, quadCoefs[EQUAL].b,
                     quadCoefs[EQUAL].c);
    }
    data->type = '0'; /* DONT COMPUTE ROOTS AGAIN! */
    sleep(2);
  }
  pthread_exit(0);
}
