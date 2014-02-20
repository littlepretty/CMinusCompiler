//
//  scan.c
//  CMinusCompiler
//
//  Created by Littlepretty Yan on 2/16/14.
//  Copyright (c) 2014 littlepretty. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "scan.h"
#include "util.h"


#define MAX_BUFFER_SIZE 256

static int LinePosition = 0;
static size_t BufferSize = 0;
static char Buffer[MAX_BUFFER_SIZE];
static int EndFileFlag = FALSE;


char getNextChar()
{
    if (LinePosition >= BufferSize) {
        LineNumber++;
        
        if (fgets(Buffer, MAX_BUFFER_SIZE + 1, Source)) {
            if (EchoSource) {
                fprintf(Listing, "%d: %s", LineNumber, Buffer);
            }
            BufferSize = strlen(Buffer);
            LinePosition = 0;
            return Buffer[LinePosition++];
            
        } else {
            EndFileFlag = TRUE;
            return EOF;
        }
        
    } else {
        return Buffer[LinePosition++];
    }
    
}

void unGetNextChar()
{
    if (!EndFileFlag) {
        LinePosition--;
    }
}

typedef enum  {
    START, IN_NUM, IN_ID,
    BEFORE_COMMENT, IN_COMMENT, AFTER_COMMENT,
    IN_RE_OP,
    DONE
} StateType;

static struct {
    char *str;
    TokenType token;
} reservedWords[MAX_RESERVED_WORDS] = {
    
    {"if", IF}, {"else", ELSE}, {"while", WHILE}, {"return", RETURN}, {"void", VOID},
    {"int", INT}
};

TokenType lookUpReservedWords(char *string)
{
    for (int i = 0; i < MAX_RESERVED_WORDS; i++) {
        if (strcmp(string, reservedWords[i].str) == 0) {
            return reservedWords[i].token;
        }
    }
    return ID;
}

TokenType getNextToken()
{
    int tokenStringIndex = 0;
    TokenType currentToken;
    StateType currentState = START;
    
    int save;
    char reOpPefix;
    
    while (currentState != DONE) {
        
        char currentChar = getNextChar();
        save = TRUE;
        
        switch (currentState) {
            case START:
                
                if (currentChar == ' ' || currentChar == '\t' || currentChar == '\n') {
                    /**
                     *	skip all whitespaces
                     */
                    save = FALSE;
                    currentState = START;
                    
                } else if (isdigit(currentChar)) {
                    /**
                     *	met a digit
                     */
                    currentState = IN_NUM;
                    
                } else if (isalpha(currentChar)) {
                    /**
                     *	met a letter
                     */
                    currentState = IN_ID;
                    
                } else if (currentChar == '/') {
                    currentState = BEFORE_COMMENT;
                    save = FALSE;
                    
                } else if (currentChar == '=' || currentChar == '>'
                           || currentChar == '<' || currentChar == '!') {
                    currentState = IN_RE_OP;
                    reOpPefix = currentChar;
                    
                } else {
                    
                    /**
                     *	single character tokens
                     */
                    currentState = DONE;
                    if (currentChar == EOF) {
                        currentToken = END_FILE;
                        save = FALSE;
                    } else if (currentChar == '+') {
                        currentToken = PLUS;
                    } else if (currentChar == '-') {
                        currentToken = MINUS;
                    } else if (currentChar == '*') {
                        currentToken = MUL;
                    } else if (currentChar == '(') {
                        currentToken = LPAREN;
                    } else if (currentChar == ')') {
                        currentToken = RPAREN;
                    } else if (currentChar == '[') {
                        currentToken = LBRACKET;
                    } else if (currentChar == ']') {
                        currentToken = RBRACKET;
                    } else if (currentChar == '{') {
                        currentToken = LBRACE;
                    } else if (currentChar == '}') {
                        currentToken = RBRACE;
                    } else if (currentChar == ',') {
                        currentToken = COMMA;
                    } else if (currentChar == ';') {
                        currentToken = SEMI;
                    }
                }
                
                break;
                
            case BEFORE_COMMENT:
                if (currentChar == '*') {
                    currentState = IN_COMMENT;
                    save = FALSE;
                } else {
                    /**
                     *	div operator
                     */
                    unGetNextChar();
                    currentToken = DIV;
                    currentState = DONE;
                }
                
                break;
            case IN_COMMENT:
                if (currentChar == '*') {
                    currentState = AFTER_COMMENT;
                } else {
                    currentState = IN_COMMENT;
                }
                save = FALSE;
                break;
                
            case AFTER_COMMENT:
                if (currentChar == '/') {
                    currentState = START;
                } else {
                    currentState = IN_COMMENT;
                }
                save = FALSE;
                break;
                
            case IN_NUM:
                if (!isdigit(currentChar)) {
                    unGetNextChar();
                    currentState = DONE;
                    save = FALSE;
                    currentToken = NUMBER;
                }
                break;
                
            case IN_ID:
                if (!isalnum(currentChar) && currentChar != '_') {
                    unGetNextChar();
                    currentState = DONE;
                    save = FALSE;
                    currentToken = ID;
                }
                break;
            
            case IN_RE_OP:
                if (currentChar == '=') {
                    if (reOpPefix == '!') {
                        currentToken = NEQ;
                    } else if (reOpPefix == '=') {
                        currentToken = EQ;
                    } else if (reOpPefix == '>') {
                        currentToken = GE;
                    } else if (reOpPefix == '<') {
                        currentToken = LE;
                    }
                } else {
                    unGetNextChar();
                    if (reOpPefix == '=') {
                        currentToken = ASSIGN;
                    } else if (reOpPefix == '>') {
                        currentToken = GT;
                    } else if (reOpPefix == '<') {
                        currentToken = LT;
                    }
                    save = FALSE;
                }
                currentState = DONE;
                
                break;
                
            default:
                fprintf(Listing, "Scan Bug: state = %d\n", currentState);
                currentState = DONE;
                currentToken = ERROR;
                break;
        }
        
        
        if (save && tokenStringIndex < MAX_TOKEN_LENGTH) {
            tokenString[tokenStringIndex++] = currentChar;
        }
        if (currentState == DONE ) {
            tokenString[tokenStringIndex] = '\0';
            if (currentToken == ID) {
                currentToken = lookUpReservedWords(tokenString);
            }
        }
        
    }
    
    if (TraceScan) {
        fprintf(Listing, "\t%d: ", LineNumber);
        printToken(currentToken, tokenString);
    }

    return currentToken;
}









