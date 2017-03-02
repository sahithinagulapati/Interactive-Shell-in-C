#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<sys/utsname.h>

#include <signal.h>
#include <fcntl.h>

// This function will handle a signal. 
void HandleSignal(int sig, siginfo_t *si, void *context);

#define _XOPEN_SOURCE 700

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

void p_new(){
	char system_name[30];
	size_t len =30;
	char *user_name = getenv("USER"); // user_name
	char *cwd = getcwd(0,0);  ///
	gethostname(system_name,len); // system_name
	printf("<%s@%s:%s>",user_name,system_name,cwd);
}

int check(char *command){
	//int len = strlen(command);
	//printf("%d\n", len);
	if(strlen(command) == 5){
		return 0;
	}
	else{
		return 1;
	}
}


void printCommands(char **commands, int t) {
    int i;
    for(i = 0; i < t; i++) {
        printf("%s\n", commands[i]);
    }
}

void main(){

	struct sigaction sVal;
   	pid_t myPID;
   	pid_t myG_PID;

   	// Specify that we will use a signal handler that takes three arguments
   	// instead of one, which is the default.
   	sVal.sa_flags = SA_SIGINFO;

   	// Indicate which function is the signal handler.
   	sVal.sa_sigaction = HandleSignal;

	int t=0,k,q=0;
	int fd[2];
	int fd2[2];
	char error[1000];
	char cmd_names[100][100];
    char cmd_names2[100][100];
	int a;
	int isBG = 0;
			 // Register for SIGINT
      			sigaction(SIGINT, &sVal, NULL);
      			// Register for SIGCHLD 
      			sigaction(SIGCHLD, &sVal, NULL);

      			myPID = getpid();
      			myG_PID = getpgid(myPID);
      			//printf("\nParent: My process id = %d.\n", myPID);
      			//printf("Parent: My process group id = %d.\n", myG_PID);
	while(1){
		p_new();
	    	int m;
		size_t size = 10;
		char curr_dir[100];
		char cmd[20];
		char command[30];
		char prevcommand[30];
		char *arg[20] = {};
		int i=1;
		char *dm =" ";
		char *p;
		int res;
		char *cd = "cd";
		char *gedit = "gedit";
		char *ls = "ls";
		int pid;
		char *and = "&";
		const char target[1024];
		char directory[1024];
		char dir_name[100][100];
		int result;
		char *argv[3];
		int pid_arr[100];
		i = 1;

		strcpy(cmd, ""); // initializing cmd
		gets(cmd); // stores command read from the standard input in cmd array
		strcpy(prevcommand, cmd);
		p = strtok(cmd,dm);
		arg[0] = p; 			
		while(((p=strtok(NULL," "))!=NULL)){  // to store each word of the given command
			arg[i]=p;
			i++;
		} 
		
                // fd is an array to indicate file descriptor 
		// fd[0] is set up for reading
		// fd[1] is set up for writing
		int test = pipe(fd); // declaring pipe
		if(strstr(cmd, "&") >= 0) {   // needle in haystack
			// has to be child
		}
		else {
		}

		pid = fork(); // forking 
		if(pid == 0){	// child	
			close(fd[0]); // read end of the pipe is closed
			//printf("%s sending\n", arg[0]);
			int val1 = write(fd[1],arg[0],strlen(arg[0])+1); // arg[0] is written into the pipe
			strcpy(cmd_names2[t],arg[0]);
			strcpy(command,cmd_names2[t]);
			strcpy(cmd_names[t], command);  // t is the length of the array cmd_names[]
			//strcpy(prevcommand,command);
			if((command[0]=='h') && (command[1]=='i') && (command[2]=='s') && (command[3]=='t')){
				k = check(command);  // to get the length of the command (hist or histn)
				if(k != 0){  // hist
					if((res = strcmp(cmd_names[t],"hist")) == 0){
						for(m=0;m<=t;m++){
							printf("%s\n",cmd_names[m]);
						}
					
					}
				}
				else if(k == 0){   // histn
					q=command[4]-'0';
					for(m=t;m>t-q;m--){
						//printf("%d",q);
						printf("%s\n",cmd_names[m]);
					}
				}	
				
			}
			else if( (strstr(prevcommand, "<") != NULL) || (strstr(prevcommand, ">") != NULL) ) {
				// redirection commands
				int gt = -1;
				int lt = -1;
				char *gtchr;
				char *ltchr;
				char actcmd[30];
				char redFile[30];
				memset(actcmd, ' ', 30);
				memset(redFile, ' ', 30);
				gtchr = strchr(prevcommand, '>');
				if(gtchr) {
					gt = gtchr - prevcommand;
					printf("%d\n", gt);
				}
				ltchr = strchr(prevcommand, '<');
				if(ltchr) {
					lt = ltchr - prevcommand;
					printf("%d\n", lt);
				}
				if(gt == -1 && lt > 0) {
					//int rem = strlen(prevcommand) - lt;
					strncpy(actcmd, prevcommand, lt);
					char *rlcmd = trimwhitespace(actcmd);
					char *outname = trimwhitespace(ltchr+1);
					printf("Contains only lt -> {%s}\n, {%s}\n", actcmd, outname);
				}
				else if(gt > 0 && lt == -1) {
					strncpy(actcmd, prevcommand, gt);
					char *rlcmd = trimwhitespace(actcmd);
					char *outname = trimwhitespace(gtchr+1);
					printf("Contains only gt -> {%s}, {%s}\n", rlcmd, outname);
					// Child process
					int npid;
					npid = fork();
					close(2);
					if (npid == 0) {
						open(outname, O_WRONLY);
						execvp(rlcmd, rlcmd); // By default, the program writes to stdout (fd no - 1). ie, in this case, the file
					}
					else if(npid == -1) {
					}
					else {g
					}
				}
				else {
					if(lt > gt) {
						printf("Error\n");
					}
					else {
					}
					printf("Contains both\n");
				}
			}
			else if((res = strcmp(arg[0],"cd"))==0){
				chdir(arg[1]);
			}
			else if(strstr(prevcommand, "&") == NULL) {
				int ret;
				ret = execvp( arg[0], arg );
				if(ret == -1) {
					perror(error);
				}
			}
			else{
				isBG = 1;
				printf("Run in background\n");
				int newpid;
				newpid = fork(); 
				if(newpid == 0){
					execvp( arg[0], arg );
				}
				else if(pid == -1){
				}
				else{
				}
			}
		}
		else if(pid == -1){
			printf("Fork Failed");
		}
		else{
			close(fd[1]);
			int val = read(fd[0],cmd_names[t],100);
			strcpy(command,cmd_names[t]);
			if(strstr(command, "&") != NULL)
				isBG = 1;	
		}
		t++;
		if( !isBG)	{
			wait(NULL);
		}
		isBG = 0;
		p_new();
	}

}

void HandleSignal(int sig, siginfo_t *si, void *context)
{
   switch(sig)
   {
      case SIGINT:
         //printf("\nControl-C was pressed: mypid = %d, mypgid = %d\n", getpid(), getpgid(getpid()));
         _exit(0);
         break;
      case SIGCHLD:
         //printf("\nSIGCHLD. mypid = %d, mypgid = %d\n",getpid(), getpgid(getpid()));
         if(si->si_code == CLD_EXITED || si->si_code == CLD_KILLED)
         {
            printf("   Process %d is done!\n", si->si_pid);
         }
         break;
   }
}

