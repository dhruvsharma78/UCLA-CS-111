// NAME: Dhruv Sharma
// EMAIL: dhruvsharma78@ucla.edu
// ID: 605082988

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <getopt.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

#include "command.h"
#include "commands.h"
#include "optionparsing.h"
#include "file.h"
#include "signal.h"

#define BUF_SIZE 2048

int main(int argc, char* argv[]){
  simpsh_commands_t* cmds = SIMPSH_COMMANDS_T_INIT();
  simpsh_filetable_t* ft = SIMPSH_FILETABLE_T_INIT();
  int file_flags = 0;
  int c;
  int verbose = 0;
  int profileFlag = 0;
  int maxError = 0;
  int maxSignal = 0;
  int terminatedWithSignal = 0;
  opterr = 0;
  while(1){
    c = getopt_long(argc, argv, "", simpsh_long_options, NULL);
    if(c == -1) break;
    switch(c){
    case SIMPSH_O_PROFILE:
      profileFlag = 1;
      if(profileFlag){
	printf("profile here...");
      }
      break;
    case SIMPSH_O_VERBOSE:
      verbose = 1;
      if(verbose){
	fprintf(stdout, "%s\n", argv[optind - 1]);
	fflush(stdout);
      }
      break;
    case SIMPSH_O_APPEND:
    case SIMPSH_O_CLOEXEC:
    case SIMPSH_O_CREAT:
    case SIMPSH_O_DIRECTORY:
    case SIMPSH_O_DSYNC:
    case SIMPSH_O_EXCL:
    case SIMPSH_O_NOFOLLOW:
    case SIMPSH_O_NONBLOCK:
    case SIMPSH_O_RSYNC:
    case SIMPSH_O_SYNC:
    case SIMPSH_O_TRUNC:
      if(verbose){
	fprintf(stdout, "%s\n", argv[optind - 1]);
	fflush(stdout);
      }
      updateFlags(&file_flags, c);
      break;
    case SIMPSH_O_WR_ONLY:
    case SIMPSH_O_RD_WR:
    case SIMPSH_O_RD_ONLY:{
      if(verbose){
	fprintf(stdout, "%s ", argv[optind - 2]);
	fprintf(stdout, "%s\n", argv[optind - 1]);
	fflush(stdout);
      }
      updateFlags(&file_flags, c);
      int fd = open(optarg, file_flags, 0644);
      int err = errno;
      int status = 1;
      if(fd < 0){
        status = 0;
        char em[10];
        switch(c){
        case SIMPSH_O_RD_ONLY:
          strcpy(em, "--rdonly");
          break;
        case SIMPSH_O_RD_WR:
          strcpy(em, "--rdwr");
          break;
        case SIMPSH_O_WR_ONLY:
          strcpy(em, "--wronly");
          break;
        }
        fprintf(stderr, "%s: ERROR: %s\n\tFile \'%s\' could not be opened/created\n", argv[0], em, optarg);
        fprintf(stderr, "\t%s\n", strerror(err));
	fflush(stderr);
        maxError = 1;
      }
      addFile(ft, fd, status);
      clearFlags(&file_flags);
      break;
    }
    case SIMPSH_O_PIPE:
      if(verbose){
	fprintf(stdout, "%s\n", argv[optind - 1]);
	fflush(stdout);
      }
      int temp_fd[2];
      int status = 1;
      if(pipe(temp_fd) == -1){
        status = 0;
        fprintf(stderr, "%s: ERROR: Pipe creation failiure\n", argv[0]);
        fprintf(stderr, "\t%s\n", strerror(errno));
	fflush(stderr);
      }
      addFile(ft, temp_fd[0], status);
      addFile(ft, temp_fd[1], status);
      break;
    case SIMPSH_O_COMMAND:{
      if(verbose){
	fprintf(stdout, "%s ", argv[optind - 2]);
	fflush(stdout);
      }
      int index = optind - 1;
      simpsh_command_t* newCmd = SIMPSH_COMMAND_T_INIT();
      int arg_ind = 0;
      while(index < argc){
        if(argv[index][0] == '-' && argv[index][1] == '-'){
          break;
	}
        if(verbose){
	  fprintf(stdout, "%s ", argv[index]);
	  fflush(stdout);
	}
        if(arg_ind >= 0 && arg_ind <= 2){
          newCmd->fdTable[arg_ind] = atoi(argv[index]);
        }else if(arg_ind >= 3){
          addArgument(newCmd, argv[index]);
          if(arg_ind == 3) newCmd->processName = strdup(argv[index]);
        }
        ++arg_ind;
        ++index;
      }
      if(arg_ind <= 3){
	fprintf(stderr, "%s: ERROR: No command given to --command option\n", argv[0]);
	fflush(stderr);
	SIMPSH_COMMAND_T_DESTROY(newCmd);
	maxError = 1;
	break;
      }
      optind = index;
      if(verbose){
	fprintf(stdout, "\n");
	fflush(stdout);
      }
      addCommand(cmds, newCmd);
      simpsh_command_t* cmd = newCmd;
      int i;
      int filesExist = 1;
      for(i=0; i<3; i++){
        int active;
        int found = findFile(ft, cmd->fdTable[i], &active);
        if(!found || !active){
          filesExist = 0;
          break;
	}
      }
      if(!filesExist){
        maxError = 1;
        fprintf(stderr, "%s: ERROR: Incorrect file descriptor numbers...\n", argv[0]);
	fflush(stderr);
        continue;
      }
      pid_t p = fork();
      int err;
      if(p < 0){
        fprintf(stderr, "%s: ERROR: forking unsuccessful\n", argv[0]);
	fflush(stderr);
        _exit(1);
      }else if(p == 0){
        int err_fd = dup(STDERR_FILENO);
	if((err = replaceFiles(ft, cmd->fdTable) != 0 )){
          if(err == 1) dprintf(err_fd, "%s: ERROR: I/O Redirection failed\n", argv[0]);
          else dprintf(err_fd, "%s: ERROR: Unknown error occured\n", argv[0]);
	  _exit(1);
	}
	closeAllFiles(ft);
        execvp(cmd->processName, cmd->arguments);
        _exit(2);
      }else{
	      cmd->processID = p;
      }
      break;
    }
    case SIMPSH_O_CLOSE:
      if(verbose){
	fprintf(stdout, "%s ", argv[optind - 2]);
	fprintf(stdout, "%s\n", argv[optind - 1]);
	fflush(stdout);
      }
      if(closeFile(ft, atoi(optarg))<0){
        fprintf(stderr, "%s: ERROR: An error occured while closing file \'%d\'...\n", argv[0], atoi(optarg));
        fprintf(stderr, "\t%s\n", strerror(errno));
	fflush(stderr);
      }
      break;
    case SIMPSH_O_WAIT:
      if(verbose){
	fprintf(stdout, "%s\n", argv[optind - 1]);
	fflush(stdout);
      }
      while(1){
	int status;
	pid_t finished = wait(&status);
	if(finished == -1){
	  if(errno == ECHILD) break;
	  else{
	    fprintf(stderr, "%s: ERROR. An error occured while waiting for child processes to finish\n\t%s", argv[0], strerror(errno));
	    fflush(stderr);
	  }
	}else if(finished != 0){
	  char exitString[BUF_SIZE];
	  exitString[0] = 0;
	  if(WIFEXITED(status)){
	    status = WEXITSTATUS(status);
	    fprintf(stdout, "exit %d %s\n", status, getCommandExitString(cmds, finished, exitString));
	    maxError = status > maxError ? status : maxError;
	  }else if(WIFSIGNALED(status)){
	    status = WTERMSIG(status);
	    terminatedWithSignal = 1;
	    maxSignal = status > maxSignal ? status : maxSignal;
	    fprintf(stdout, "signal %d %s\n", status, getCommandExitString(cmds, finished, exitString));
	  }
	  fflush(stdout);
	  exitString[0] = 0;
	}
      }
      break;
    case SIMPSH_O_CHDIR:
      if(verbose){
	fprintf(stdout, "%s ", argv[optind - 2]);
	fprintf(stdout, "%s\n", argv[optind - 1]);
	fflush(stdout);
      }
      if(chdir(optarg) < 0){
        fprintf(stderr, "%s: ERROR: An error occured while changing directories to \'%s\'...\n", argv[0], optarg);
        fprintf(stderr, "\t%s\n", strerror(errno));
	fflush(stderr);
	maxError = 1;
      }
      break;
    case SIMPSH_O_CATCH:
      if(verbose){
	fprintf(stdout, "%s ", argv[optind - 2]);
	fprintf(stdout, "%s\n", argv[optind - 1]);
	fflush(stdout);
      }
      signal(atoi(optarg), simpshSignalHandler);
      break;
    case SIMPSH_O_DEFAULT:
      if(verbose){
	fprintf(stdout, "%s ", argv[optind - 2]);
	fprintf(stdout, "%s\n", argv[optind - 1]);
	fflush(stdout);
      }
      signal(atoi(optarg), SIG_DFL);
      break;
    case SIMPSH_O_IGNORE:
      if(verbose){
	fprintf(stdout, "%s ", argv[optind - 2]);
	fprintf(stdout, "%s\n", argv[optind - 1]);
	fflush(stdout);
      }
      signal(atoi(optarg), SIG_IGN);
      break;
    case SIMPSH_O_PAUSE:
      pause();
      break;
    case SIMPSH_O_ABORT:{
      if(verbose){
	fprintf(stdout, "--abort\n");
	fflush(stdout);
      }
      fflush(stdout);
      fflush(stderr);
      raise(SIGSEGV);
      break;
    }
    case '?':
      if(argv[optind-1][0] == '-' && argv[optind-1][1] != '-') break;
      fprintf(stderr, "%s: ERROR: Unrecognized option \'%s\'...\n", argv[0], argv[optind-1]);
      fflush(stderr);
      maxError = 1;
      break;
    }
  }

  closeAllFiles(ft);

  // Free memory
  SIMPSH_COMMANDS_T_DESTROY(cmds);
  SIMPSH_FILETABLE_T_DESTROY(ft);

  if(terminatedWithSignal > 0){
    signal(maxSignal, SIG_DFL);
    raise(maxSignal);
  }
  exit(maxError);
}
