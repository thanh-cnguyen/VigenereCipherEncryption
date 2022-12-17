#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>

/* Hold max quantity of 254 */
#define MAX_SIZE 75

/* List of functions */
int logging(FILE *fp);
void toUpperCase();
void printRes();
void inputSplit();

int main(int argc, char *argv[])
{
    // Require a log file to run
    if (argc != 2)
    {
        fprintf(stderr, "Error: Missing a text file to save log history!\n");
        exit(1);
    }
    else
    {
        // Open the text file to log in append mode
        FILE *fp = NULL;
        fp = fopen(argv[1], "a+");
        if (fp == NULL)
        {
            fprintf(stderr, "Error: Opening the log file unsuccessfully!\n");
            exit(1);
        }

        // Call logging function to start logging process
        if (logging(fp) < 0)
        {
            fclose(fp);
            fprintf(stderr, "Error: Logger failed!\n");
            exit(1);
        }
        fclose(fp);
    }
    return 0;
}

int logging(FILE *fp)
{
    /* This method logs run history to a file */

    // Declare and obtain time in seconds
    struct tm *local;
    char buf[80];
    time_t currentTime;
    time(&currentTime);

    // Obtain current date and time
    local = localtime(&currentTime);
    strftime(buf, 80, "%Y-%m-%d %H:%M:%S", local);

    // Declare action, message, and space-eater variables
    char *input = (char *)malloc(MAX_SIZE);
    char **args[MAX_SIZE];

    // Log begins
    printRes(fp, buf, "START", "Logging Started");

    while (1) // Logging until it receives "QUIT"
    {
        // Read inputs from terminal
        if (read(STDIN_FILENO, input, MAX_SIZE) < 0)
        {
            fprintf(stderr, "Error: Logger read fails.\n");
            exit(1);
        }
        toUpperCase(input);
        inputSplit(input, args);
        if (strcmp((char *)args[0], "QUIT") == 0)
        {
            // Log ends prints terminate message
            printRes(fp, buf, "STOP", "Logging Stopped");
            break;
        }
        else
            printRes(fp, buf, args[0], args[1]);
    }
    return 0;
}

void inputSplit(char *command, char **args)
{
    // Process the command
    args[0] = strtok(command, " \n");

    // Process the message
    args[1] = strtok(NULL, "\n");
}

void printRes(FILE *fp, char *buf, char *act, char *msg)
{
    /* This method writes log history to a file */

    fprintf(fp, "%s [%s] %s.\n", buf, act, msg);

    /*
        To display the log history on terminal,
        uncomment the command below:
    */

    // printf("%s [%s] %s.\n", buf, act, msg);
}

void toUpperCase(char *src)
{
    /* This method uppercases the src string*/

    int i = 0;
    while (src[i]) // Format until end of string
    {
        src[i] = toupper((unsigned char)src[i]);
        i++;
    }
}
