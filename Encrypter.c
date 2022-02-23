#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv[])
{

    if (argc != 2)
        printf("Missing log file\n");
    else
    {
        logger();
    }
}