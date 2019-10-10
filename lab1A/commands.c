#include "command.h"
#include "commands.h"
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>

simpsh_commands_t* SIMPSH_COMMANDS_T_INIT(){
  simpsh_commands_t* x = (simpsh_commands_t*) malloc(sizeof(simpsh_commands_t));
  x->num = 0;
  x->commandList = (simpsh_command_t**) malloc(sizeof(simpsh_command_t*));
  return x;
}

void SIMPSH_COMMANDS_T_DESTROY(simpsh_commands_t* cmds){
  int i;
  for(i=0; i<cmds->num; ++i){
    SIMPSH_COMMAND_T_DESTROY(cmds->commandList[i]);
  }
  free(cmds->commandList);
  free(cmds);
  cmds = NULL;
}

void addCommand(simpsh_commands_t* cmds, simpsh_command_t* x){
  cmds->commandList = (simpsh_command_t**) realloc(cmds->commandList, sizeof(simpsh_command_t*) * (cmds->num+2));
  cmds->commandList[cmds->num++] = x;
  cmds->commandList[cmds->num] = NULL;
}

char* getCommandExitString(simpsh_commands_t* cmds, pid_t pid, char* str){
  int i=0;
  for(;i<cmds->num;++i){
    if(cmds->commandList[i]->processID == pid){
      for(int j=0; j<cmds->commandList[i]->numArguments; ++j){
	str = strcat(str, cmds->commandList[i]->arguments[j]);
	str = strcat(str, " ");
      }
      return str;
    }
  }
  return "";
}
