#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Hold max quantity of 254 */
#define MAX_SIZE 75

/* List of functions */
void toUpperCase();
void inputSplit();
int encrypting();
void passKey();
void vigEncrypt();
void vigDecrypt();

/* List of data structure and public variables */
char passToUse[MAX_SIZE];
char output[MAX_SIZE];
const char alphabetStr[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

int main(int argc, char *argv[])
{
    // Execute encryption/decryption using Vigenere Cipher
    if (encrypting() != 0)
    {
        fprintf(stderr, "Encrypter failed!");
        exit(1);
    }
    return 0;
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

int encrypting()
{
    /* This method performs encryption/decryption on the message */

    // Declare command and space-eater variables
    char input[MAX_SIZE];
    char **args[MAX_SIZE];

    // Get commands and message until "QUIT"
    while (1)
    {
        /*
            To display available Vigenere Cypher commands
            uncoomment the command below:
        */

        // printf("Encrypter commands: \n1. PASSKEY \n2. ENCRYPT \n3. DECRYPT \n4. QUIT \nPlease enter in format [COMMAND] [message]: ");

        // Read inputs from terminal
        read(STDIN_FILENO, input, MAX_SIZE);
        toUpperCase(input);
        inputSplit(input, args);
        // Process commands
        if (strcasecmp((char *)args[0], "QUIT") == 0)
        {
            printf("Encrypter terminated!\n");
            break;
        }
        else if (strcasecmp((char *)args[0], "PASSKEY") == 0) // Set a password to the message
            passKey(args[1]);
        else if (strcasecmp((char *)args[0], "ENCRYPT") == 0) // Vigenere encryption
            vigEncrypt(args[1]);
        else if (strcasecmp((char *)args[0], "DECRYPT") == 0) // Vigenere decryption
            vigDecrypt(args[1]);
        else
            printf("Warning: Invalid Command!\nPlease follow the input format.\n");
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

void passKey(char *pass)
{
    /* This method prepares a password ready for en/de-cryption */
    memset(output, 0, MAX_SIZE); // Reset the output
    strcpy(passToUse, pass);
    strcpy(output, "RESULT Password is set");
    write(STDOUT_FILENO, output, MAX_SIZE);
    printf(".\n");
}

void vigEncrypt(char *msg)
{
    /* This method encrypts the message */

    // Declare variables and generate encryption table
    int i;
    char mappedPass[MAX_SIZE];
    char encryptedText[MAX_SIZE];
    memset(output, 0, MAX_SIZE); // Reset the output

    // Verify available password
    if (strcmp(passToUse, "") == 0)
    {
        strcpy(output, "ERROR Password not set");
        write(STDOUT_FILENO, output, MAX_SIZE);
        printf(".\n");
        return;
    }

    // Mapping key to message
    int j;
    j = 0;
    for (i = 0; i < strlen(msg); i++)
    {
        if (msg[i] == 32)
            mappedPass[i] = 32;
        else
        {
            if (j < strlen(passToUse))
                mappedPass[i] = passToUse[j];
            else
            {
                j = 0;
                mappedPass[i] = passToUse[j];
            }
            j++;
        }
    }
    mappedPass[strlen(msg)] = 0; // Terminate the string

    // Generate encrypted string
    for (i = 0; i < strlen(msg); i++)
    {
        // Capture space character
        if (msg[i] == 32 && mappedPass[i] == 32)
            encryptedText[i] = (char)32;
        else
        {
            int x, y;
            x = (int)msg[i] - 65;
            y = (int)mappedPass[i] - 65;
            if ((x + y) >= 26)
                encryptedText[i] = (char)alphabetStr[x + y - 26];
            else
                encryptedText[i] = (char)alphabetStr[x + y];
        }
    }

    // Output result
    strcpy(output, "RESULT ");
    strcat(output, encryptedText);
    write(STDOUT_FILENO, output, MAX_SIZE);
    printf(".\n");
}

void vigDecrypt(char *msg)
{
    /* This method decrypts the message */

    // Declare variables and generate encryption table
    int i;
    char mappedPass[MAX_SIZE];
    char decryptedText[MAX_SIZE];
    memset(output, 0, MAX_SIZE); // Reset the output

    // Verify available password
    if (strcmp(passToUse, "") == 0)
    {
        strcpy(output, "ERROR Password not set");
        write(STDOUT_FILENO, output, MAX_SIZE);
        printf(".\n");
        return;
    }

    // Mapping key to message
    int j;
    j = 0;
    for (i = 0; i < strlen(msg); i++)
    {
        if (msg[i] == 32)
            mappedPass[i] = 32;
        else
        {
            if (j < strlen(passToUse))
                mappedPass[i] = passToUse[j];
            else
            {
                j = 0;
                mappedPass[i] = passToUse[j];
            }
            j++;
        }
    }
    mappedPass[strlen(msg)] = 0; // Terminate the string

    // Generate decrypted string
    for (i = 0; i < strlen(msg); i++)
    {
        // Capture space character
        if (msg[i] == 32 && mappedPass[i] == 32)
            decryptedText[i] = (char)32;
        else
        {
            int x, y;
            x = (int)msg[i] - 65;
            y = (int)mappedPass[i] - 65;
            if ((x - y) < 0)
                decryptedText[i] = (char)alphabetStr[x - y + 26];
            else
                decryptedText[i] = (char)alphabetStr[x - y];
        }
    }

    // Output result
    strcpy(output, "RESULT ");
    strcat(output, decryptedText);
    write(STDOUT_FILENO, output, MAX_SIZE);
    printf(".\n");
}