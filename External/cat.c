#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void cat_stdin() {
    int bufferSize = 4096;
    char buffer[bufferSize];

    while (fgets(buffer, bufferSize, stdin)) {
        int len = strlen(buffer);
        buffer[len - 1] = '\0';
        fprintf(stdout, "%s\n", buffer);
    }
}

void catFile(int argc, char *argv[], int currfIndex, int flag_s, int flag_b) {
    int bufferSize = 4096;
    char buffer[bufferSize];

    FILE *fptr = fopen(argv[currfIndex], "rb");
    if (fptr == NULL) {
        fprintf(stderr, "cat: %s: No such file or directory", argv[currfIndex]);
        exit(1);
    }
    
    int isLastBlank = 0;
    int lineNumber = 1;

    while (fgets(buffer, bufferSize, fptr)) {
        int len = strlen(buffer);
        if (flag_s) {
            len = strlen(buffer);
            // int isBlank = (len <= 1) ? 1 : 0;
            int isBlank = 0;
            if (len <= 1) {
                isBlank = 1;
            }
            if (isLastBlank && isBlank) {
                continue;
            }
            isLastBlank = isBlank;
        }

        if (flag_b) {
                len = strlen(buffer);
                if (len >= 1) {
                    char *tmp = strdup(buffer);
                    buffer[0] = '\0';
                    sprintf(buffer, "%*d\t", 6,  lineNumber++);
                    strcat(buffer, tmp);
                }
        }
        fprintf(stdout, "%s", buffer);
    }
    fclose(fptr);
}

int main(int argc, char *argv[]) {
    int flag_s = 0, flag_b = 0;
    if (argc < 1) {
        perror("Unknown option error");
        exit(EXIT_FAILURE);
    } else if (argc > 1) {
        if (argv[1][0] != '-') {
            int currfIndex = 1;
            while (currfIndex < argc) {
                catFile(argc, argv, currfIndex, 0, 0);
                currfIndex++;
            }
        } else if (argv[1][0] == '-' && argv[1][1] == 's') {
            flag_s = 1;
            catFile(argc, argv, argc - 1, 1, 0);
        } else if (argv[1][0] == '-' && argv[1][1] == 'b') {
            flag_b = 1;
            catFile(argc, argv, argc - 1, 0, 1);
        } else {
            perror("Unknown option error");
            exit(EXIT_FAILURE);
        }
    } else {
        cat_stdin();
    }
    printf("\n");
    return 0;
}