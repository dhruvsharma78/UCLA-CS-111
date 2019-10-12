// NAME: Dhruv Sharma
// EMAIL: dhruvsharma78@ucla.edu
// ID: 605082988

#ifndef OPTIONPARSING_H
#define OPTIONPARSING_H

#include <getopt.h>

// Bit field manipulation
#define SET_BIT(field, bit) field |= (1 << bit)
#define CLEAR_BIT(field, bit) field &= ~(1 << bit)
#define TOGGLE_BIT(field, bit) field ^= (1 << bit)
#define IS_BIT_SET(field, bit) (field & (1 << bit))∑

// File opening modes
#define SIMPSH_O_RD_ONLY 31
#define SIMPSH_O_RD_WR 32
#define SIMPSH_O_WR_ONLY 33
#define SIMPSH_O_PIPE 34

// File opening flags
#define SIMPSH_O_APPEND 51
#define SIMPSH_O_CLOEXEC 52
#define SIMPSH_O_CREAT 53
#define SIMPSH_O_DIRECTORY 54
#define SIMPSH_O_DSYNC 55
#define SIMPSH_O_EXCL 56
#define SIMPSH_O_NOFOLLOW 57
#define SIMPSH_O_NONBLOCK 58
#define SIMPSH_O_RSYNC 59
#define SIMPSH_O_SYNC 60
#define SIMPSH_O_TRUNC 61

// Subcommand Options
#define SIMPSH_O_COMMAND 98
#define SIMPSH_O_WAIT 99
  
// Miscellaneous options
#define SIMPSH_O_CLOSE 70
#define SIMPSH_O_VERBOSE 71
#define SIMPSH_O_PROFILE 72
#define SIMPSH_O_ABORT 73
#define SIMPSH_O_CATCH 74
#define SIMPSH_O_IGNORE 75
#define SIMPSH_O_DEFAULT 76
#define SIMPSH_O_PAUSE 77

extern struct option simpsh_long_options[];

void updateFlags(int* field, int code);
void clearFlags(int* field);

#endif
