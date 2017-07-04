/*
 * my_shell.h
 *
 *  Created on: Apr 27, 2017
 *      Author: eranPeled
 */

#ifndef MY_SHELL_H_
#define MY_SHELL_H_


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//
// Created by Eran Peled on 4/25/17.
// 
//
void refreshShell(char *command, char **parameters, int argSize){
	printf("\n");
	free(command); //free the memory occupied by the allocated by string
	for(int j = 0; j< argSize ;j++)free(parameters[j]);//clear the parameters array(is on the heap!)

	sleep(1);
}

extern char* getCurrentDir();

// allocates new string, contains one line from standard input
extern char * getCommand();

// allocates new array, contains parameters as allocated strings. also update "getSize" to number of parameters
extern char **getAllParameters(const char *command, int *size);

// creates new process using fork, then execute
extern void execute(char ** parameters, int size);

//checks if the args contains an cd command
extern int is_cd_command(char **parameters);

//execute the special command "ls"
extern void execute_cd_command(char *command);

#endif /* MY_SHELL_H_ */
