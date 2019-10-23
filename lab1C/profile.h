// NAME: Dhruv Sharma
// EMAIL: dhruvsharma78@ucla.edu
// ID: 605082988

#ifndef PROFILE_H
#define PROFILE_H

#include <sys/resource.h>
#include <sys/time.h>

typedef struct{
  struct timeval t_user;
  struct timeval t_system;
} simpsh_timeinfo_t;

int getCurrentTime(int, simpsh_timeinfo_t*);
int getElapsedTime(int, simpsh_timeinfo_t, simpsh_timeinfo_t*);
struct timeval getTimeDiff(struct timeval, struct timeval);

#endif
