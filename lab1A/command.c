// NAME: Dhruv Sharma
// EMAIL: dhruvsharma78@ucla.edu
// ID: 605082988

#include "command.h"
#include "file.h"
#include <stdlib.h>
#include <stdio.h>

simpsh_command_t* SIMPSH_COMMAND_T_INIT(){
  simpsh_command_t* x = (simpsh_command_t*) malloc(sizeof(simpsh_command_t));
  x->fdTable[0] = x->fdTable[1] = x->fdTable[2] = 0;
  x->numArguments = 0;
  x->processName = NULL;
  x->arguments = (char**) malloc(sizeof(char*));
  x->processID = 0;
  return x;
}

void SIMPSH_COMMAND_T_DESTROY(simpsh_command_t* cmd){
  free(cmd->processName);
  free(cmd->arguments);
  free(cmd);
  cmd = NULL;
}

void addArgument(simpsh_command_t* cmd, char* arg){
  char** temp = (char**) realloc(cmd->arguments, sizeof(char*) * (cmd->numArguments+2));
  cmd->arguments = temp;
  cmd->arguments[cmd->numArguments] = arg;
  cmd->numArguments++;
  cmd->arguments[cmd->numArguments] = NULL;
}

