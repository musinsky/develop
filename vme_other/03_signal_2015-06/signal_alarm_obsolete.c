// Musinsky Jan
// 2015-06-15

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>


char *printTime(const char *fmt)
{
  static char outstr[200];
  time_t t;
  struct tm *loc;

  t = time(NULL);
  loc = localtime(&t);
  if (loc == NULL) {
    perror("localtime failed");
    return NULL;
  }

  printf("custom: %02dhod%02dmin%02dsek\n", loc->tm_hour, loc->tm_min, loc->tm_sec);

  if (strftime(outstr, sizeof(outstr), (fmt != NULL) ? fmt : "%T", loc) != 0) {
    printf("strftime: %s\n", outstr);
    return outstr;
  }
  return NULL;
}

unsigned int alarm2(unsigned int seconds)
{
  struct itimerval old, new;
  new.it_interval.tv_usec = 0;
  new.it_interval.tv_sec = 0;
  new.it_value.tv_usec = 0;
  new.it_value.tv_sec = (long int) seconds;
  if (setitimer(ITIMER_REAL, &new, &old) < 0) return 0;
  else return old.it_value.tv_sec;
}

/* This flag controls termination of the main loop. */
volatile sig_atomic_t keep_going = 1;

/* The signal handler just clears the flag and re-enables itself. */
void catch_alarm(int sig)
{
  printf("!!!!!!!!!!!!!!!!!!!!!catch_alarm");
  keep_going = 0;
  signal(sig, catch_alarm);
}

void do_stuff(void)
{
  puts("Doing stuff while waiting for alarm....");
  printTime(NULL);
}

int main(void)
{
  /* Establish a handler for SIGALRM signals. */
  //signal(SIGINT, catch_alarm); (Ctrl+C)
  signal(SIGALRM, catch_alarm);

  // signal(SIGALRM, SIG_DFL); - ak je v loope tak, druhy signal uz bude occurs
  // SIG_IGN, then the signal is ignored.
  // SIG_DFL, then the default action associated with the signal (see signal(7)) occurs.

  /* Set an alarm to go off in a little while. */
  // alarm(3); // emituje SIGALRM
  //alarm(3);  // neemituje SIGALRM

  /* Check the flag once in a while to see when to quit. */
  while (keep_going) {
    sleep(1);
    do_stuff();
  }

  return EXIT_SUCCESS;
}
