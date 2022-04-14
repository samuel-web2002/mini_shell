#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include <fcntl.h>

#define MAX_SIZE 1024
#define MAX_WORDS 64

void free_memory(char** parsed_cmd, char* cmd, char* temp1)    //this is a void function. this function is to free the allocated memory.
{

	for(int i=0;i<MAX_WORDS;i++)                           // this for loop is used to clear the memory that allocated to each parsed_cmd[i].
	{
		if(parsed_cmd[i] != NULL)
		{
			free(parsed_cmd[i]);
		}
	}
	free(parsed_cmd);
	free(cmd);
	free(temp1);
}

void change_last_char_to_null_char(char* s) 
{
	for(int i=0;i<MAX_SIZE;i++)
	{
		if(s[i] == '\n')
		{
			s[i] = '\0';
			break;
		}
	}
}

void change_last_char_to_new_line(char* s)
{
	for(int i=0;i<MAX_SIZE;i++)
	{
		if(s[i] == '\0')
		{
			s[i] = '\n';
			break;
		}
	}
}

int check(char** parsed_cmd, int len, char* internal_cmd)                    //this is the check functio and this functions the checks the entered internal command is the internal command mentioned 
                                                                            //in the function entries or not.
{
	int b = 0;
	for(int i=0;i<len;i++)
	{
		if(strcmp(parsed_cmd[i], internal_cmd) == 0)
		{
			b = 1;
			return b;
		}
	}
	return b;
}

void external_linux_commands(char** parsed_cmd)
{
	pid_t pid = fork();
	if(pid < 0)
	{
		printf("%s\n", "ERROR :- failed to fork a child");
		exit(1);
	}
	else if(pid == 0)
	{
		if(execvp(parsed_cmd[0], parsed_cmd) < 0)
		{
			printf("%s\n", "ERROR :- failed to execute the command");
			exit(1);
		}
		exit(0);
	}
	else
	{
		wait(NULL);
	}
}

void handling_pipe(char** parsed_cmd, char** after_pipe)                 //this is single pipe handling.
{                                                                   //A pipe is a form of redirection. That is used to send the output of one command to 
	int stat;                                           
	pid_t p1, p2;
	int pipefd[2];                                   
	int std_out_dup = dup(1);
	if(pipe(pipefd) < 0)
	{
		printf("%s\n", "ERROR :- Not able to initialise the pipe");
		exit(1);
	}
	p1 = fork();
	if(p1 < 0)
	{
		printf("%s\n", "ERROR :- Not able to fork first child");
		exit(1);
	}
	else if(p1 == 0)
	{
		dup2(pipefd[1], 1);
		int ret = execvp(parsed_cmd[0], parsed_cmd);
		fflush(stdout);
		if(ret < 0)
		{
			printf("%s\n", "ERROR :- couldn't execute the command before pipe");
			exit(1);
		}
		exit(0);

	}
	else
	{
		p2 = fork();
		if(p2 < 0)
		{
			printf("%s\n", "ERROR :- Not able to fork second child");
			exit(1);
		}
		else if(p2 == 0)
		{
			dup2(std_out_dup,1);
			close(pipefd[1]);
			dup2(pipefd[0], 0);
			int ret = execvp(after_pipe[0], after_pipe);
			if(ret < 0)
			{
				printf("%s\n", "ERROR :- couldn't execute the command after pipe");
				exit(1);
			}
			exit(0);	
		}
		else
		{
			close(pipefd[0]);
			close(pipefd[1]);
			dup2(std_out_dup, 1);
			waitpid(p1, &stat, 0);
			waitpid(p2, &stat, 0);
		}
	}

}






