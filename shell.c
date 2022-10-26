#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>

char origin[1024];

void initializeShell() {
    printf("\n\t################################################\n");
    printf("\t          Shell by Viviana Longjam \n");
    printf("\t################################################\n\n");
}

void printCurrentDirectory() {
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));
	printf("%s$ ", cwd);
}

char* readinput() {
    char* input;
    size_t size = 100;
    size_t curr_line;
    input = (char*)malloc(size*sizeof(char));
    curr_line = getline(&input, &size, stdin);

    if(curr_line == -1){
        printf("Error: Couldn't allocate memory to input\n");
        exit(1);
    }
    return input;
}

char **parse(char* input){
	int pos = 0;
	char **tokens = calloc(1024, (sizeof(char*) + 1));
	char *token;
	token = strtok(input, " \n");
	while(token != NULL){
		tokens[pos] = token;
		pos++;
		token = strtok(NULL, " \n");
	}
    tokens[pos] = "\0";
	return tokens;
}

int echo(int argc, char* argv[]) {
    int i;
    if (argc == 1) {
        printf("\n");
    } else {
        if(strcmp(argv[1], "-n") == 0){ //option to print the given statement without any \n for the next terminal prompt
            for(i = 2; i < argc-1; i++){
                printf("%s ", argv[i]);
            }
            printf("%s", argv[i]);
        }
        else if(strcmp(argv[1], "--help") == 0){ //displays the help page
            char cwd[1024];
	        getcwd(cwd, sizeof(cwd));
            chdir("/");
            chdir(origin);
            chdir("Descriptions");
            FILE *fp;
            fp = fopen("echo.txt", "r");
            char *fline = NULL;
            size_t flen = 0;
            while(getline(&fline, &flen, fp) != -1){
                printf("%s", fline);
            }
            printf("\n");
            fclose(fp);
            chdir("/");
            chdir(cwd);
        }
        else{
            for(i = 1; i < argc; i++){ //no option implemented, it will print wrong/invalid options as well (error case handled)
                printf("%s ", argv[i]);
            }
            printf("\n");
        }
    }
    return 1;
}

int pwd(int argc, char* argv[]) {
    if (argc == 1) {
        printf("Current Working Directory: %s \n", getenv("PWD"));
    }
    else if(argc == 2) {
        if (strcmp(argv[1], "-L") == 0) {
            printf("Current Working Directory: %s \n", getenv("PWD"));
        }
        else if(strcmp(argv[1], "-P") == 0){
            char presentWorkingDirectory[1000];
            printf("Current Working Directory: %s \n", getcwd(presentWorkingDirectory, sizeof(presentWorkingDirectory)));
        }
        else{
            printf("Invalid option\n");
        }
    }
    else{
        printf("Invalid option\n");
    }
    return 1;
}

int cd(int argc, char** command) {
    if (argc > 3) {
        printf("Incorrect number of parameters\n");
        return 1;
    } 
    if (argc == 1) {
        chdir(getenv("HOME")); 
    }
    else if(argc == 2){
        if(strcmp(command[1], "~") == 0 || strcmp(command[1], "-P") == 0 || strcmp(command[1], "-L") == 0) {
            chdir(getenv("HOME"));
        }
        else if (chdir(command[1]) == -1) {
		    printf("%s: no such directory\n", command[1]);
        }
    }
    else if(argc == 3){
        if(strcmp(command[1], "-P") == 0) {
            if (chdir(getenv(command[2])) == -1) {
                printf("%s: no such directory\n", command[2]);
            }
        }
        else if(strcmp(command[1], "-L") == 0) {
            if (chdir(command[2]) == -1) {
                printf("%s: no such directory\n", command[2]);
            }
        }
        else{
            printf("Invalid option\n");
        }
    }
    return 1;
}

int checkAndExecuteInternal(int argc, char** command) {
    if (strcmp(command[0], "cd") == 0) {
        return cd(argc, command);
	}
	if (strcmp(command[0], "pwd") == 0) {
	    return pwd(argc, command);
	}
	if (strcmp(command[0], "echo") == 0) {
	    return echo(argc, command);
	}
    if (strcmp(command[0], "exit") == 0) {
	    exit(0);
	}
    return 0;
}

