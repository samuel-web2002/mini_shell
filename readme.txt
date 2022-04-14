Name             : B.SAMUEL SUNNY KUMAR
Roll Number      : 200101023
language used    : c
operating system : works in linux.(not in windows).
compailing code  : gcc 200101023_Assign02.c
running code     : ./a.out

                                              INTERNAL COMMANDS :- 
                   
cd       : cd desktop
echo     : it is divided into 2 cases
		case 1:- if parsed[1] starts with $ then check                      case 2:- if not case 1 then it just prints whatever present after echo.
			  whether next is environmental variable                                  ex: echo 1345   (prints 1345)
			  or not. if yes then it prints its value
			  if not then it gives "Error: "
			  	 ex: echo $USER  (prints username)
			         echo $110   (prints Error: )

exit || x || quit    : removes the history file and exits from the mini shell
			just enter  "exit" / "x" / "quit".

help      : enter "help" it prints all the internal commands list.

history   : enter "history" it prints all the previously given commands after the last exit.
pwd       : pwd -L: Prints the symbolic path.
            pwd -P: Prints the actual path. 


                                              EXTERNAL COMMANDS :-


man       : man command in Linux is used to display the user manual of any command that we can run on the terminal.
	    It provides a detailed view of the command which includes NAME, SYNOPSIS, DESCRIPTION, OPTIONS, EXIT STATUS,RETURN VALUES, ERRORS, FILES, VERSIONS, EXAMPLES, AUTHORS and SEE ALSO.
	    syntax : $ man [COMMAND NAME]
	          ex: man printf
which     : which command in Linux is a command which is used to locate the executable file associated with the given command by searching it in the path environment variable.
	          ex: which cpp
	           	    	    
SOMEMORE EXTERNAL COMMANDS : chsh, whereis, passwd, date, cal, clear, sleep, shutdown, script, apropos, exit, ls, cat, touch, more, less, cp, mv, rm...etc 
	    
PIPING    : A pipe is a form of redirection. That is used to send the output of one command to another command for further processing. and both commands are sepersted by "|"
	    Here i impemented single piping.here i implemented piping such that it requires space before and after pipe to execute the command else it gives error "no such file/directory exists".
                  ex: ls | wc

REDIRECTION : implemented input and output redirection ">","<", and append ">>".
	       here it requires spaces on both sides for the command to ececute correctly.
	               ex: cat > filename.txt
	       else it gives error "no such file/directory exists".
	       similar implementation for "<" and ">>" both requires spaces to work else gives the error "no such file/directory exists" in the shell.
	                ex: cat filename1.txt >> filename2.txt
	                    cat < filename.txt

ENVIRONMENTAL VARIABLES : USER   = username
			   HOME   = home directory
			   SHELL  = shell type
			   TERM   = operating system.

setenv   : function defined to set the desired value for the environmental variable. setenv should be included in the command and spaces before and after "=" are must for execution.
	            ex: setenv USER = 10 
	   if the required spaces are not given then it prints segmentation error.
	   
printenv : function defined to print the value of environmental variable. printenv should be included in the command and spaces before and after "=" are must for execution.
		    ex: printenv USER
	   if the required spaces are not given then it prints segmentation error.
		    	                     	    
