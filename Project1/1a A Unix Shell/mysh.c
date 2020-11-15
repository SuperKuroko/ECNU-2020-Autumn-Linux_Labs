#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
#include <sys/stat.h>    
#include <fcntl.h>

char prompt[10] = "mysh> ";
char input[1000];
char *p;
char *shell[512],*file[512];
char *sep[2];
int sep_cnt,shell_cnt,file_cnt;

void output_error();
int count(char *s,int len, char c); //count times of c in s
int exe_built_in_commands();
void exe_command_with_child_process(int isFile,char* FileName);

int main(int argc,char* argv[])
{
	int batch_mode = 0; 
	FILE *fp;
	if(argc > 2)
	{
		output_error();
		exit(1);
	}	
	else if(argc == 2)
	{
		fp = fopen(argv[1],"r");
		if(fp == NULL)
		{
			output_error();
			exit(1);
		}
		else batch_mode = 1;
	}
	while(1)
	{
		file_cnt = shell_cnt = sep_cnt = 0;
		if(batch_mode)
		{
			if(fgets(input,1000,fp) == NULL)
			{
				return 0;
			}
			write(1,input,strlen(input));
		}
		else
		{
			write(1,prompt,strlen(prompt));
			fgets(input,1000,stdin); 
		}
		input[strlen(input)-1] = '\0'; //remove the '\n' at the end of string
		if(strlen(input) == 0) continue;
		if(strlen(input) > 512) //the input length exceeds the limit
			output_error();
		else
		{
			int cnt = count(input,strlen(input),'>');
			if(cnt > 1) output_error();
			else if(cnt == 1)
			{
				p = strtok(input,">");
				while(p != NULL)
				{
				    sep[sep_cnt++]=p;
				    p = strtok(NULL, ">");
				}
				if(sep_cnt != 2) output_error();
				else 
				{
					p = strtok(sep[0]," ");
					while(p != NULL)
					{
						shell[shell_cnt++]=p;
						p = strtok(NULL," ");
					}
					shell[shell_cnt] = NULL;
					p = strtok(sep[1]," ");
					while(p != NULL)
					{
						file[file_cnt++]=p;
						p = strtok(NULL," ");
					}
					if(file_cnt > 2)
					{
						output_error();
						continue;
					}
					else if(file_cnt == 2)
					{
						if(strcmp(file[1],"&") != 0) 
						{
							output_error();
							continue;
						}
						else 
						{
							shell[shell_cnt++] = "&";
							shell[shell_cnt] = NULL;
						}
					}	
					exe_command_with_child_process(1,file[0]);
				}
				
			}
			else if(cnt == 0)
			{
				p = strtok(input," ");
				while(p != NULL)
				{
					shell[shell_cnt++] = p;
					p = strtok(NULL," ");
				}
				if(shell_cnt == 0) continue;
				shell[shell_cnt] = NULL;
				if(exe_built_in_commands() == -1)
					exe_command_with_child_process(0,"");
			}
		}
	}
	return 0;
}

int count(char *s,int len, char c) //count times of c in s
{
	int result = 0,i;
	for(i = 0;i < len;i++)
	{
		if(s[i] == c) 
			result++;
	}
	return result;
}
void output_error()
{
	char error_message[30] = "An error has occurred\n";
	write(STDERR_FILENO,error_message,strlen(error_message));
}
int exe_built_in_commands()
{
	if(shell_cnt == 1)
	{
		if(strcmp(shell[0],"exit") == 0) exit(0);
		else if(strcmp(shell[0],"cd") == 0) 
		{
			chdir(getenv("HOME"));
			return 1;
		}
		else if(strcmp(shell[0],"pwd") == 0) 
		{
			char path[1000];
			if(getcwd(path,1000) == NULL)
			{
				output_error();
				return 1;
			}
			int len = strlen(path);
			path[len]='\n';
			path[len+1] = '\0';
			write(STDOUT_FILENO,path,strlen(path));
			return 1;
		}
		else if(strcmp(shell[0],"wait") == 0)
		{
			wait(NULL);
			return 1;
		}
			
	}
	else if(shell_cnt > 1)
	{
		if(strcmp(shell[0],"cd") == 0 && shell_cnt == 2)
		{
			if(chdir(shell[1]) == -1)
			{
				output_error();
			}
		}
		else if(strcmp(shell[0],"pwd") == 0)
			output_error();
		else if(strcmp(shell[0],"exit") == 0)
			output_error();
		else if(strcmp(shell[0],"wait") == 0)
			output_error();
		return 1;
	}
	return -1;
}
void exe_command_with_child_process(int isFile,char *FileName)
{
	int isBackground_Job = 0;
	if(shell_cnt > 1 && strcmp(shell[shell_cnt-1],"&") == 0)
	{
		shell[shell_cnt-1] = NULL;
		shell_cnt--;
		isBackground_Job = 1;
	}
	int rc = fork();
    if (rc < 0)
    {
        output_error();
        exit(1);
    }
    else if (rc == 0)
    {
    	if(isFile == 1)
    	{
    		close(STDOUT_FILENO);
    		if(open(FileName,O_CREAT|O_WRONLY|O_TRUNC,S_IRWXU) == -1)
    		{
    			output_error();
    		}
    	}
        if (execvp(shell[0],shell) == -1)
        {
            output_error();
            exit(1);
        }
    }
    else
    {
        if(!isBackground_Job) waitpid(rc,NULL,0);
    }
}
