/* main.c
 *
 * CS 210.Wilder.........._MSC_VER 1910............Hunter Casteel
 * September 16, 2019 .....Core i7-8700............cast9209@vandals.uidaho.edu
 *
 * A program that parses a CCX file into tokens
 *---------------------------------------------------------------------
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int checkCharacter(FILE*);
int isComment(FILE*);
int isString(FILE*);
int isKeyword(FILE*);
int isIdentifier(FILE*);
int isCharLiteral(FILE*);
int isOperator(FILE*);
int isNumLiteral(FILE*);
char currentCharacter;

int main(int argc, char *argv[])
{
    if(argc <= 1)
    {
        printf("[Error: No File Input]");
        return -1;
    }
    else if(argc > 2)
    {
        printf("[Error: Too Many Arguments]");
        return -1;
    }
    else if(strstr(argv[1], ".ccx") == NULL && strstr(argv[1], ".cci") == NULL)
    {
        printf("[Error: Invalid File]");
        return -1;
    }
    
    FILE *file = fopen(argv[1], "r");

    while((currentCharacter = fgetc(file)) != EOF)
    {
        switch(checkCharacter(file))
        {
            case 1:
                printf(" (comment)\n");
                break;
            case 2:
                printf(" (string)\n");
                break;
            case 3:
                printf(" (keyword)\n");
                break;
            case 4:
                printf(" (identifier)\n");
                break;
            case 5:
                printf(" (character literal)\n");
                break;
            case 6:
                printf(" (operator)\n");
                break;
            case 7:
                printf(" (numeric literal)\n");
                break;
            case 8:
                if(currentCharacter == EOF)
                    return 0;
                else
                {
                    printf("%c (UNK)\n", currentCharacter);
                    return -1;
                }
        }
    }

    return 0;
}

int checkCharacter(FILE *file)
{
    char ignoreCharacters[] = {' ', '\n', '\t', '\0', EOF};
    char *doesContain;

    while(doesContain = strchr(ignoreCharacters, currentCharacter))
        currentCharacter = fgetc(file);
    
    if(isComment(file))
        return 1;
    else if(isString(file))
        return 2;
    else if(isKeyword(file))
        return 3;
    else if(isIdentifier(file))
        return 4;
    else if(isCharLiteral(file))
        return 5;
    else if(isOperator(file))
        return 6;
    else if(isNumLiteral(file))
        return 7;
    else
        return 8;
}

int isComment(FILE *file)
{
    if(currentCharacter == '/')
    {
        if((currentCharacter = fgetc(file)) == '*')
        {
            printf("/*");
            currentCharacter = fgetc(file);

            while(currentCharacter != '*' && currentCharacter != EOF)
            {
                printf("%c", currentCharacter);

                if((currentCharacter = fgetc(file)) == '*')
                {
                    printf("%c", currentCharacter);

                    if((currentCharacter = fgetc(file)) == '/')
                    {
                        printf("%c", currentCharacter);
                        return 1;
                    }
                }
            }
        }
        else
            fseek(file, -1L, SEEK_CUR);
    }

    return 0;   
}

int isString(FILE *file)
{
    if(currentCharacter == '\"')
    {
        do
        {
            if(currentCharacter == '\\')
            {
                printf("%c", currentCharacter);
                currentCharacter = fgetc(file);
            }

            printf("%c", currentCharacter);
        }
        while((currentCharacter = fgetc(file)) != '\"');

        printf("%c", currentCharacter);
        return 1;
    }
    
    return 0;
}

int isKeyword(FILE *file)
{
    char *keywords[] = {"accessor", "and", "array", "begin", "boolean", "case", "character", "constant", "else", "elsiftrue", "end", "exit", "float", "functor", "iftrue", "integer", "interface", "in", "is", "loop", "mutator", "natural", "nil", "of", "or", "others", "out", "positive", "proctor", "range", "record", "rtrn", "select", "subtype", "type", "unit", "when", "while"};
    int count = 0;
    int i;
    int x;
    int y;

    for(i = 0; i < 38; i++)
    {
        if(currentCharacter == keywords[i][0])
        {
            for(x = 0; keywords[i][x] != '\0';)
            {
                if(currentCharacter == keywords[i][x])
                {
                    currentCharacter = fgetc(file);
                    count++;
                }
                else
                {
                    for(y = 0; y < count + 1; y++)
                        fseek(file, -1L, SEEK_CUR);
                    
                    currentCharacter = fgetc(file);
                    count = 0;
                    break;
                }

                if(keywords[i][++x] == '\0')
                {
                    printf("%s", keywords[i]);
                    fseek(file, -1L, SEEK_CUR);
                    return 1;
                }
            }
        }
    }
    
    return 0;
}

int isIdentifier(FILE *file)
{
    if(((int)(currentCharacter) >= 65 && (int)(currentCharacter) <= 90) || ((int)(currentCharacter) >= 97 && (int)(currentCharacter) <= 122))
    {
        while(((int)(currentCharacter) >= 65 && (int)(currentCharacter) <= 90) || ((int)(currentCharacter) >= 97 && (int)(currentCharacter) <= 122) || ((int)(currentCharacter) >= 48 && (int)(currentCharacter) <= 57) || currentCharacter == '_')
        {
            printf("%c", currentCharacter);
            currentCharacter = fgetc(file);
        }

        fseek(file, -1L, SEEK_CUR);
        return 1;
    }

    return 0;
}

int isCharLiteral(FILE *file)
{
    if(currentCharacter == '\'')
    {
        do
        {
            if(currentCharacter == '\\')
            {
                printf("%c", currentCharacter);
                currentCharacter = fgetc(file);
            }

            printf("%c", currentCharacter);
        }
        while((currentCharacter = fgetc(file)) != '\'');

        printf("%c", currentCharacter);
        return 1;
    }
    
    return 0;
}

int isOperator(FILE *file)
{
    char *doubleOperators[] = {":=", "..", "<<", ">>", "<>", "<=", ">=", "!=", "=>"};
    char singleOperators[] = {'.', '<', '>', '(', ')', '+', '-', '*', '/', ';', ',', '#', '[', ']', '='};
    char *doesContain;
    int i;

    for(i = 0; i < 9; i++)
    {
        if(currentCharacter == doubleOperators[i][0])
        {
            if((currentCharacter = fgetc(file)) == doubleOperators[i][1])
            {
                printf("%s", doubleOperators[i]);
                return 1;
            }
            else
            {
                fseek(file, -2L, SEEK_CUR);
                currentCharacter = fgetc(file);
            }
        }
    }
    
    if(doesContain = strchr(singleOperators, currentCharacter))
    {
        printf("%c", currentCharacter);
        return 1;
    }

    return 0;
}

int isNumLiteral(FILE *file)
{
    if((int)currentCharacter >= 48 && (int)currentCharacter <= 57)
    {
        while(((int)currentCharacter >= 48 && (int)currentCharacter <= 57) || ((int)currentCharacter >= 65 && (int)currentCharacter <= 70) || ((int)currentCharacter >= 97 && (int)currentCharacter <= 102) || currentCharacter == '_' || currentCharacter == '.' || currentCharacter == '#')
        {
            printf("%c", currentCharacter);
            currentCharacter = fgetc(file);
        }

        fseek(file, -1L, SEEK_CUR);
        return 1;
    }

    return 0;
}