#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>

void logging(FILE *fp);
void toUpperCase();
void printRes();

int main(int argc, char *argv[])
{
    // Require a log file to run
    if (argc != 2)
    {
        printf("Error: Missing a text file to save log history!\n");
        exit(1);
    }
    else
    {
        // Open the text file to log in append mode
        FILE *fp = NULL;
        fp = fopen(argv[1], "a+");
        if (fp == NULL)
        {
            printf("Error: Opening the log file unsuccessfully!\n");
            exit(1);
        }

        // Call logging function to start logging process
        logging(fp);

        fclose(fp);
        exit(0);
    }
}

void logging(FILE *fp)
{
    // Declare and obtain time in seconds
    struct tm *local;
    char buf[80];
    time_t currentTime;
    time(&currentTime);

    // Obtain current date and time
    local = localtime(&currentTime);
    strftime(buf, 80, "%Y-%m-%d %H:%M:%S", local);

    // Declare necessary variables
    char act[50];
    char msg[100];
    char dump;

    // Log begins
    printRes(fp, buf, "START", "Logging Started");

    while ((strcmp(act, "QUIT") != 0)) // Logging until it receives "QUIT"
    {
        // Read inputs from terminal
        scanf("%s", act);
        toUpperCase(act);
        if (strcmp(act, "QUIT") == 0)
            break;
        scanf("%c",&dump); // This line to remove spaces from last input
        scanf("%[^\n]", msg);
        printRes(fp, buf, act, msg);
    }

    // Log ends
    printRes(fp, buf, "STOP", "Logging Stopped");
}

void printRes(FILE *fp, char *buf, char *act, char *msg)
{
    // Write log history to a file
    fprintf(fp, "%s [%s] %s.\n", buf, act, msg);

    /*
    To stream the log history on terminal, uncomment the command below:

    printf("%s [%s] %s.\n", buf, act, msg);

    */
}

void toUpperCase(char *src)
{

    // Format the action tag
    int i = 0;
    while (src[i])
    {
        src[i] = toupper((unsigned char)src[i]);
        i++;
    }
}
