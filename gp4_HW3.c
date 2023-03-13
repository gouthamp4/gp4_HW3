#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAXSIZE 5000

int cmdopt = 0;
int S = 0;
int s = 0;
char eE = ' ';
char *f1 = NULL;
char *unixcommand =NULL;
char *token[1024] = {NULL};
int f2 = 0;
char *t = NULL;
int position = 0;
char *path = NULL;

struct stat buff;
char *type1 = NULL;
char *type2 = NULL;
long long int size = 0;
char *lasttime = NULL;

int x = 0;
int y = 0;
char array[MAXSIZE];
char array1[MAXSIZE];
char fsize[MAXSIZE];
char listfiles[MAXSIZE];



typedef void FUNCTION(char *df, char *path, int position);
void printRecfiles(char *df, char *path, int position);
void getfileinfo(char *df, char *path, int position);
void getoutput(char *df, char *path, int position);
void execute_unix_command(char *df,char *unixcommand);

void funcptr(char *df, char *path, int position, FUNCTION *func)
{
	func(df, path, position);
}

int main (int argc, char **argv)
{
	while((cmdopt = getopt(argc, argv, "Ss:f:t:Ee")) != -1) 
    {
		switch(cmdopt)
		{ 
            case 'S':
				S = 1;
				break;
            case 's':
				s = atoi(optarg);
				break;
            case 'f':
				f1 = optarg;
				f2 = atoi(argv[optind]);
				optind = optind + 1;
				
                break; 
            case 't':
				t = optarg;
                break;
	    case 'e':
				eE = 'e';
				unixcommand = argv[optind];
				optind = optind + 1;
			        break;
	    case 'E':
				eE = 'E';
				unixcommand = argv[optind];
				optind = optind + 1;
				break;
				
        }
	}
	y = optind;
	
	if (y != argc)
	{
		funcptr(argv[y], path, position, printRecfiles);
	}
	else{
		funcptr(".", path, position, printRecfiles);
	}
	if(eE == 'E')
	{
		printf("unixcommand: %s\n",unixcommand);
		listfiles[strlen(listfiles)-1] = '\0'; 
	        execute_unix_command(listfiles,unixcommand);
	}
	printf("\n");
	
	return 0;
}

void printRecfiles(char *basedir, char *path, int position)
{
	char str[MAXSIZE];
    struct dirent *drp;
	x=0;
    DIR *df = opendir(basedir);

    if (df == NULL)
	{
        return;
	}

    while ((drp = readdir(df)) != NULL)
    {
        if (strcmp(drp->d_name, ".") != 0 && strcmp(drp->d_name, "..") != 0)
        {
            while(x < position)
            {
                if ((x%2==0)||(x == 0))
				{
					printf("	");
				}
                else
				{
					printf("");
				}
				x++;
            }

            strcpy(str, basedir);
            strcat(str, "/");
            strcat(str, drp->d_name);	
			funcptr(drp->d_name,str,position,getoutput);
            funcptr(str, str, position + 2, printRecfiles);
        }
    }
    

    closedir(df);
}

void getfileinfo(char *df, char *path, int position)
{	
	lstat(path, &buff);
	
	switch(buff.st_mode & S_IFMT)
	{
		case S_IFBLK:
			type1 = "Block device";
			break;
		case S_IFCHR:
			type1 = "Character device";
			break;
		case S_IFDIR:
			type1 = "Directory";
			type2 = "d";
			break;
		case S_IFIFO:
			type1 = "Named pipe(FIFO)";
			break;
		case S_IFLNK:
			type1 = "Symbolic link";
			break;
		case S_IFREG:
			type1 = "Regular file";
			type2 = "f";
			break;
		case S_IFSOCK:
			type1 = "UNIX domain socket";
			break;
		default:
			type1 = "Unknown";
	}
	
	(strcmp(type1,"Directory") != 0) ? (size = buff.st_size) : (size = 0);
	
	lasttime = ctime(&buff.st_atime);
	
	(buff.st_mode & S_IRUSR) ? strcat(array,"r") : strcat(array,"-");
	(buff.st_mode & S_IWUSR) ? strcat(array,"w") : strcat(array,"-");
	(buff.st_mode & S_IXUSR) ? strcat(array,"x") : strcat(array,"-");
	(buff.st_mode & S_IRGRP) ? strcat(array,"r") : strcat(array,"-");
	(buff.st_mode & S_IWGRP) ? strcat(array,"w") : strcat(array,"-");
	(buff.st_mode & S_IXGRP) ? strcat(array,"x") : strcat(array,"-");
	(buff.st_mode & S_IROTH) ? strcat(array,"r") : strcat(array,"-");
	(buff.st_mode & S_IWOTH) ? strcat(array,"w") : strcat(array,"-");
	(buff.st_mode & S_IXOTH) ? strcat(array,"x") : strcat(array,"-");
	
	sprintf(array1,"%d",size);
	strcat(array1," bytes");
	strcat(array1,"		");
	strcat(array1,lasttime);
	strcat(array1,"		");
	strcat(array1,array);
}

