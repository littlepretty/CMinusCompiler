//
//  parse.c
//  CMinusCompiler
//
//  Created by Littlepretty Yan on 2/19/14.
//  Copyright (c) 2014 littlepretty. All rights reserved.
//

#include <stdio.h>
#include "parse.h"
#include "globals.h"
#include "scan.h"
#include "util.h"


/**
 *	get token from scanner
 */
static TokenType token;

static void syntaxError(char *message)
{
    fprintf(Listing, ">>>\n");
    fprintf(Listing, "Syntax Error at Line %d: %s", LineNumber, message);
    GlobalError = TRUE;
}

static void match(TokenType expected)
{
    if (token == expected) {
        getNextToken();
    } else {
        syntaxError("Unexpected Token -> ");
        printToken(token, tokenString);
        fprintf(Listing, "\t\t");
    }
}

static TreeNode *declaration_list();
static TreeNode *declaration();
static TreeNode *var_declaration();
static TreeNode *fun_declaration();
static TreeNode *param_list();
static TreeNode *param();

static TreeNode *compound_stmt();
static TreeNode *local_var_declaration();

static TreeNode *stmt_list();
static TreeNode *stmt();
static TreeNode *selection_stmt();
static TreeNode *iteration_stmt();
static TreeNode *return_stmt();
static TreeNode *expression_stmt();

static TreeNode *expression();
static TreeNode *var();
static TreeNode *simple_expression();
static TreeNode *additive_expression();
static TreeNode *term();
static TreeNode *factor();
static TreeNode *call();
static TreeNode *arg_list();


static TreeNode *arg_list()
{
    TreeNode *t = expression();
    TreeNode *temp = t;
    
    while (token != SEMI) {
        match(COMMA);
        TreeNode *p = expression();
        if (p != NULL) {
            if (t != NULL) {
                temp->sibling = p;
                temp = p;
            } else {
                t = temp = p;
            }
        }
    }
    return t;
}

static TreeNode *call()
{
    TreeNode *t = newExprNode(CallK);
    
    match(ID);
    match(LPAREN);
    
    if (token != RPAREN) {
        t->childrens[0] = arg_list();
    }
    match(RPAREN);
    
    return t;
}

static TreeNode *factor()
{
    TreeNode *t = NULL;
    
    switch (token) {
        case LPAREN:
            match(LPAREN);
            t = expression();
            match(RPAREN);
            break;
        case NUMBER:
            t = newExprNode(ConstK);
            if (t != NULL) {
                t->attribute.val = atoi(tokenString);
                match(NUMBER);
            }
            break;
        case ID:
            
            
            break;
        default:
            break;
    }
    
    return t;
}


































