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

#define BUF_SIZE 2048

int main(int argc, char* argv[]){
  simpsh_commands_t* cmds = SIMPSH_COMMANDS_T_INIT();
  simpsh_filetable_t* ft = SIMPSH_FILETABLE_T_INIT();
  int file_flags = 0;
  int waitFlag = 0;
  int c;
  int verbose = 0;
  int maxError = 0;
  opterr = 0;
  while(1){
    c = getopt_long(argc, argv, "", simpsh_long_options, NULL);
    if(c == -1) break;
    switch(c){
    case SIMPSH_O_VERBOSE:
      verbose = 1;
      if(verbose) fprintf(stdout, "%s\n", argv[optind - 1]);
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
      if(verbose) fprintf(stdout, "%s\n", argv[optind - 1]);
      updateFlags(&file_flags, c);
      break;
    case SIMPSH_O_WR_ONLY:
    case SIMPSH_O_RD_WR:
    case SIMPSH_O_RD_ONLY:{
      if(verbose) fprintf(stdout, "%s ", argv[optind - 2]);
      if(verbose) fprintf(stdout, "%s\n", argv[optind - 1]);
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
	maxError = 1;
      }
      addFile(ft, fd, status);
      clearFlags(&file_flags);
      break;
    }
    case SIMPSH_O_PIPE:
      if(verbose) fprintf(stdout, "%s\n", argv[optind - 1]);
      int temp_fd[2];
      int status = 1;
      if(pipe(temp_fd) == -1){
	status = 0;
	fprintf(stderr, "%s: ERROR: Pipe creation failiure\n", argv[0]);
	fprintf(stderr, "\t%s\n", strerror(errno));
      }
      addFile(ft, temp_fd[0], status);
      addFile(ft, temp_fd[1], status);
      break;
    case SIMPSH_O_COMMAND:{
      if(verbose) fprintf(stdout, "%s ", argv[optind - 2]);
      int index = optind - 1;
      simpsh_command_t* newCmd = SIMPSH_COMMAND_T_INIT();
      int arg_ind = 0;
      while(index < argc){
	if(argv[index][0] == '-' && argv[index][1] == '-'){
	  break;
	}
	if(verbose) fprintf(stdout, "%s ", argv[index]);
	if(arg_ind >= 0 && arg_ind <= 2){
	  newCmd->fdTable[arg_ind] = atoi(argv[index]);
	}else if(arg_ind >= 3){
	  addArgument(newCmd, argv[index]);
	  if(arg_ind == 3) newCmd->processName = strdup(argv[index]);
	}
	++arg_ind;
	++index;
      }
      if(verbose) fprintf(stdout, "\n");
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
	continue;
      }
      pid_t p = fork();
      int err;
      if(p < 0){
	fprintf(stderr, "%s: ERROR: forking unsuccessful\n", argv[0]);
	_exit(1);
      }else if(p == 0){
	int err_fd = dup(STDERR_FILENO);
	closeStandardFiles();
	if((err = replaceFiles(ft, cmd->fdTable) != 0 )){
	  if(err == 1) dprintf(err_fd, "%s: ERROR: I/O Redirection failed\n", argv[0]);
	  else dprintf(err_fd, "%s: ERROR: Unknown error occured\n", argv[0]);
	  _exit(1);
	}
	execvp(cmd->processName, cmd->arguments);
	_exit(2);
      }else{
	cmd->processID = p;
      }
      break;
    }
    case SIMPSH_O_CLOSE:
      break;
    case SIMPSH_O_WAIT:
      waitFlag = 1;
      break;
    case '?':
      if(argv[optind-1][0] == '-' && argv[optind-1][1] != '-') break;
      fprintf(stderr, "%s: ERROR: Unrecognized option \'%s\'...\n", argv[0], argv[optind-1]);
      maxError = 1;
      break;
    }
  }

  if(waitFlag){
    while(1){
      int status;
      pid_t finished = wait(&status);
      if(finished == -1){
	if(errno == ECHILD) break;
	else fprintf(stderr, "%s: ERROR. An error occured while waiting for child processes to finish\n\t%s", argv[0], strerror(errno));
      }else if(finished != 0){
	char exitString[BUF_SIZE];
	exitString[0] = 0;
	status = WEXITSTATUS(status);
	fprintf(stdout, "exit %d %s\n", status, getCommandExitString(cmds, finished, exitString));
	maxError = status > maxError ? status : maxError;
	exitString[0] = 0;
      }
    }
  }

  // Free memory
  SIMPSH_COMMANDS_T_DESTROY(cmds);
  SIMPSH_FILETABLE_T_DESTROY(ft);
  
  exit(maxError);
}
