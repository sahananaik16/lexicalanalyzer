#ifndef LEXER_H
#define LEXER_H

#define MAX_KEYWORDS 20
#define MAX_TOKEN_SIZE 100

/* Defining return status codes */
#define success 0
#define failure -1

typedef enum {
    KEYWORD,
    OPERATOR,
    SPECIAL_CHARACTER,
    CONSTANT,
    IDENTIFIER,
    PREPROCESSOR_DIRECTIVE,
    HEADER_FILE,
    LITERAL,
    UNKNOWN
} TokenType;

typedef struct {
    char lexeme[MAX_TOKEN_SIZE];
    TokenType type;
} Token;

int validate(const char *filename);
void initializeLexer(const char* filename);
Token getNextToken(Token *token);
void categorizeToken(Token* token);
int isKeyword(const char* str);
int isOperator(const char ch);
int isSpecialCharacter(char ch);
int isConstant(const char* str);
int isIdentifier(const char* str);
int isDirective(const char* str);

#endif
