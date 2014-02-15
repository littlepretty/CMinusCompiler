//
//  globals.h
//  CMinusCompiler
//
//  Created by Littlepretty Yan on 2/15/14.
//  Copyright (c) 2014 littlepretty. All rights reserved.
//

#ifndef CMinusCompiler_globals_h
#define CMinusCompiler_globals_h

typedef enum {
    
    END_FILE, ERROR,
    
    IF, ELSE, INT, RETURN, VOID, WHILE,
    
    PLUS, MINUS, MUL, DIV, LT, LE, GT, GE, EQ, NEQ, SEMI, COMMA, LPAREN, RPAREN,
    LBRACKET, RBRACKET, LBRACE, RBRACE,
    
    ID, NUMBER
    
} TokenType;

#define MAX_RESERVED_WORDS 6




typedef struct TreeNode {
    
    struct TreeNode* childrens[3];
    struct TreeNode* sibling;
    
    
    
    
    
} TreeNode;

















#endif
