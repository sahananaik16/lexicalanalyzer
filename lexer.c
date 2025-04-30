#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

static const char *keywords[MAX_KEYWORDS] = {
    "int", "float", "return", "if", "else", "while", "for", "do", "break", "continue",
    "char", "double", "void", "switch", "case", "default", "const", "static", "sizeof", "struct"};

static const char *operators = "+-*/%=!<>|&";
static const char *specialCharacters = ",;{}()[]";
static const char *identifiers[MAX_KEYWORDS] = {"printf", "scanf", "main"};
static const char *directives[MAX_KEYWORDS] = {"#include", "#define", "#undef", "#if", "#endif", "#else", "#elif", "#ifdef", "#ifndef", 
                                               "#error", "#pragma", "#line"};

static FILE *filec;
static char ch;
static int flag=0;

void initializeLexer(const char* filename)
{
      filec=fopen(filename,"r");
      if(!filec)
      {
        printf("Error in opening file!!\n");
        return;
      }
      ch=getc(filec);
}

Token getNextToken(Token *token)
{
    
    memset(token, 0, sizeof(Token));
    token->type = UNKNOWN;

    int index=0;
    if(ch==EOF)
    {
        token->type=UNKNOWN;
        return *token;
    }

    while(ch==' ' || ch=='\n')
    {
        ch=getc(filec);
    }

    if(ch=='#')
    {
        while(isalnum(ch) || ch!=' ')
        {
            token->lexeme[index++]=ch;
            ch=getc(filec);
        }
        token->lexeme[index]='\0';
        token->type= isDirective(token->lexeme) ? PREPROCESSOR_DIRECTIVE : IDENTIFIER ;
        if(token->type==PREPROCESSOR_DIRECTIVE)
        {
            flag=1;
        }
        return *token;
    }


    if(ch == '<' && flag==1)
    {
        
        token->lexeme[index++]=ch;
        ch=getc(filec);
        while(ch!='>')
        {
            token->lexeme[index++]=ch;
            ch=getc(filec);
        }
        token->lexeme[index++]=ch;
        token->lexeme[index++]='\0';
        token->type=HEADER_FILE;
        
        flag=0;
        ch=getc(filec);
        return *token;
    }
    
    if(ch== '"')
    {
        token->lexeme[index++]=ch;
        ch=getc(filec);
        while(ch!='"' && ch!=EOF)
        {
            token->lexeme[index++]=ch;
            ch=getc(filec);
        }
        token->lexeme[index]=ch;
        token->lexeme[index+1]='\0';
        token->type = LITERAL;
        ch=getc(filec);
        return *token;
    }
     
    if (isalpha(ch))
    {
        while (isalnum(ch) || ch == '_')
        {
            token->lexeme[index++] = ch;
            ch = getc(filec);
        }
        token->lexeme[index] = '\0';
        if (isKeyword(token->lexeme)) {
        token->type = KEYWORD;
    } else if (isIdentifier(token->lexeme)) {
        token->type = IDENTIFIER;
    } else {
        token->type = UNKNOWN;
    }
        return *token;
    }

    if (isdigit(ch) || ch == '.')
{
    int hasDecimal = 0;
    while (isdigit(ch) || (ch == '.' && !hasDecimal))
    {
        if (ch == '.')
            hasDecimal = 1;
        token->lexeme[index++] = ch;
        ch = getc(filec);
    }
    token->lexeme[index] = '\0';
    token->type = isConstant(token->lexeme);
    return *token;
}

    
    if(isOperator(ch))
    {
        token->lexeme[index++] = ch;
        token->lexeme[index] = '\0';
        token->type = OPERATOR;
        ch = getc(filec);
        return *token;
    }

    if(isSpecialCharacter(ch))
    {
        token->lexeme[index++]=ch;
        token->lexeme[index] = '\0';
        token->type = SPECIAL_CHARACTER;
        ch = getc(filec);
        return *token;
    }

    token->lexeme[index++] = ch;
    token->lexeme[index] = '\0';
    token->type = UNKNOWN;
    ch = getc(filec);
    return *token;
}

int isDirective(const char* str)
{
    /* run the loop till i is not greater than 12 */
    for (int i = 0; i < 12; i++)
    {
        /* comparing directives[i] with str using strcmp function */
        if ((strcmp(directives[i], str)) == 0)
        {
            /* returning PREPROCESSOR_DIRECTIVES to function getNextToken function */
            return PREPROCESSOR_DIRECTIVE;
        }
    }
    /* returning IDENTIFIER to function getNextToken function */
    return IDENTIFIER;
}

int isKeyword(const char* str) {
    for (int i = 0; i < MAX_KEYWORDS; i++) {
        if (strcmp(keywords[i], str) == 0) {
            return 1; 
        }
    }
    return 0; 
}



int isOperator(const char ch)
{
    for(int i=0;operators[i]!=0;i++)
    {
         if(ch==operators[i])
         {
            return 1;
         }
    }
    return 0;
}

int isSpecialCharacter(char ch)
{
    for(int i=0;specialCharacters[i]!=0;i++)
    {
        if(ch==specialCharacters[i])
        {
            return 1;
        }
    }
    return 0;
}

// int isIdentifier(const char* str)
// {
//     /* run the loop till i is not greater than 3 */
//     for (int i = 0; i < 3; i++)
//     {
//         /* comparing identifiers[i] with str using strcmp function */
//         if ((strcmp(identifiers[i], str)) == 0)
//         {
//             /* returning IDENTIFIER to function getNextToken function */
//             return IDENTIFIER;
//         }
//     }
// }

int isIdentifier(const char* str) {
    // Ensure the first character is a letter or underscore
    if (!isalpha(str[0]) && str[0] != '_') {
        return 0; // Invalid identifier
    }

    // Check remaining characters
    for (int i = 1; str[i] != '\0'; i++) {
        if (!isalnum(str[i]) && str[i] != '_') {
            return 0; // Invalid identifier
        }
    }

    // Ensure it's not a keyword
    if (isKeyword(str)) {
        return 0; // Keywords are not identifiers
    }

    return 1; // Valid identifier
}


int isConstant(const char* str)
{
    int i = 0, hasDecimal = 0;

    // Check for optional sign
    if (str[i] == '-' || str[i] == '+')
        i++;

    // Process digits and optional decimal point
    while (str[i] != '\0')
    {
        if (isdigit(str[i]))
        {
            // Valid digit
        }
        else if (str[i] == '.' && !hasDecimal)
        {
            hasDecimal = 1; // Allow one decimal point
        }
        else
        {
            return UNKNOWN; // Invalid character in constant
        }
        i++;
    }

    // If no digits were processed, it's not a constant
    if (i == 0 || (i == 1 && hasDecimal))
        return UNKNOWN;

    return CONSTANT; // Valid numeric constant
}
