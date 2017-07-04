

//
// Created by Eran Peled on 4/25/17.
// 
//

#include "my_shell.h"
int argSize = 0;

int main(){
    printf("Welcome to my Custom Made Shell\n");
    while(1) {

    	printf("%s:@ ", getCurrentDir());//print my prompt sign and current directory
    	fflush(0); // prompt. (clean buffer and force print)

        char *command = getCommand(); //read a single line from user (as requested)

        char **parameters = getAllParameters(command,&argSize); //parse parameters and count the number of arguments

        //**check if cd command is entered**/
        if(is_cd_command(parameters))
        {
        	execute_cd_command(command);
        	refreshShell(command,parameters,argSize);
        	continue;
        }
        /***********************************/
        execute(parameters, argSize); //execute the user prompt
    	refreshShell(command,parameters,argSize);


    }
    return 0;

}
