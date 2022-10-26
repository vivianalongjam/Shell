#include <stdio.h>                                                                
#include <time.h>
#include <string.h>
#include <stdlib.h>

#define BUF_LEN 256

int date(int flag_u, int flag_I) {
    char buf[BUF_LEN] = {0};                                                                               
    char* current_time;    
    struct tm* UTCTime;
                                                              
    time_t t = time(NULL);                                                        
    current_time = ctime(&t);    
    struct tm *ptm = localtime(&t); 

    UTCTime = gmtime(&t);
    time_t utc_time = mktime(UTCTime); 

    if (flag_I) {
        memset(buf, 0, BUF_LEN);
        strftime(buf, BUF_LEN, "The date in ISO format is %F", ptm);
        puts(buf);
    } else if (flag_u) {
        printf("UTC time: %s", ctime(&utc_time)); 
    } else {
        printf("Local time: %s", current_time); 
    }                                                                                                                    
    return 1;   
}
                                                                                
int main(int argc, char *argv[]) {
    if (argc == 1) {
        date(0, 0);
    } else if (argc == 2) {
        if (argv[1][0] == '-') { 
            int flag_u = 0, flag_I = 0;
            char *p = (char *)(argv[1] + 1);
            while (*p) {
                if (*p == 'u') flag_u = 1;
                else if (*p == 'I') flag_I = 1;
                else {
                    perror("Unknown option error");
                    exit(EXIT_FAILURE);
                }
                p++;
            }
            date(flag_u, flag_I);
        } else {
            date(0, 0);
        }
    } else {
        perror("Invalid number of arguments");
        exit(EXIT_FAILURE);
    }
    return 0;                  
}   