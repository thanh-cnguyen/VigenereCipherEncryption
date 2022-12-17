#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

/* Hold max quantity 254*/
#define MAX_SIZE 255

int main(int argc, char *argv[])
{

    // Declare pipes and processId
    /*
        index 0 = read mode
        index 1 = write mode
    */

    int pLog[2];
    int pEnc[2];
    int pDri[2];
    pid_t child1;
    pid_t child2;
    char *buf = (char *)malloc(MAX_SIZE);
    char *buf2 = (char *)malloc(MAX_SIZE);

    // Verify if pipes are ready to work

    if (pipe(pLog) < 0)
    {
        fprintf(stderr, "Error: Unable to create Log Pipe.\n");
        exit(1);
    }

    if (pipe(pEnc) < 0)
    {
        fprintf(stderr, "Error: Unable to create Encrypt Pipe.\n");
        exit(1);
    }

    if (pipe(pDri) < 0)
    {
        fprintf(stderr, "Error: Unable to create Driver Pipe.\n");
        exit(1);
    }

    /* ********************************************************************* LOGGER BEGINS ********************************************************************* */

    // Verify fork system call
    if ((child1 = fork()) < 0)
    {
        fprintf(stderr, "Error: Unable to create Logger process.\n");
        exit(1);
    }

    if (child1 == 0)
    {

        // Close unuse pipes
        close(pLog[1]);

        // READ FROM DRIVER
        /* // dup2 leaves input blank
        if (dup2(pLog[0], 0))
        {
            fprintf(stderr, "Error: Logger unable to read from Driver.\n");
            exit(3);
        } */
        if (read(pLog[0], buf, MAX_SIZE) < 0)
        {
            fprintf(stderr, "Error: Logger unable to read from Driver.\n");
            exit(3);
        }

        // Call the executioner
        char *args[] = {"./Logger", "Logger.txt", NULL};
        if (execvp(args[0], args) < 0)
        {
            fprintf(stderr, "Error: Unable to call Logger.\n");
            exit(2);
        }
    }
    else
    {

        /* ********************************************************************* ENCRYPTER BEGINS ********************************************************************* */

        // Verify fork system call
        if ((child2 = fork()) < 0)
        {
            fprintf(stderr, "Error: Unable to create process 2.\n");
            exit(1);
        }

        if (child2 == 0)
        {

            // Close unuse pipes
            close(pEnc[1]);
            close(pDri[0]);

            // READ FROM DRIVER
            /* // dup2 leaves input blank
            if (dup2(pEnc[0], 0) < 0)
            {
                fprintf(stderr, "Error: Encrypter unable to read from Driver.\n");
                exit(2);
            } */
            if (read(pEnc[0], buf, MAX_SIZE) < 0)
            {
                fprintf(stderr, "Error: Encrypter unable to read from Driver.\n");
                exit(2);
            }

            // WRITE TO DRIVER
            /* // dup2 leaves input blank
            if (dup2(pDri[1], 1) < 0)
            {
                fprintf(stderr, "Error: Encrypter unable to write to Driver.\n");
                exit(2);
            } */
            if (write(pDri[1], buf, MAX_SIZE) < 0)
            {
                fprintf(stderr, "Error: Encrypter unable to write to Driver.\n");
                exit(2);
            }

            // Call the executioner
            char *args[] = {"./Encrypter", NULL};
            if (execvp(args[0], args) < 0)
            {
                fprintf(stderr, "Error: Unable to call Encrypter.\n");
                exit(2);
            }
        }
        else
        {

            /* ********************************************************************* PARENT BEGINS ********************************************************************* */
            // Close the unuse
            close(pLog[0]);
            close(pEnc[0]);
            close(pDri[1]);

            /* ATTEMPT TO PROCESS THE OPTIONS

            // while (1)
            // {
            // char cmd[MAX_SIZE];
            // char **args[MAX_SIZE];
            // Program Menu

            */

            // Print the guide
            printf("1. Passkey \n2. Encrypt \n3. Decrypt \n4. History \n5. Quit\nPlease select an option: [Command] [Message]\n");
            char cmd[MAX_SIZE];
            scanf("%s", (char *)cmd);
            strcpy(buf, (char *)cmd);

            /* ATTEMP TO PROCESS THE OPTION

            // scanf("%s", (char *)cmd);
            // inputSplit(cmd, args);
            // char str[MAX_SIZE];

            // // Process selection
            // if (strcasecmp((char *)args[0], "1") == 0)
            // {
            //     strcat(str, (char *)args[0]);
            //     strcat(str, " ");
            //     strcat(str, (char *)args[1]);
            //     strcpy(buf, str);
            // }
            // else if (strcasecmp((char *)args[0], "2") == 0)
            // {
            //     strcat(str, (char *)args[0]);
            //     strcat(str, " ");
            //     strcat(str, (char *)args[1]);
            //     strcpy(buf, str);
            // }
            // else if (strcasecmp((char *)args[0], "3") == 0)
            // {
            //     strcat(str, (char *)args[0]);
            //     strcat(str, " ");
            //     strcat(str, (char *)args[1]);
            //     strcpy(buf, str);
            // }
            // else if (strcasecmp((char *)args[0], "4") == 0)
            // {
            //     // buf = "HISTORY");
            //     strcpy(buf, "HISTORY");
            //     printf("Show History: \n");
            // }
            // else if (strcasecmp((char *)args[0], "5") == 0)
            // {
            //     strcat(str, (char *)args[0]);
            //     strcat(str, " ");
            //     strcat(str, (char *)args[1]);
            //     strcpy(buf, str);
            //     close(pLog[1]);
            //     close(pEnc[1]);
            //     close(pDri[0]);

            //     waitpid(child1, NULL, 0);
            //     waitpid(child2, NULL, 0);

            //     printf("Program terminated.\n");
            //     exit(0);
            // }
            // else
            // {
            //     printf("Invalid option! Please choose 1-5 only.\n");
            //     continue;
            // }

             */

            // Write input to Logger
            if (write(pLog[1], buf, MAX_SIZE) < 0)
            {
                fprintf(stderr, "Error: Driver unable to write to Logger.\n");
                exit(3);
            }

            // Write input to Encrypter
            if (write(pEnc[1], buf, MAX_SIZE) < 0)
            {
                fprintf(stderr, "Error: Driver unable to write to Encrypter.\n");
                exit(3);
            }

            // Read from Encrypter
            if (read(pDri[0], buf2, MAX_SIZE) < 0)
            {
                fprintf(stderr, "Error: Driver unable to read from Encrypter.\n");
                exit(3);
            }
            // write back to Logger
            if (write(pLog[1], buf2, MAX_SIZE) < 0)
            {
                fprintf(stderr, "Error: Driver unable to write back to Logger.\n");
                exit(3);
            }

            // wait for the child to complete
            waitpid(child2, NULL, 0);
            waitpid(child1, NULL, 0);
            // } close the while loop
        }
    }

    return 0;
}
