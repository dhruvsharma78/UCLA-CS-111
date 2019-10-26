// NAME: Dhruv Sharma
// EMAIL: dhruvsharma78@ucla.edu
// ID: 605082988

#include <sys/resource.h>
#include <sys/time.h>
#include "profile.h"

int getCurrentTime(int who, simpsh_timeinfo_t* ti){
  struct rusage usage;
  if(getrusage(who, &usage) != 0) return -1;
  ti->t_user = usage.ru_utime;
  ti->t_system = usage.ru_stime;
  return 0;
}

int getElapsedTime(int who, simpsh_timeinfo_t ti_start, simpsh_timeinfo_t* elapsedTime){
  simpsh_timeinfo_t ti_end;
  if(getCurrentTime(who, &ti_end) < 0) return -1;
  elapsedTime->t_user = getTimeDiff(ti_end.t_user, ti_start.t_user);
  elapsedTime->t_system = getTimeDiff(ti_end.t_system, ti_start.t_system);
  return 0;
}

/*
  This function was largely taken from
  https://www.gnu.org/software/libc/manual/html_node/Elapsed-Time.html
  I changed the implementation in many ways.
  1 - I return a struct containing the diff
  2 - I am guaranteed (unless the OS messes up) that the diff is not negative
      so I don't return that.
  3 - Input params are different, more suited for my needs.
*/
struct timeval getTimeDiff(struct timeval t1, struct timeval t2){
  struct timeval diff;
  if(t1.tv_usec < t2.tv_usec) {
    int nsec = (t2.tv_usec - t1.tv_usec) / 1000000 + 1;
    t2.tv_usec -= 1000000 * nsec;
    t2.tv_sec += nsec;
  }
  if(t1.tv_usec - t2.tv_usec > 1000000) {
    int nsec = (t1.tv_usec - t2.tv_usec) / 1000000;
    t2.tv_usec += 1000000 * nsec;
    t2.tv_sec -= nsec;
  }
  
  diff.tv_sec = t1.tv_sec - t2.tv_sec;
  diff.tv_usec = t1.tv_usec - t2.tv_usec;

  return diff;
}