void getoutput(char *df, char *path, int position)
{
	funcptr(df,path,position,getfileinfo);
	if(S != 0)
	{
		if(s != 0)
		{
			if((f1 != NULL) && (f2 != 0))
			{
				if(t != NULL)
				{
					if(size <= s)
					{
						if(position <= f2)
						{
							if(strstr(df,f1))
							{
								(strcmp(type2,t) == 0) ? printf("%s	%s\n",df,array1) : printf("\n");	//-S -s -f -t
							}
							printf("\n");
						}
						printf("\n");
					}
					printf("\n");
				}
				else
				{
					if(size <= s)
					{
						if(position <= f2)
						{
							if(strstr(df,f1))
							{
								printf("%s	%s",df,array1);		//-S -s -f
							}
							printf("\n");
						}
						printf("\n");
					}
					printf("\n");
				}
			}
			else
			{
				if(size <= s)
				{
					printf("%s	%s",df,array1);		//-S -s
				}
				printf("\n");
			}
		}
		else
		{
			printf("%s	%s",df,array1);		//-S 
			printf("\n");
		}
	}
	else if(s != 0)
	{
		if((f1 != NULL) && (f2 != 0))
		{
			
			if(t != NULL)
			{
				if(size <= s)
				{
					if(position <= f2)
					{
						if(strstr(df,f1))
						{
							(strcmp(type2,t) == 0) ? printf("%s\n",df) : printf("\n");	//-s -f -t
						}
						printf("\n");
					}
					printf("\n");
				}
				printf("\n");
			}
			else
			{
				if(size <= s)
				{
					if(position <= f2)
					{
						if(strstr(df,f1))
						{
							if(eE != ' ' && eE == 'e')
							{
								execute_unix_command(path,unixcommand);
							}
							else if(eE!=' ' && eE == 'E')
							{
								strcat(listfiles,path);
								strcat(listfiles," ");
							}
							else
							printf("%s",df);	//-s -f
						}
						printf("\n");
					}
					printf("\n");
				}
				printf("\n");
			}
		}
		else
		{
			if(size <= s)
			{
				if(eE != ' ' && eE == 'e')
				{
					execute_unix_command(path,unixcommand);
				}
				else if(eE!= ' ' && eE == 'E')
				{
					strcat(listfiles,path);
					strcat(listfiles," ");
				}
				else
				printf("%s",df);	//-s
			}
			printf("\n");
		}
	}
	else if((f1 != NULL) && (f2 != 0))
	{
		
		if(t != NULL)
		{
			if(position <= f2)
			{
				if(strstr(df,f1))
				{
					(strcmp(type2,t) == 0) ? printf("%s\n",df) : printf("\n");	//-f -t
				}
				printf("\n");
			}
			printf("\n");
		}
		else
		{
			if(position <= f2)
			{
				if(strstr(df,f1))
				{
					if(eE != ' ' && eE == 'e')
					{
						execute_unix_command(path,unixcommand);
					}
					else if(eE != ' ' && eE == 'E')
					{
						strcat(listfiles,path);
						strcat(listfiles," ");
					}

					else
					printf("%s\n",df);	//-f
				}
				printf("\n");
			}
		}
	}
	else if(t != NULL)
	{
		(strcmp(type2,t) == 0) ? printf("%s\n",df) : printf("\n");	//-t
	}
	else
	{
		printf("%s",df);	//no commands
		printf("\n");
	}
	
	memset(array, '\0', sizeof(array));
	memset(array1, '\0', sizeof(array1));
}


void execute_unix_command(char *df,char *unixcommand)
{

	int i = 0;

	char *command = strdup(unixcommand);
	for(int i=0;i<1024;i++)
	{
		token[i] = NULL;	
	}

	token[i] = strtok(command," \n");
	while(token[i]!=NULL)
	{
		i++;
		token[i] = strtok(NULL," \n");
	}

	token[i] = strtok(df," \n");
	while(token[i]!=NULL)
	{
		i++;
		token[i] = strtok(NULL," \n");
	}

	pid_t pid;
	int status;
	pid = fork();
	if(pid == 0 )
	{
		execvp(token[0],token);
		printf("If you see this statement then ecexcvp failed\n");
		perror("execvp");
		exit(-1);
	}
	else if (pid > 0) { /* this is the parent process */
        wait(&status); /* wait for the child process to terminate */
        if (WIFEXITED(status)) { /* child process terminated normally */       
		printf("Child process exited with status = %d\n", WEXITSTATUS(status));
        } else { /* child process did not terminate normally */
            printf("Child process did not terminate normally!\n");
            
        }
    } else { 
        perror("fork"); 
        exit(EXIT_FAILURE);
    }
	return;
}

/* 
References -
https://codeforwin.org/2018/03/c-program-to-list-all-files-in-a-directory-recursively.html
https://c-for-dummies.com/blog/?p=4101
https://www.ibm.com/docs/en/i/7.1?topic=functions-memset-set-bytes-value
https://beginnersbook.com/2014/01/c-if-else-statement-example/
https://www.google.com/search?q=conditional+operator+in+c&sxsrf=AJOqlzVFmFcQJ884HduiBnEMSe63bsl09A%3A1676880030916&source=hp&ei=nijzY9r0NZCHwbkPqfKFmAo&iflsig=AK50M_UAAAAAY_M2rp4zcofvy6n4ZDwb3aXc60JSemlS&oq=conditi&gs_lcp=Cgdnd3Mtd2l6EAMYADIECCMQJzIECAAQQzIECAAQQzIFCAAQkQIyBQgAEJECMgUIABCRAjIFCAAQkQIyBQgAEJECMgQIABBDMgQIABBDOgsIABCABBCxAxCDAToICAAQsQMQgwE6CggAELEDEIMBEEM6BAgAEANQAFjWDWCFF2gAcAB4AIABiAGIAdkGkgEDMC43mAEAoAEB&sclient=gws-wiz
https://www.fresh2refresh.com/c-programming/c-operators-expressions/c-conditional-operators/

*/