int main()
{
	char* user;
	char* home;
	char* shell;
	char* term;
	user = (char*)malloc(MAX_SIZE * sizeof(char));     //this allocates memory for this environmental variable user
	home = (char*)malloc(MAX_SIZE * sizeof(char));     //this allocates memory for this environmental variable home
	shell = (char*)malloc(MAX_SIZE * sizeof(char));    //this allocates memory for this environmental variable shell
	term = (char*)malloc(MAX_SIZE * sizeof(char));     //this allocates memory for this environmental variable term
	/*strcpy(user, "samuel");                            //this copies the value for the environmental variable user
	strcpy(home, "/home/samuel");                      //this copies the value for the environmental variable home
	strcpy(shell, "bash");                             //this copies the value for the environmental variable shell
	strcpy(term, "ubuntu"); */                           //this copies the value for the environmental variable term.

	while(1)
	{
        //declaring variables
        char* cmd;                            // these are pointers used in the following code, for creating history file ,reading history file and for checking.
		FILE* hist;                    // these are pointers used in the following code, for creating history file ,reading history file and for checking.
		FILE* read_hist;               // these are pointers used in the following code, for creating history file ,reading history file and for checking.
		char* temp;
		char* temp1;                   // these are pointers used in the following code, for creating history file ,reading history file and for checking.
		char** parsed_cmd;             // these are pointers used in the following code, for creating history file ,reading history file and for checking.
		char cwd[MAX_SIZE];
		

        //print current working directory and waiting for a command....
		getcwd(cwd, sizeof(cwd));
		printf("%s$ ", cwd);
		cmd = (char*)malloc(MAX_SIZE * sizeof(char));
		temp1 = (char*)malloc(MAX_SIZE * sizeof(char));
		fgets(cmd, MAX_SIZE, stdin);
		change_last_char_to_null_char(cmd);




		//parsing the cmd and storing input to history file
		parsed_cmd = (char**)malloc(MAX_WORDS * sizeof(char*));        //this creates some space and allocates for parsed_cmd
		for(int i=0;i<MAX_WORDS;i++)
		{
			parsed_cmd[i] = (char*)malloc(MAX_SIZE * sizeof(char));      //this creates some space and allocates for parsed_cmd[i].
		}
		int len = 0;
		strcpy(temp1, cmd);
		temp = temp1;
		for(int i=0;i<MAX_WORDS;i++)
		{
		    char* sep = strsep(&temp, " ");
			if(sep == NULL)
			{
				break;
			}
			if(strlen(sep) == 0)
			{
				i--;
			}
			else
			{
				strcpy(parsed_cmd[i], sep);
				len++;
			}
		}
		parsed_cmd[len] = NULL;
		if(len != 0)
		{
			change_last_char_to_new_line(cmd);
			hist = fopen("/tmp/history.txt", "a+");             //creating a file for history iin a+ mode using hist pointer.
			fputs(cmd, hist);
			fclose(hist);
			change_last_char_to_null_char(cmd);
		}



        //handling help
        if(check(parsed_cmd, len, "help") == 1)
        {
        	printf("%s\n", "cd\n");
        	printf("%s\n", "echo\n");
        	printf("%s\n", "exit\n");
        	printf("%s\n", "help\n");
        	printf("%s\n", "history\n");
        	printf("%s\n", "pwd\n");
        	
        	// print all the internal commands
        	free_memory(parsed_cmd, cmd, temp1);
        	continue;	
        }
        //handling echo for the variables
        if(check(parsed_cmd, len, "echo") == 1)
        {
        	if((parsed_cmd[1][0])=='$')
        	{
        		const char* m=(parsed_cmd[1]+1);
        		const char* n=getenv(m);
        		if(n!=NULL)
        			printf("%s\n",n);
        		else
        			perror("Error: ");
        	}
        	else
        	{
        		printf("%s\n",parsed_cmd[1]);
        	}
        	continue;
        }


        //handling environment variables            
        if(check(parsed_cmd, len, "setenv") == 1)                  //function defined to set the desired value for the environmental variable.
        {                                                          //setenv should be included in the command and spaces before and after "=" are must for execution.
        	if(strcmp(parsed_cmd[1], "USER") == 0)             //if the required spaces are not given then it prints segmentation error.
        	{
        		strcpy(user, parsed_cmd[3]);
        	}
        	else if(strcmp(parsed_cmd[1], "HOME") == 0)
        	{
        		strcpy(home, parsed_cmd[3]);
        	}
        	else if(strcmp(parsed_cmd[1], "SHELL") == 0)
        	{
        		strcpy(shell, parsed_cmd[3]);
        	}
        	else
        	{
        		strcpy(term, parsed_cmd[3]);
        	}
        	free_memory(parsed_cmd, cmd, temp1);
        	continue;
        }

        /*if(check(parsed_cmd, len, "printenv") == 1)            //function defined to print the value of environmental variable.
        {                                                      //printenv should be included in the command and spaces before and after "=" are must for execution.
        	if(len == 1)
        	{                                               //if the required spaces are not given then it prints segmentation error.
        		printf("USER = %s\n", user);
        		printf("HOME = %s\n", home);
        		printf("SHELL = %s\n", shell);
        		printf("TERM = %s\n", term);
        	}
        	else if(strcmp(parsed_cmd[1], "USER") == 0)
        	{
        		printf("%s\n", user);
        	}
        	else if(strcmp(parsed_cmd[1], "HOME") == 0)
        	{
        		printf("%s\n", home);
        	}
        	else if(strcmp(parsed_cmd[1], "SHELL") == 0)
        	{
        		printf("%s\n", shell);
        	}
        	else
        	{
        		printf("%s\n", term);
        	}
        	free_memory(parsed_cmd, cmd, temp1);
        	continue;
        }*/

		//handling cd
		if(check(parsed_cmd, len, "cd") == 1)
		{
			if(len == 1)
			{
				chdir("/home/samuel");
			}
			else
			{
				chdir(parsed_cmd[1]);
			}
			free_memory(parsed_cmd, cmd, temp1);
			continue;
		}




        //handling history
		if(check(parsed_cmd, len, "history") == 1)
		{
			read_hist = fopen("/tmp/history.txt", "r");
			while(1)
			{
				char c = fgetc(read_hist);
				if(c == EOF)
				{
					break;
				}
				printf("%c", c);
			}
			fclose(read_hist);
			free_memory(parsed_cmd, cmd, temp1);
			continue;
		}




		//handling exit
		if(check(parsed_cmd, len, "exit") == 1 || check(parsed_cmd, len, "x") == 1 || check(parsed_cmd, len, "quit") == 1)
		{
			free_memory(parsed_cmd, cmd, temp1);
			remove("/tmp/history.txt");
			break;
		}

                                                             //A pipe is a form of redirection. That is used to send the output of one command to
		//handling pipe
		if(check(parsed_cmd, len, "|") == 1)            //"|" is a metacharector.
		{                                                //this is single pipe handling.       
			int i = 0;                               //A pipe is a form of redirection. That is used to send the output of one command to another command for further processing.
			while(i < len)
			{                                    //and both commands are sepersted by "|" Here i impemented single piping.here i implemented piping such that it requires space before and
				if(strcmp(parsed_cmd[i], "|") == 0)        //after pipe to execute the command else it gives error "no such file/directory exists".
				{
					break;
				}
				i++;
			}
			int l = i;
			i++;
			char** after_pipe;
			after_pipe = (char**)malloc(MAX_WORDS * sizeof(char*));
			for(int j=0;j<MAX_WORDS;j++)
			{
				after_pipe[j] = (char*)malloc(MAX_SIZE * sizeof(char));
			}
			int k = 0;
			while(i < len)
			{
				strcpy(after_pipe[k], parsed_cmd[i]);
				k++;
				i++;
			}
			after_pipe[k] = NULL;
			parsed_cmd[l] = NULL;
			handling_pipe(parsed_cmd, after_pipe);
			free_memory(parsed_cmd, cmd, temp1);
			for(int j=0;j<MAX_WORDS;j++)
			{
				if(after_pipe[j] != NULL)
				{
					free(after_pipe[j]);
				}
			}
			free(after_pipe);
			continue;
		}



		//handling input output redirection
		if(check(parsed_cmd, len, ">") == 1)          //implemented input and output redirection ">","<", and append ">>".
		{                                             //here it requires spaces on both sides for the command to ececute correctly.else it gives error "no such file/directory exists".
			int std_out_dup = dup(1);
			int fd = open(parsed_cmd[len-1], O_WRONLY | O_CREAT, 0666);
			dup2(fd, 1);
			parsed_cmd[len-2] = NULL;
			external_linux_commands(parsed_cmd);
			fflush(stdout);
			close(fd);
			dup2(std_out_dup, 1);
			free_memory(parsed_cmd, cmd, temp1);
			continue;
		}                                   //similar implementation for "<" and ">>" both requires spaces to work else gives the error "no such file/directory exists" in the shell.

		if(check(parsed_cmd, len, "<") == 1)
		{
			int std_in_dup = dup(0);
			int fd = open(parsed_cmd[len-1], O_RDONLY);
			dup2(fd, 0);
			parsed_cmd[len-2] = NULL;
			external_linux_commands(parsed_cmd);
			close(fd);
			dup2(std_in_dup, 0);
			free_memory(parsed_cmd, cmd, temp1);
			continue;
		}

		if(check(parsed_cmd, len, ">>") == 1)
		{
			int std_out_dup = dup(1);
			int fd = open(parsed_cmd[len-1], O_WRONLY | O_APPEND | O_CREAT, 0666);
			dup2(fd, 1);
			parsed_cmd[len-2] = NULL;
			external_linux_commands(parsed_cmd);
			fflush(stdout);
			close(fd);
			dup2(std_out_dup, 1);
			free_memory(parsed_cmd, cmd, temp1);
			continue;
		}


		//handing external linux commands
		external_linux_commands(parsed_cmd);
		free_memory(parsed_cmd, cmd, temp1);
	}
	free(user);
	free(shell);
	free(term);
	free(home);

		
}
