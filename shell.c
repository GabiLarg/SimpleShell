/*Author: Gabriela Pereira de Assis*/

#include "shell.h"
#include "getword.h"

char promp[40]="shell%";
char newfile[100]="";
char buff[STORAGE][MAXITEM];
int p=0;
int i;
int ch1;


void sig(int signalnum){

	}

int listDirectory(char s[]){/*function to handle the ls-F*/

	DIR *dirp;
	struct dirent  *dp;
    struct stat sb;
    char *l;
    FILE *f, list;
	if((dirp = opendir(s))!=NULL){/*open the directory if it doesnt return */
		while(dirp){
			if((dp = readdir(dirp)) != NULL){
                if(dp->d_name[0] != '.'){
                    if(lstat(dp->d_name, &sb)==-1){
                        fprintf(stderr,"Cannot access.");
                        break;
                    }else{
                        if(S_ISREG(sb.st_mode))
                        {
                            if(sb.st_mode & S_IXUSR)
                                fprintf(l"%s* \n", dp->d_name);
                            else
                                (void)printf("%s \n", dp->d_name);
                        }
                        if(S_ISDIR(sb.st_mode))
                            (void)printf("%s/ \n", dp->d_name);
                        if(S_ISLNK(sb.st_mode)){
                            if((f = fopen(dp->d_name, "r"))==NULL)/*if it can not be opened it's a broken link*/
                                (void)printf("%s& \n", dp->d_name);
                            else
                             (void)printf("%s@ \n", dp->d_name);
                            fclose(f);
                        }
                        else
                            continue;
                        }
                }
			}else{
				closedir(dirp);
				break;
			}
		}
		return 0;
	}else{
		return 1;
	}
}
int changeDirectory(char s[]){

	if(strcmp(s,"HOME")==0){
		char *home = getenv(s);
		if((chdir(home)) < 0){
			return 1;
		}else{
			return 0;
		}
	}
	else{
		char *path = s;
		if((chdir(path)) < 0){
			return 1;
		}else{
			return 0;
		}
	}
}
int parse(void){
	int r;
	int waiting = 0;
	int append = 0;
	int redirect = 0;
	int reading = 0;
	char s[STORAGE];

	i=0;
	for(;;){
		r=getword(s); /*calls the getword function*/
		if(r == -1)return r;
		if(r!= 0 &&  *s != '&' && *s!='>' && *s!='<' && redirect == 0 && waiting == 0 && append == 0){
		/*If it isn't a metacharecter or if any mecharacter was found record the input in the buff*/
            if(*s == '|'){
                p++;
                ch1 =i;
            }
            strcpy(buff[i], s);
            i++;

		}else if(strcmp(s, ">>")==0){
		/*If it finds some redirecion signal and sets redirectio as 1*/
			append = 1;
		}else if(*s=='&'){
		/*If it finds an & sets waiting as 1*/
			waiting = 1;
		}else if(*s=='>'){
            redirect = 1;

        }else if(*s=='<'){
            reading = 1;
        }
		if(redirect==1){
		/*if redirection is 1 the gets next word, sets it on newfile an d return 1*/
			r=getword(s);
			strcpy(newfile, s);
			return 1;
		}else if(waiting==1){
		/*if waiting is 1 returns 2*/
			return 2;
		}else if (append == 1){
            r=getword(s);
			strcpy(newfile, s);
			return 3;
		}else if (reading == 1){
            r=getword(s);
			strcpy(newfile, s);
			return 4;
		}

		else if(r==0){
		/*If it fins a 0 it means new line or ;, returns 0*/
			return 0;
		}
	}
}
int main(int argc, char *argv[]) {
	int c;
	char *args[MAXITEM];
	char *args2[MAXITEM];
	FILE *fl;
	int pipes[2];
	for (;;){
		(void)printf("%s",promp);
		signal(SIGTERM,sig);
		int j=0;
		c = parse(); /*calls the parse funcion*/
		if(c==-1)break;
 		j = strlen(*buff);/*verify the number of character in buff*/
		if(j==0)continue;/*If there is no character return to the for*/

		if(c>=0 && j!=0){
		/*if parse returns a number greater thanor equal 0 and buff wasn't empaty starts the executions*/
			fflush(NULL);
			if(strcmp(buff[0], "cd")==0){
			/*if the first word is a cd verifies the arguments*/
				if(i>3){
				/*if it was more than 3 arguments (with cd)prints a error*/
					fprintf(stderr,"cd: Many arguments.\n");
				}else if(i==1){
				/*if is only cd send home to the chageDirector function*/
					if(changeDirectory("HOME")>0){
					/*verifies errors*/
						fprintf(stderr,"cd:Cannot access\n");
					}
				}else{
					if(changeDirectory(buff[1])>0){
					/*verifies errors*/
						fprintf(stderr, "p2: cd: No such file or directory\n");
					}
				}

				for(j=0; j<MAXITEM;j++){/*cleans the buffer*/
					args[j] = '\0';
					buff[j][j]='\0';
				}
				continue;
			}else if ((strcmp(buff[0], "ls-F")==0)){/*If it finds an ls-F*/
				if(i==1){/*verifies if its is just ls-F*/
					if(listDirectory(".")>0){/*Verifies is the function resturn 1 and print a error*/
						fprintf(stderr,"ls: Cannot access.\n");
					}


				}else{
					if(listDirectory(buff[1])>0){/*Verifies is the function resturn 1 and print a error*/
				 		fprintf(stderr,"ls: Cannot access.\n");
					}
				}

				int j;
				for(j=0; j<MAXITEM;j++){/*Cleans the buffer*/
					args[j] = '\0';
					*buff[j]='\0';
				}
				continue;
			}else if(strcmp(buff[0], "printenv")==0 && p==0){
                    if(i > 2){
                        fprintf(stderr,"printenv: Cannot access.\n");
                    }else{
                        char *env = getenv(buff[1]);
                        (void)printf("%s \n",env);
                    }
			}else if(strcmp(buff[0], "setenv")==0){
                     if(i > 3){
                        fprintf(stderr,"setenv: Cannot access.\n");
                    }else{
                       setenv(buff[1], buff[2], 1);
                    }
			}else{
			/*If it finds anything else*/
                if(p==0){
				int j;
				for(j=0;j<i;j++){
                    if(*buff[j] != '$'){
                        args[j] = buff[j]; /*sets the buffer values in agrs*/
                    }else{
                        int i=0;
                        while(buff[j][i] != '\0'){
                            buff[j][i] = buff[j][i+1];
                            i++;
                        }e
                        char *env = getenv(buff[j]);
                        args[j] = env;
                    }
                }

                args[i] = NULL;/*sets the last value as NULL*/

				int kid;
				fflush(NULL);
				if (0==(kid=fork())){
				/*starts the fork and verifies if it works*/
					if(c==1){
					/*If parse returns 1 redirects the output to the new file*/
						if(open(newfile, O_CREAT|O_WRONLY, S_IWRITE | S_IREAD)<0){
						/*if open returns 1 the file did not exist*/
							fl= fopen(newfile, "ab+");/*creates the file and redirects*/
							dup2(open(newfile, O_CREAT|O_WRONLY, S_IWRITE | S_IREAD), fileno(stdout));
						}else if((open(newfile, O_CREAT|O_WRONLY, S_IWRITE | S_IREAD)<0)){
						/*if it cannot open prints a error message*/
							fprintf(stderr,"Can not open.\n");
						}else{
						/*if is returns 0 redirects*/
							dup2(open(newfile, O_CREAT|O_WRONLY, S_IWRITE | S_IREAD), fileno(stdout));
						}
						close(*newfile);
					}
					if(c==2){
					/*If parse returns 2 means to not wait the child*/
						dup2(open("/dev/null", 2), fileno(stdin));/*redirects the input to devnull*/
					}
					if(c==3){
                        /*If parse returns 1 redirects the output to the new file*/
						if(open(newfile, 8|O_APPEND|O_WRONLY)<0){
						/*if open returns 1 the file did not exist*/
							fl= fopen(newfile, "ab+");/*creates the file and redirects*/
							dup2(open(newfile, 8|O_APPEND|O_WRONLY), fileno(stdout));
						}else if((open(newfile, 8|O_APPEND|O_WRONLY)<0)){
						/*if it cannot open prints a error message*/
							fprintf(stderr,"Can not open.\n");
						}else{
						/*if is returns 0 redirects*/

                            fl = fopen(newfile, "ab+");
							dup2(open(newfile, 8|O_APPEND|O_WRONLY), fileno(stdout));
							fprintf(fl, "\n");
						}
						close(*newfile);
					}
					if (c==4){
                        if(open(newfile, 8|O_RDWR)<0){
                            fprintf(stderr, "File does not exist.\n");
						}else if((open(newfile, 8|O_RDWR)<0)){
						/*if it cannot open prints a error message*/
							fprintf(stderr,"Can not open.\n");
						}else{
						/*if is returns 0 redirects*/
							dup2(fileno(stdout), open(newfile, 8|O_RDWR));
						}
						close(*newfile);
					}
					if (execvp(args[0], args) ==-1){
                    /*checks if the program can execute*/
                        fprintf(stderr,"Can not execute\n");/*if not prints a error message*/
                        exit(1);
                    }
                    int j;
                    for(j=0; j<MAXITEM;j++){
                    /*Cleans the buffer*/
                        args[j] = '\0';
                        *buff[j]='\0';
                    }
                }else if(kid<0){
				/*if forks return -1 the process can not be foked, then prints a error message */
					fprintf(stderr,"Cannot fork!\n");
					exit(9);
				}else if(kid>0){

                /*If is return a 1 goes to the parent process*/
                    if(c==1 || c==0){
                    /*if parse returns 0 or 1 wait for the child process finish */
                        while(kid !=(wait(NULL))){}
                    }else if(c==2){
                    /*if parse returns 2 prints the child process name and number and keeps executing other process*/
                        (void)printf("%s [%d]\n", args[0], kid);
                    }
				}

				}else if(p==1){

                    int n = 0;
                    int m = 0;
                    for(j=0;j<i-1;j++){

                        if(*buff[j] == '|'){
                            n=1;
                        }
                        if(n==0){
                            if(*buff[j] != '$'){
                            args[j] = buff[j]; /*sets the buffer values in agrs*/


                            }else{
                                int i=0;
                                while(buff[j][i] != '\0'){
                                    buff[j][i] = buff[j][i+1];
                                    i++;
                            }
                                char *env = getenv(buff[j]);
                                args[j] = env;
                            }
                        }else{
                            if(*buff[j] != '$'){
                                args2[m] = buff[j+1]; /*sets the buffer values in agrs*/
                                m++;
                            }else{
                                int i=0;
                                while(buff[j][i] != '\0'){
                                    buff[j][i] = buff[j][i+1];
                                    i++;
                                }
                                char *env = getenv(buff[j]);
                                args2[m] = env;
                                m++;
                            }
                        }
                    }
                    args[ch1] = NULL;
                    args2[m] = NULL;
                    pipe(pipes);
                    int kid;

                    if(0>(kid=fork())){
                    /*Fork the first child and check if its a error*/
                        fprintf(stderr,"Cannot fork!\n");
                        exit(9);
                    }else if(kid==0){
                    /*Success fork*/
                        dup2(pipes[1],fileno(stdout)); /*redirects the stdout to write the pipe*/
                        close(pipes[0]);
                        close(pipes[1]);
                        if (execvp(args[0], args) ==-1){
                        /*checks if the program can execute*/
                            fprintf(stderr,"Can not execute\n");/*if not prints a error message*/
                            exit(1);
                        }

                    }else{
                        if(0>(kid=fork())){
                        /*Fork the second child and check if its a error*/
                            fprintf(stderr,"Cannot fork!\n");
                            exit(9);
                        }else if(kid==0){
                            /*Sucess fork*/
                            dup2(pipes[0],fileno(stdin));/*Redirects the stdin to read the pipe*/
                            close(pipes[0]);
                            close(pipes[1]);
                            if (execvp(args2[0], args2) ==-1){
                            /*checks if the program can execute*/
                                fprintf(stderr,"Can not execute\n");/*if not prints a error message*/
                                exit(1);
                            }

                        }
                    }
                    close(pipes[0]);
                    close(pipes[1]);
                    if(c==1 || c==0){
                    /*if parse returns 0 or 1 wait for the child process finish */
                        while(kid !=(wait(NULL))){}
                    }else if(c==2){
                    /*if parse returns 2 prints the child process name and number and keeps executing other process*/
                        (void)printf("%s [%d]\n", args[0], kid);
                    }
                    p=0;
                }else if(p>1){
                    fprintf(stderr,"Pipe error\n");
                    p=0;
                }
			}
		}
	}
	/*If parse return -1 the program leaves the for and kill all the process from group and send the signal to sig function*/
	if(killpg(getpid(), SIGTERM)==0){
	/*verifies if killpg works*/
	}else{
		exit(128+15);/*if it doesnt exit*/
	}
	(void)printf("Program has terminated.\n");
	return 0;
}
