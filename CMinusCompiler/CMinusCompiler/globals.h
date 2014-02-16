//
//  globals.h
//  CMinusCompiler
//
//  Created by Littlepretty Yan on 2/15/14.
//  Copyright (c) 2014 littlepretty. All rights reserved.
//

#ifndef CMinusCompiler_globals_h
#define CMinusCompiler_globals_h


#include <stdio.h>
#include <stdlib.h>

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif


extern int LineNumber;

extern FILE *Source;
extern FILE *Listing;
extern FILE *Code;

extern int EchoSource;
extern int TraceScan;
extern int TraceParse;

extern int GlobalError;

typedef enum {
    
    END_FILE, ERROR,
    
    IF, ELSE, INT, RETURN, VOID, WHILE,
    
    PLUS, MINUS, MUL, DIV, ASSIGN, LT, LE, GT, GE, EQ, NEQ, SEMI, COMMA, LPAREN, RPAREN,
    LBRACKET, RBRACKET, LBRACE, RBRACE,
    
    ID, NUMBER
    
} TokenType;

#define MAX_RESERVED_WORDS 6




typedef struct TreeNode {
    
    struct TreeNode* childrens[3];
    struct TreeNode* sibling;
    
    
    
    
    
} TreeNode;

















#endif
