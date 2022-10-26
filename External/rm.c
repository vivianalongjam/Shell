#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void rm(int argc, char *argv[], int currentFileIndex, int flag_v, int flag_i) {
    if (flag_i) {
        FILE *fptr = fopen(argv[currentFileIndex], "rb");
        if (fptr == NULL) {
            fprintf(stderr, "rm: %s: No such file or directory", argv[currentFileIndex]);
            exit(1);
        }
        
        char *copy[argc - 2];
        for (int i = 0; i < argc - 2; i++) {
            copy[i] = argv[i + 2];
        }
        
        printf("remove %s? ", argv[currentFileIndex]);
        char response[100];
        scanf("%s", response);
        if((strcmp(response, "y") == 0) || (strcmp(response, "yes") == 0) || (strcmp(response, "Y") == 0) || 
           (strcmp(response, "YES") == 0) || (strcmp(response, "Yes") == 0) || (strcmp(response, "yEs") == 0) || 
           (strcmp(response, "yeS") == 0) || (strcmp(response, "YEs") == 0) || (strcmp(response, "yES") == 0) || 
           (strcmp(response, "YeS") == 0)) {
            int isDeleted = remove(copy[currentFileIndex - 2]);
        }
        return;
    } 
    
    int isDeleted = remove(argv[currentFileIndex]);
    if (isDeleted != 0) {
        printf("rm: %s: No such file or directory\n", argv[currentFileIndex]);
    } else {
        if (flag_v) {
            printf("%s\n", argv[currentFileIndex]);
        }
    }
}

int main(int argc, char *argv[]) {
    int flag_v = 0, flag_i = 0;
    int currentFileIndex = 1;
    if (argc < 2) {
        printf("usage: rm [-f | -i] [-dPRrvW] file ...\n");
        printf("       unlink file\n");
    } else {
        if (argv[1][0] =='-') {
            currentFileIndex++;
            if (argc < 3) {
                perror("Invalid arguments");
                exit(EXIT_FAILURE);  
            } else if (argv[1][1] == 'v') {
                flag_v = 1;
            } else if (argv[1][1] == 'i') {
                flag_i = 1;
            } else {
                perror("Unknown option error");
                exit(EXIT_FAILURE);
            }
        }
        while (currentFileIndex < argc) {
            rm(argc, argv, currentFileIndex, flag_v, flag_i);
            currentFileIndex++;
        }
    }
    return 0;
}