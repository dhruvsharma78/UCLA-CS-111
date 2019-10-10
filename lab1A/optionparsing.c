#include "optionparsing.h"
#include <getopt.h>
#include <fcntl.h>

struct option simpsh_long_options[] = {
    {"rdonly"   , required_argument, 0, SIMPSH_O_RD_ONLY},
    {"rdwr"     , required_argument, 0, SIMPSH_O_RD_WR},
    {"wronly"   , required_argument, 0, SIMPSH_O_WR_ONLY},
    {"pipe"     , no_argument      , 0, SIMPSH_O_PIPE},
    {"append"   , no_argument      , 0, SIMPSH_O_APPEND},
    {"cloexec"  , no_argument      , 0, SIMPSH_O_CLOEXEC},
    {"creat"    , no_argument      , 0, SIMPSH_O_CREAT},
    {"directory", no_argument      , 0, SIMPSH_O_DIRECTORY},
    {"dsync"    , no_argument      , 0, SIMPSH_O_DSYNC},
    {"excl"     , no_argument      , 0, SIMPSH_O_EXCL},
    {"nofollow" , no_argument      , 0, SIMPSH_O_NOFOLLOW},
    {"nonblock" , no_argument      , 0, SIMPSH_O_NONBLOCK},
    {"rsync"    , no_argument      , 0, SIMPSH_O_RSYNC},
    {"sync"     , no_argument      , 0, SIMPSH_O_SYNC},
    {"trunc"    , no_argument      , 0, SIMPSH_O_TRUNC},
    {"command"  , required_argument, 0, SIMPSH_O_COMMAND},
    {"wait"     , no_argument      , 0, SIMPSH_O_WAIT},
    {"close"    , required_argument, 0, SIMPSH_O_CLOSE},
    {"verbose"  , no_argument      , 0, SIMPSH_O_VERBOSE},
    {"profile"  , no_argument      , 0, SIMPSH_O_PROFILE},
    {"abort"    , no_argument      , 0, SIMPSH_O_ABORT},
    {"catch"    , required_argument, 0, SIMPSH_O_CATCH},
    {"ignore"   , required_argument, 0, SIMPSH_O_IGNORE},
    {"default"  , required_argument, 0, SIMPSH_O_DEFAULT},
    {"pause"    , no_argument      , 0, SIMPSH_O_PAUSE},
    {0, 0, 0, 0}
};

void updateFlags(int* field, int code){
  int flag = (code == SIMPSH_O_APPEND)    ? O_APPEND :
             (code == SIMPSH_O_CLOEXEC)   ? O_CLOEXEC :
             (code == SIMPSH_O_CREAT)     ? O_CREAT :
             (code == SIMPSH_O_DIRECTORY) ? O_DIRECTORY :
             (code == SIMPSH_O_DSYNC)     ? O_DSYNC :
             (code == SIMPSH_O_EXCL)      ? O_EXCL :
             (code == SIMPSH_O_NOFOLLOW)  ? O_NOFOLLOW :
             (code == SIMPSH_O_NONBLOCK)  ? O_NONBLOCK :
             (code == SIMPSH_O_RSYNC)     ? O_RSYNC :
             (code == SIMPSH_O_SYNC)      ? O_SYNC :
             (code == SIMPSH_O_TRUNC)     ? O_TRUNC :
             (code == SIMPSH_O_RD_ONLY)   ? O_RDONLY :
             (code == SIMPSH_O_RD_WR)     ? O_RDWR:
             (code == SIMPSH_O_WR_ONLY)   ? O_WRONLY: 0;
  *field |= flag;
}

void clearFlags(int* field){
  *field = 0;
}
