// NAME: Dhruv Sharma
// EMAIL: dhruvsharma78@ucla.edu
// ID: 605082988

#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

#define BUF_SIZE 1024

#define INPUT 'i'
#define OUTPUT 'o'
#define SEG 's'
#define CATCH 'c'
#define CORE_DUMP 'd'

void usage(){
  fprintf(stderr, "usage: ./lab0 [--input i] [--output o] [--segfault] [--catch] [--dump-core]\n");
  fprintf(stderr, "\t --input i: input file to be used\n");
  fprintf(stderr, "\t --input o: output file to be used\n");
}

void signalHandler(int signum){
  switch(signum){
  case SIGSEGV:{
    fprintf(stderr, "Error: Segmentation fault has occured: Signal number: %d\n", signum);
    break;
  }
  }
  exit(4);
}

void causeSegFault(){
  char* cp = NULL;
  *cp = 'd';
}

int main(int argc, char* argv[]){

  int c;
  opterr = 0;

  static struct option long_options[] = {
    {"input", required_argument, 0, INPUT},
    {"output", required_argument, 0, OUTPUT},
    {"segfault", no_argument, 0, SEG},
    {"catch", no_argument, 0, CATCH},
    {"dump-core", no_argument, 0, CORE_DUMP},
    {0, 0, 0, 0}
  };
  
  while(1){
    c = getopt_long(argc, argv, "", long_options, NULL);
    if(c == -1){
      break;
    }
    switch(c){
    case INPUT:{
      close(0);
      int fd = open(optarg, O_RDONLY);
      int err = errno;
      if(fd < 0){
 	fprintf(stderr, "%s: Error: --input\n\tFile \'%s\' could not be opened\n", argv[0], optarg);
	fprintf(stderr, "\t%s\n", strerror(err));
	exit(2);
      }
      if(dup2(fd, 0) < 0){
	fprintf(stderr, "%s: Error: input file stream failed to connect\n", argv[0]);
	fprintf(stderr, "\t%s\n", strerror(errno));
	exit(2);
      }
      break;
    }
    case OUTPUT:{
      close(1);
      int fd = open(optarg, O_WRONLY | O_CREAT | O_TRUNC, 0644);
      int err = errno;
      if(fd < 0){
	fprintf(stderr, "%s: Error: --output\n\tFile \'%s\' could not be opened/created\n", argv[0], optarg);
	fprintf(stderr, "\t%s\n", strerror(err));
	exit(3);
      }
      if(dup2(fd, 1) < 0){
        fprintf(stderr, "%s: Error: output file stream failed to connect\n", argv[0]);
        fprintf(stderr, "\t%s\n", strerror(errno));
        exit(2);
      }
      break;
    }
    case SEG:{
      causeSegFault();
      break;
    }
    case CATCH:{
      signal(SIGSEGV, signalHandler);
      break;
    }
    case CORE_DUMP:{
      signal(SIGSEGV, SIG_DFL);
      break;
    }
    case '?':{
      fprintf(stderr, "%s: Error: Unrecognized option \'%s\'\n\n", argv[0], argv[optind-1]);
      usage();
      exit(1);
      break;
    }
    }
  }
  
  char buf[BUF_SIZE];
  int bytesRead;
  while((bytesRead = read(0, buf, BUF_SIZE)) > 0){
    if(write(1, buf, bytesRead) < 0){
      fprintf(stderr, "%s: Error: Write unsuccessfull\n", argv[0]);
      fprintf(stderr, "\t%s\n", strerror(errno));
    }
  }
  if(bytesRead < 0){
    fprintf(stderr, "%s: Error: Read unsuccessfull\n", argv[0]);
    fprintf(stderr, "\t%s\n", strerror(errno));
  }
  close(0);
  close(1);
  return 0;
}
