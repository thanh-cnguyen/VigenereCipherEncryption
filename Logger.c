#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>

void logging(const char *action, const char *message);
void logger();
void toUpperCase();
void printTime();

int main(int argc, char **argv[])
{
    printf("this is argv %s\n", argv[1]);
    if (argc != 2)
    {
        printf("Missing log file\n");
        return 1;
    }
    else
    {
        int fd = open(argv[1], O_CREAT | O_APPEND);

        if (fd < 0)
        {
            printf("Error opening the file\n");
            return 1;
        }

        dup2(fd, 1);
        logger();
    }
        
   
    
    
}

void logger()
{
    // Declare necessary variables
    char act[50];
    char msg[100];

    printTime();
    printf("[START] Logging Started.\n");
    while (!(strcmp(act, "QUIT") == 0)) // Stop logging if receive "QUIT"
    {
        // Read from terminal
        scanf("%s", act);
        toUpperCase(act);
        if (strcmp(act, "QUIT") == 0)
            break;
        scanf("%s", msg);
        logging(act, msg);
    }
    printTime();
    printf("[STOP] Logging Terminated.\n");
}

void printTime()
{

    // Declare date and time variables
    int year, month, day, hour, minute, second;

    // Declare and obtain time in seconds
    time_t currentTime;
    time(&currentTime);

    // Obtain current date and time
    struct tm *local = localtime(&currentTime);
    year = local->tm_year + 1900;
    month = local->tm_mon + 1;
    day = local->tm_mday;
    hour = local->tm_hour;
    minute = local->tm_min;

    // Print timestamp
    printf("%d-%02d-%02d %02d:%02d ", year, month, day, hour, minute);
}

void toUpperCase(char *src)
{

    // Format action
    char upperAct[50];
    int i = 0;
    while (src[i])
    {
        src[i] = toupper((unsigned char)src[i]);
        i++;
    }
}

void logging(const char *action, const char *message)
{

    // Print output
    printTime();
    printf("[%s] %s.\n", action, message);
}