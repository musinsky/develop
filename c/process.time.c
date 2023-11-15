// 2023-11-15

// $ gcc -o process.time process.time.c && time ./process.time

#include <stdio.h>        // printf
#include <unistd.h>       // sleep
#include <stdlib.h>       // rand
#include <time.h>         // time, clock, clock_gettime
#include <sys/time.h>     // gettimeofday
#include <sys/resource.h> // getrusage

void do_something() {
  sleep(1);
  float value;
  for (int i = 0; i < 500000000; i++) {
    value = (float)rand()/(float)(RAND_MAX/100.0);
    value = value*(float)i;
  }
}

int main() {
  // https://stackoverflow.com/q/12392278   # Measure time in Linux
  //
  // https://en.cppreference.com/w/c/chrono/clock
  // https://www.gnu.org/software/libc/manual/html_node/Time-Types.html
  // https://www.gnu.org/software/libc/manual/html_node/CPU-Time.html
  // https://www.gnu.org/software/libc/manual/html_node/Getting-the-Time.html

  time_t t_start = time(NULL); // time_t {aka long int}
  do_something();
  time_t t_stop = time(NULL);
  printf("time():\t\t %ld s      "
         "# simplest function, only second resolution\n", t_stop - t_start);

  clock_t c_start = clock();   // clock_t {aka long int}
  do_something();
  clock_t c_stop = clock();
  printf("clock():\t %.2f s\n", ((float) (c_stop - c_start))/CLOCKS_PER_SEC);

  struct timespec gt_start, gt_stop;
  clock_gettime(CLOCK_MONOTONIC_RAW, &gt_start); // or CLOCK_PROCESS_CPUTIME_ID
  do_something();
  clock_gettime(CLOCK_MONOTONIC_RAW, &gt_stop);
  printf("clock_gettime(): %.2f s   "
         "# better (nanoseconds) resolution than clock()\n",
         (gt_stop.tv_nsec - gt_start.tv_nsec) / 1000000000.0 +
         (gt_stop.tv_sec  - gt_start.tv_sec));

  struct timeval gtd_start, gtd_stop;
  gettimeofday(&gtd_start, NULL);
  do_something();
  gettimeofday(&gtd_stop, NULL);
  printf("gettimeofday():\t %.2f s   "
         "# obsolete, recommended use clock_gettime() instead\n",
         (gtd_stop.tv_usec - gtd_start.tv_usec) / 1000000.0 +
         (gtd_stop.tv_sec  - gtd_start.tv_sec));

  // https://www.gnu.org/software/libc/manual/html_node/Resource-Usage.html
  struct rusage gru_start, gru_stop;
  getrusage(RUSAGE_SELF, &gru_start);
  do_something();
  getrusage(RUSAGE_SELF, &gru_stop);
  printf("getrusage():\t %.2f s   "
         "# get resource usage (user CPU time used)\n",
         // struct timeval ru_utime;   // user CPU time used
         (gru_stop.ru_utime.tv_usec - gru_start.ru_utime.tv_usec) / 1000000.0 +
         (gru_stop.ru_utime.tv_sec  - gru_start.ru_utime.tv_sec));
}
