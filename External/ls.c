#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

void ls(const char* dirname, int flag_a, int flag_R) {
    DIR* dir = opendir(dirname);
    if (dir == NULL) {
        if (errno == ENOENT) {
            perror("Directory does not exist");
        } else {
            perror("Unable to read directory");
        }
        exit(EXIT_FAILURE);
    }

    printf("Reading files in: %s\n", dirname);

    struct dirent* entity;
    entity = readdir(dir);
    while (entity != NULL) {
        if (!flag_a && entity->d_name[0] == '.') {
            entity = readdir(dir);
            continue;
        }
        printf("%s/%s\n", dirname, entity->d_name);
        if (flag_R && entity->d_type == DT_DIR && strcmp(entity->d_name, ".") != 0 && strcmp(entity->d_name, "..") != 0) {
            printf("\n");
            char path[100] = {};
            strcat(path, dirname);
            strcat(path, "/");
            strcat(path, entity->d_name);
            ls(path, flag_a, flag_R);
            printf("\n");
        }
        entity = readdir(dir);
    }

    closedir(dir);
}

int main(int argc, const char *argv[]) {
    if (argc == 1) {
        ls(".", 0, 0);
    } else if (argc == 2) {
        if(strcmp(argv[1], "-R") == 0){
            ls(".", 0, 1);
        }
        else if(strcmp(argv[1], "-a") == 0){
            ls(".", 1, 0);
        }
        else if(argv[1][0] == '-'){
            perror("Invalid option error");
            exit(EXIT_FAILURE);
        }
        else{
            ls(argv[1], 0, 0);
        }
    }
    else if (argc == 3) {
        if(strcmp(argv[1], "-a") == 0){
            if(strcmp(argv[2], "-R") == 0){
                ls(".", 1, 1);
            }
            else{
                ls(argv[2], 1, 0);
            }
        }
        else if(strcmp(argv[1], "-R") == 0){
            if(strcmp(argv[2], "-a") == 0){
                ls(".", 1, 1);
            }
            else{
            ls(argv[2], 0, 1);
            }
        }
        else{
            perror("Invalid option error");
            exit(EXIT_FAILURE);
        }
    }
    return 0;
}