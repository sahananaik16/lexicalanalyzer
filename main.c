#include <stdio.h>
#include <string.h>
#include "lexer.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        /* Prompting error message if argument count is less than 1 */
        printf("\033[1;31m");
        printf("ERROR:\nInvalid number of arguments!!\nCLA should be: ./a.out filename.c ...\n");
        printf("\033[0m");
        /* Return failure if invalid argument count */
        return failure;
    }
    int ret = validate(argv[1]);
    if (ret == failure)
    {
        return failure;
    }
    initializeLexer(argv[1]);

    Token token;
    while ((token = getNextToken(&token)).type != UNKNOWN)
    {
        printf("Token: %s \tType:",(token.lexeme));
        if (token.type == KEYWORD)
        {
            printf("KEYWORD\n");
        }
        else if (token.type == OPERATOR)
        {
            printf("OPERATOR\n");
        }
        else if (token.type == SPECIAL_CHARACTER)
        {
            printf("SPECIAL_CHARACTER\n");
        }
        else if (token.type == CONSTANT)
        {
            printf("CONSTANT\n");
        }
        else if (token.type == IDENTIFIER)
        {
            printf("IDENTIFIER\n");
        }
        else if (token.type == PREPROCESSOR_DIRECTIVE)
        {
            printf("PREPROCESSOR_DIRECTIVES,\n");
        }
        else if (token.type == LITERAL)
        {
            printf("LITERAL\n");
        }
        else if (token.type == HEADER_FILE)
        { 
            printf("HEADER_FILE\n");
        }
    
    }
     
    return 0;
}

int validate(const char *filename)
{
    /*declaring str array*/
    char str[50];

    /*copying the CLA argument to the string*/
    strcpy(str, filename);
    /*extracting the token from the string*/
    char *name = strtok(str, ".");
    name = strtok(NULL, ".");
    /*condition to check whether all the files are text files*/
    if (strstr(filename, ".c") != NULL && strcmp(name, "c") == 0)
    {
        /*opening the c files*/
        FILE *fptr = fopen(filename, "r");
        /*checking the prescence of file*/
        if (fptr != NULL)
        {
            /*Moving cursor to the end of the file*/
            fseek(fptr, 0, SEEK_END);
            /*Getting the file size*/
            int size = ftell(fptr);
            /*condition to check the content of the file*/
            if (size == 0)
            {
                /*prompting error message if file doesn't contains any content*/
                printf("\033[1;31m");
                printf("%s file doesn't have any content\n", filename);
                printf("\033[0m");
                return failure;
            }
        }
        else
        {
            /*prompting error message if file doesn't exist*/
            printf("\033[1;31m");
            printf("%s file doesn't exist\n", filename);
            printf("\033[0m");
            return failure;
        }
    }
    else
    {
        /*prompting error message if file contains extension other than .txt*/
        printf("\033[1;31m");
        printf("%s file has invalid extension\n", filename);
        printf("\033[0m");
        return failure;
    }
    printf("File validated successfully\n");

return success;
}