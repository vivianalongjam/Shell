#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
    if (argc < 2) {
        perror("Invalid number of arguments");
        exit(EXIT_FAILURE);
    } else if (argc == 2) {
        char* newdir = argv[1];
        mkdir(newdir, 0777);
    } else if (argc == 3){
        if (strcmp(argv[1], "-v") == 0) {
            char* newdir = argv[2];
            if (mkdir(newdir, 0777)) {
                perror(newdir);
            } else {
                printf("created directory '%s'\n", newdir);
            }
        } else if (strcmp(argv[1], "-p") == 0) {
            char* dirPath = argv[2];
            char* inPath = strtok(dirPath, "/");
            while (inPath != NULL) {
                mkdir(inPath, 0777);
                chdir(inPath);
                inPath = strtok(NULL, "/");
            }
        } else {
            printf("Invalid command\n");
        }
    }
    exit(0);
}