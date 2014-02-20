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







































