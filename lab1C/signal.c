// NAME: Dhruv Sharma
// EMAIL: dhruvsharma78@ucla.edu
// ID: 605082988

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "signal.h"

void simpshSignalHandler(int signum){
  char num[4];
  num[3] = 0;
  int size;
  int done;
  int written;
  done = 0;
  size = itoa_async_signal_safe(num, signum);
  written = 0;
  while((done = write(STDERR_FILENO, num+(3-size)+written, size-written)) > 0){
    written += done;
  }
  done = 0;
  written = 0;
  while((done = write(STDERR_FILENO, " caught\n"+written, 8-written)) > 0){
    written += done;
  }
  exit(signum);
}

int itoa_async_signal_safe(char* buf, int a){
  int index = 2;
  while(a != 0){
    buf[index--] = (a%10) + '0';
    a/=10;
  }
  return 2-index;
}
