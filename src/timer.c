#define __need_clockid_t

#include <sys/timerfd.h>
#include <time.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <sched.h>
#include <unistd.h>
#include <pthread.h>

#include "error.h"

#include "timer.h"

#define CLOCKID CLOCK_REALTIME
#define SIG SIGRTMIN

typedef struct
{
  int timer_fd;
  unsigned long long wakeups_missed;
} real_timer_t;

real_timer_t real_timer;

static int make_periodic(unsigned int period)
{
  int ret;
  unsigned int ns;
  unsigned int sec;
  int fd;
  struct itimerspec itval;

  /* Create the timer */
  fd = timerfd_create(CLOCK_MONOTONIC, 0);
  real_timer.wakeups_missed = 0;
  real_timer.timer_fd = fd;
  if (-1 == fd) {
    return fd;
  }

  /* Make the timer periodic */
  sec = period/1000000;
  ns = (period - (sec * 1000000)) * 1000;
  itval.it_interval.tv_sec = sec;
  itval.it_interval.tv_nsec = ns;
  itval.it_value.tv_sec = sec;
  itval.it_value.tv_nsec = ns;
  ret = timerfd_settime(fd, 0, &itval, NULL);

  return ret;
}

static void wait_period()
{
  unsigned long long missed;
  int ret;

  /* Wait for the next timer event. If we have missed any the
     number is written to "missed" */
  ret = read(real_timer.timer_fd, &missed, sizeof (missed));
  if (ret == -1) {
    error("Unable to read timer");
    return;
  }

  real_timer.wakeups_missed += missed;
}

void timer_setup()
{
  memset(&real_timer, 0, sizeof(real_timer));
  /*
   * Default is 4/4 tempo with 4 pattern rows each beat, this means that
   * the timer must trigger 1020 times per second to be able to run at
   * 255bpm as speed.
   */
  make_periodic(1000 * 10);
}

void timer_wait()
{
  wait_period();
}

void timer_cleanup()
{
}
