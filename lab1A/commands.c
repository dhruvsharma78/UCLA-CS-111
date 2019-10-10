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

void addCommand(simpsh_commands_t* cmds, simpsh_command_t* x){
  cmds->commandList = (simpsh_command_t**) realloc(cmds->commandList, sizeof(simpsh_command_t**) * (cmds->num+1));
  cmds->commandList[cmds->num++] = x;
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
