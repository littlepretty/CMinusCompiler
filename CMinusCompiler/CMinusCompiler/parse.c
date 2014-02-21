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
static TreeNode *simple_expression(TreeNode *passdown);
static TreeNode *additive_expression(TreeNode *passdown);
static TreeNode *term(TreeNode *passdown);
static TreeNode *factor(TreeNode *passdown);

static TreeNode *identifier_stmt();
//static TreeNode *var(TreeNode *passdown);
//static TreeNode *call();

static TreeNode *args();
static TreeNode *arg_list();








static TreeNode *expression_stmt()
{
    TreeNode *tree = NULL;
    if (token != SEMI) {
        tree = expression();
    }
    match(SEMI);
    return tree;
}

static TreeNode *expression()
{
    TreeNode *tree = NULL;
    TreeNode *leftValue = NULL;
    
    if (token == ID) {
        leftValue = identifier_stmt();
        if (leftValue != NULL && token == ASSIGN) {
            tree = newExprNode(AssignK);
            if (tree != NULL) {
                tree->childrens[0] = leftValue;
                match(ASSIGN);
                tree->childrens[1] = expression();
            }
        } else {
            tree = simple_expression(leftValue);
        }
    }
    
    return tree;
}

static TreeNode *simple_expression(TreeNode *passdown)
{
    TreeNode *tree = additive_expression(passdown);
    
    if (token == LT || token == LE || token == GT || token == GE
           || token == EQ || token == NEQ) {
        TreeNode *ptr = newExprNode(OpK);
        if (ptr != NULL) {
            ptr->attribute.op = token;
            ptr->childrens[0] = tree;
            tree = ptr;
        }
        match(token);
        if (tree != NULL) {
            tree->childrens[1] = additive_expression(NULL);
        }
    }
    return tree;
}

static TreeNode *additive_expression(TreeNode *passdown)
{
    TreeNode *tree = term(passdown);
    while (token == PLUS || token == MINUS) {
        TreeNode *ptr = newExprNode(OpK);
        if (ptr != NULL) {
            ptr->attribute.op = token;
            ptr->childrens[0] = tree;
            tree = ptr;
        }
        match(token);
        if (tree != NULL) {
            tree->childrens[1] = term(NULL);
        }
    }
    return tree;
}

static TreeNode *term(TreeNode *passdown)
{
    TreeNode *tree = factor(passdown);
    while (token == MUL || token == DIV) {
        TreeNode *ptr = newExprNode(OpK);
        if (ptr != NULL) {
            ptr->childrens[0] = tree;
            ptr->attribute.op = token;
            tree = ptr;
        }
        match(token);
        if (tree != NULL) {
            tree->childrens[1] = factor(NULL);
        }
    }
    return tree;
}

static TreeNode *factor(TreeNode *passdown)
{
    TreeNode *tree = NULL;
    
    if (passdown != NULL) {
        return passdown;
    }
    switch (token) {
        case LPAREN:
            match(LPAREN);
            tree = expression();
            match(RPAREN);
            break;
        case NUMBER:
            tree = newExprNode(ConstK);
            if (tree != NULL) {
                tree->attribute.val = atoi(tokenString);
                match(NUMBER);
            }
            break;
        case ID:
            tree = identifier_stmt();
            break;
        default:
            syntaxError("Unexpected Token: ");
            printToken(token, tokenString);
            GlobalError = TRUE;
            break;
    }
    
    return tree;
}

static TreeNode *identifier_stmt()
{
    TreeNode *tree;
    char* identiferName = copyString(tokenString);
    match(ID);
    
    if (token == LPAREN) {
        tree = newExprNode(CallK);
        if (tree != NULL) {
            match(LPAREN);
            tree->childrens[0] = arg_list();
            match(RPAREN);
            tree->attribute.name = identiferName;
        }
        
    } else {
        tree = newExprNode(IdK);
        if (tree != NULL) {
            if (token == LBRACKET) {
                match(LBRACKET);
                tree->childrens[0] = expression();
                match(RBRACKET);
            }
            tree->attribute.name = identiferName;
        }
    }
    
    return tree;
}
static TreeNode *args()
{
    TreeNode *tree = NULL;
    /**
     *	follows(args) = { ')' }
     */
    if (token != RPAREN) {
        tree = arg_list();
    }
    return tree;
}
static TreeNode *arg_list()
{
    TreeNode *tree = expression();
    TreeNode *iter = tree;
    
    while (token != SEMI) {
        match(COMMA);
        TreeNode *ptr = expression();
        if (ptr != NULL) {
            if (tree != NULL) {
                iter->sibling = ptr;
                iter = ptr;
            } else {
                tree = iter = ptr;
            }
        }
    }
    return tree;
}






































