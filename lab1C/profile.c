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
  elapsedTime->t_user = getTimeDiff(ti_start.t_user, ti_end.t_user);
  elapsedTime->t_system = getTimeDiff(ti_start.t_system, ti_end.t_system);
  return 0;
}

struct timeval getTimeDiff(struct timeval ti_start, struct timeval ti_end){
  struct timeval diff;
  if(ti_start.tv_usec < ti_end.tv_usec) {
    int nsec = (ti_end.tv_usec - ti_start.tv_usec) / 1000000 + 1;
    ti_end.tv_usec -= 1000000 * nsec;
    ti_end.tv_sec += nsec;
  }
  if(ti_start.tv_usec - ti_end.tv_usec > 1000000) {
    int nsec = (ti_start.tv_usec - ti_end.tv_usec) / 1000000;
    ti_end.tv_usec += 1000000 * nsec;
    ti_end.tv_sec -= nsec;
  }
  
  diff.tv_sec = ti_start.tv_sec - ti_end.tv_sec;
  diff.tv_usec = ti_start.tv_usec - ti_end.tv_usec;

  return diff;
}