void checkAndExecuteExternal(int argc, char** command) {
    char outputFilePath[2048] = {};

    if (strcmp(command[0], "cat") == 0) {
        strcat(outputFilePath, "./Out/mycat");
	} else if (strcmp(command[0], "date") == 0) {
        strcat(outputFilePath, "./Out/mydate");
	} else if (strcmp(command[0], "ls") == 0) {
        strcat(outputFilePath, "./Out/myls");
	} else if (strcmp(command[0], "mkdir") == 0) {
        strcat(outputFilePath, "./Out/mymkdir");
	} else if (strcmp(command[0], "rm") == 0) {
        strcat(outputFilePath, "./Out/myrm");
	} 

    if (execv(outputFilePath, command) == -1) {
        printf("Execution of command failed due to error in execv.\n");
    }
}

void* checkAndExecuteExternalByAPI(void* inputString) {
    char* inputStringChar = (char*) inputString;
    char** command = parse(inputStringChar);
    
    int argc = 0;
    while(strcmp(command[argc], "\0") != 0){
        argc++;
    }
    char** copy = calloc(1024, argc);
    for (int j = 0; j < argc; j++) {
        copy[j] = command[j];
    }

    char outputFilePath[1024];

    if (strcmp(command[0], "cat&t") == 0) {
        strcat(outputFilePath, "./Out/mycat");
	} else if (strcmp(command[0], "date&t") == 0) {
        strcat(outputFilePath, "./Out/mydate");
	} else if (strcmp(command[0], "ls&t") == 0) {
        strcat(outputFilePath, "./Out/myls");
	} else if (strcmp(command[0], "mkdir&t") == 0) {
        strcat(outputFilePath, "./Out/mymkdir");
	} else if (strcmp(command[0], "rm&t") == 0) {
        strcat(outputFilePath, "./Out/myrm");
	}

    for (int i = 1; i < argc; i++) {
        strcat(outputFilePath, " ");
        strcat(outputFilePath, copy[i]);
    }

    if (system(outputFilePath) == -1) {
        printf("Execution of command failed due to error in system() call.\n");
    }
    
    return NULL;
}

int main() {
    initializeShell();
    int shouldExit = 0;
    int isInternalCommand = 0, isExternalCommand = 0, isExternalAPICommand = 0;

    while (!shouldExit) {
        isInternalCommand = 0, isExternalCommand = 0, isExternalAPICommand = 0;
        printCurrentDirectory();

        char *inputString;
        inputString = readinput();
        char *inputStringCopy = malloc(strlen(inputString));
        inputStringCopy = strcpy(inputStringCopy, inputString);

        if (strcmp(inputString, "\n") == 0) {
            continue;
        } else {
            char** command = parse(inputString);
            int argc = 0;
            while(strcmp(command[argc], "\0") != 0){
                // printf("%s\n", parsed[i]);
                argc++;
            }
            char** copy = calloc(1024, argc);
            for (int j = 0; j < argc; j++) {
                copy[j] = command[j];
            }
            isInternalCommand = checkAndExecuteInternal(argc, command);
            
            if ((strcmp(command[0], "cat") == 0) || (strcmp(command[0], "date") == 0) || (strcmp(command[0], "ls") == 0) || (strcmp(command[0], "mkdir") == 0) || (strcmp(command[0], "rm") == 0)) {
                isExternalCommand  = 1;
	        } else if ((strcmp(command[0], "cat&t") == 0) || (strcmp(command[0], "date&t") == 0) || (strcmp(command[0], "ls&t") == 0) || (strcmp(command[0], "mkdir&t") == 0) || (strcmp(command[0], "rm&t") == 0)) {
                isExternalAPICommand = 1;
            } else if (!isInternalCommand) {
                printf("command not found: %s\n", command[0]);
            }

            if (isExternalCommand) {
                pid_t pid = fork();
                if (pid < 0) {
				    printf("Error in creating child process.\n");
                } else if (pid == 0) {
                    printf("Created child process.\n");
                    checkAndExecuteExternal(argc, copy);
                } else if (pid > 0) {
                    pid_t p_temp = waitpid(pid, NULL, WUNTRACED);
				    if(p_temp < 0) {
					    printf("Error no %d\n", errno);
					    perror("waitpid");
				    }
                }
            }
            
            if (isExternalAPICommand) {
                printf("Created thread\n");
                pthread_t tid;
                // printf("%s\n", inputStringCopy);
                if (pthread_create(&tid, NULL, &checkAndExecuteExternalByAPI, inputStringCopy) != 0) {
                    printf("Error in creating thread");
                }
                if (pthread_join(tid, NULL) != 0) {
                    printf("Error in execution of thread");
                };
                printf("Done with thread\n");
            }
        }
    }

    return 0;
}