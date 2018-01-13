#ifndef _THREAD_PROBLEM_SOLVER_H_
#define _THREAD_PROBLEM_SOLVER_H_

struct thread_data {
  int shouldRun;
  char name[101];
  char type;
  pthread_t tid;
  struct thread_data *previousThread;
};

typedef struct thread_data ThreadData;

/* Coefficients for quadratic equation */
typedef struct {
  int a;
  int b;
  int c;
} Coeffs;

#define REAL 0
#define EQUAL 1
#define COMPLEX 2
#define NUM_THREADS 3
#define MAX_LINE_LENGTH 80

void createThread(char command, ThreadData **latestThread);

void cancelThread(ThreadData **latestThread);

void redoComputation(ThreadData *latestThread);

void *runner(void *param);

#endif /* _THREAD_PROBLEM_SOLVER_H_ */
