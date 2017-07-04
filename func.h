//
// Created by Eran Peled on 4/25/17.
// 
//

#ifndef EX2_SHELL_H
#define EX2_SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <limits.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <ctype.h>
#include <unistd.h>

typedef enum { false, true } bool;
#define BUFFER_SIZE 255
char* getCurrentDir();

// allocates new string, contains one line from standard input
char * getCommand();

// allocates new array, contains parameters as allocated strings. also update "getSize" to number of parameters
char **getAllParameters(const char *command, int *size);


// creates new process using fork, then execute
void execute(char ** parameters, int size);

//checks if the args contains an cd command
bool is_cd_command(char **parameters);

//execute the special command "ls"
void execute_cd_command(char *command);


#endif //EX2_SHELL_H
