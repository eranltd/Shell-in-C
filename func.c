//
// Created by Eran Peled on 4/25/17.
// 
//


#include "func.h"



//get a single line from user (command)
char *getCommand() {

	char command[200] = "";
		if(read(1,command, 200) == -1)
			{perror("read fail"); exit(1);}
		command[strlen(command)] = '\0';
		return (char*)strdup(command);
}

//*get the currrent path*/
	char* getCurrentDir() {
	char cur_path[255];
	if (getcwd(cur_path, sizeof(cur_path)) != NULL) {
		return (char*)strdup(cur_path); //return new allocated string on heap
	}

	return "";
}

	//remove an element from a heap allocated array
char* remove_element(char* array, int sizeOfArray, int indexToRemove)
	{
	    char* temp = malloc((sizeOfArray - 1) * sizeof(char*)); // allocate an array with a size 1 less than the current one

	    if (indexToRemove != 0)
	        memcpy(temp, array, (indexToRemove - 1) * sizeof(char*)); // copy everything BEFORE the index

	    if (indexToRemove != (sizeOfArray - 1))
	        memcpy(temp+indexToRemove, array+indexToRemove+1, (sizeOfArray - indexToRemove - 1) * sizeof(char*)); // copy everything AFTER the index

	    free (array);
	    return temp;
	}

//trim spaces at the end of string
char *trimwhitespace(char *str)
{
  char *end;

  // Trim leading space
  while(isspace((unsigned char)*str)) str++;

  if(*str == 0)  // All spaces?
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace((unsigned char)*end)) end--;

  // Write new null terminator
  *(end+1) = 0;

  return str;
}

//parse the command(user single-line input) into array of args
char **getAllParameters(const char *command, int *size) {
    int count = 0;
    for(int i=0; i<strlen(command); i++)
        if(isspace(command[i])) count++; // count parameters
    char **parameters = (char**)malloc(count*sizeof(char*));

    int i=0, k=0;
    while(k < count) { // more parameters
        char token[255] = "";
        for(int j=0; !isspace(command[i]) && i < strlen(command); i++) // get tokens
            token[j++] = command[i];
        token[strlen(token)] = '\0'; // end of token
        i++; // skip space
        parameters[k++] = (char*)strdup(token); // add
    }
    *size = count;
    return parameters;
}

void execute(char ** parameters, int size) {
    bool specialCase = false; //in case of & sign is entered by the user
    //printf("The Current number of args is : %d\n",size);


    bool is_inputFile = 0, is_outputFile = 0;
    pid_t pid;
    int fd;


    char input_file[100];
    char output_file[100];

    //iter over args and markup
        for(int i=1; i<size; i++) {
        	if(!strcmp(parameters[i], "&"))
        		{
        		specialCase=true;
        		}
            if(!strcmp(parameters[i], "<")){
            	is_inputFile = true;
            	strcpy(input_file,parameters[i+1]);
            	trimwhitespace(input_file);
            }
            if(!strcmp(parameters[i], ">"))
            	{
            	is_outputFile = true;
            	strcpy(output_file,parameters[i+1]);
            	trimwhitespace(output_file);
            	}

        }
        //printf("current map : inputFile=%d,outputFile=%d,specialCase=%d",is_inputFile,is_outputFile,specialCase);


    if((pid = fork()) == -1){
    	perror("fork fail");
    	exit(1);
    }

    if(pid == 0) {
        // son
    	if (is_outputFile) { //handle file output redirection
    					fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    					dup2(fd, STDOUT_FILENO);

    					if (fd == -1) {
    						perror("opening output file failed\n");
    						exit(255);
    					}
    				}

    	if (is_inputFile) { //handle file input redirection
    					fd = open(input_file, O_RDONLY | S_IRUSR | S_IWUSR);
    					dup2(fd, STDIN_FILENO);
    					if (fd == -1) {
    						perror("opening input file failed\n");
    						exit(255);
    					}


    				}

    	if(specialCase)
    	{
    				//clear & from program args
    				int len = strlen(*parameters);
    				remove_element(*parameters,len,len);
    	}

    	else
    	{
    		if(is_inputFile || is_outputFile)
    		{
    			if(execlp(parameters[0],parameters[0], (char*) NULL))
    			    					  { //execute the command
    			    					  perror("failed to execute command execl\n");
    			    					  exit(1);
    			    					  }

    			    					is_inputFile = false;
    			    					is_outputFile = false;

    		}
    		else
    		{


    		if (execvp(parameters[0], parameters) < 0) { //execute the command
    		    		perror("failed to execute command execvp\n");
    		    		exit(1);
    		    		}
    		}

    		    				/*wait for child to terminate*/
    		    				waitpid(pid, NULL, 0);

    	}


  				    			}
    else {
        //father
    	if(specialCase){
    		//print pid of son and return
    		printf("\n[1] %d\n",pid); //[1] 7227
    		specialCase=false;
    		return; //return immediately to prompt
    	}

        int status;
        pid = wait(&status);
        if(status<0) // error
        {printf("exit: %d", WEXITSTATUS(status)); exit(1);}

    }

}

//checks if the args contains an cd command
bool is_cd_command(char **parameters){
	if(!strcmp(parameters[0],"cd")){
		return true;
	}
	return false;
}

//execute the special command "ls"
void execute_cd_command(char *command)
{
		command[strlen(command)-1] = '\0'; // end of token//eliminate '\n' from input command at the last place
		char path[BUFFER_SIZE];
		char cwd[BUFFER_SIZE];
		char *newLine;
	    strcpy(path,command);


	    newLine = strchr(path,' ');

	    if(newLine!=NULL)
	    {
	    	++newLine; //get rid of ' '
	    	strcpy(path,newLine);
	    }


	    if (newLine == NULL) {
	    		chdir(getenv("HOME")); //if the string only contains 'cd' direct to home folder
	    		return;
	    	}

	    if(path[0] != '/')
	    {
	        getcwd(cwd,sizeof(cwd));
	        strcat(cwd,"/");
	        strcat(cwd,path); //concat working directory path to the string and than trigger chdir
		    printf("\n%s\n",cwd);
	        if(chdir(cwd)==-1){
	    		perror("chdir");
	        }
	    }else{
	    	if(chdir(path)==-1){
	    		perror("chdir");
	    	}
	    }
}



