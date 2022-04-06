#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Hold max quantity of 50 */
#define MAX_SIZE 51

/* List of functions */
void toUpperCase();
void encrypting();
void passKey();
void vigEncrypt();
void vigDecrypt();

/* List of data structure and public variables */
char passToUse[MAX_SIZE];
const char alphabetStr[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

int main(int argc, char *argv[])
{
    // Execute encryption/decryption using Vigenere Cipher
    encrypting();
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

void encrypting()
{
    /* This method performs encryption/decryption on the message */

    // Declare command and space-eater variables
    char command[7];
    char augument[MAX_SIZE];
    char dump;

    // Get commands and message until "QUIT"
    while ((strcmp(command, "QUIT") != 0))
    {
        // Display available Vigenere Cypher commands
        printf("Encrypter commands: \n1. PASSKEY \n2. ENCRYPT \n3. DECRYPT \n4. QUIT \nPlease enter in format [COMMAND] [message]: ");

        // Read inputs from terminal
        scanf("%s", command);
        toUpperCase(command);
        scanf("%c", &dump); // This line to remove spaces from last input

        // Process commands
        if (strcmp(command, "QUIT") == 0)
            break;
        else if (strcasecmp(command, "PASSKEY") == 0) // Set a password to the message
        {
            scanf("%[^\n]", augument);
            toUpperCase(augument);
            passKey(augument);
        }
        else if (strcasecmp(command, "ENCRYPT") == 0) // Vigenere encryption
        {
            scanf("%[^\n]", augument);
            toUpperCase(augument);
            vigEncrypt(augument);
        }
        else if (strcasecmp(command, "DECRYPT") == 0) // Vigenere decryption
        {
            scanf("%[^\n]", augument);
            toUpperCase(augument);
            vigDecrypt(augument);
        }
        else
            printf("Error: Invalid Command!\nPlease follow the input format.\n");
        printf("\n");
    }
}

void passKey(char *pass)
{
    /* This method prepares a password ready for en/de-cryption */

    strcpy(passToUse, pass);
    printf("RESULT Password is set.\n");
}

void vigEncrypt(char *msg)
{
    /* This method encrypts the message */

    // Declare variables and generate encryption table
    int i;
    char mappedPass[MAX_SIZE];
    char encryptedText[MAX_SIZE];

    // Verify available password
    if (strcmp(passToUse, "") == 0)
    {
        printf("ERROR Password not set.\n");
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
    printf("RESULT %s\n", encryptedText);
}

void vigDecrypt(char *msg)
{
    /* This method decrypts the message */

    // Declare variables and generate encryption table
    int i;
    char mappedPass[MAX_SIZE];
    char decryptedText[MAX_SIZE];

    // Verify available password
    if (strcmp(passToUse, "") == 0)
    {
        printf("ERROR Password not set.\n");
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
    printf("RESULT %s\n", decryptedText);
}
