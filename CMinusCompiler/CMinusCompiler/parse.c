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

void reportSuntaxError(void)
{
    syntaxError("Unexpected Token: ");
    printToken(token, tokenString);
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

/**
 *	forward declaration for recursive call
 */
static TreeNode *declaration_list();
static TreeNode *declaration();
static TreeNode *var_declaration();
static TreeNode *fun_declaration();
static TreeNode *params();
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

static TreeNode *args();
static TreeNode *arg_list();


TreeNode *parse()
{
    token = getNextToken();
    TreeNode *tree = declaration_list();
    if (token != END_FILE) {
        syntaxError("Unexpected Symbol at End of File");
    }
    return tree;
}


static TreeNode *declaration_list()
{
    TreeNode *tree = declaration();
    TreeNode *temp = tree;
    
    while (token != SEMI && token != RBRACE ) {
        TreeNode *ptr = declaration();
        temp->sibling = ptr;
        temp = ptr;
    }
    
    return tree;
    
}
static TreeNode *declaration()
{
    TreeNode *tree = NULL;
    
    //TODO: declaration implementation
    if (token == VOID || token == INT) {
        match(token);
    }
    char *name = copyString(tokenString);
    match(ID);
    
    if (token == SEMI) {
        match(SEMI);
        tree = newDeclNode(VarDeclK);
        tree->attribute.name = copyString(name);
    } else if (token == LBRACKET) {
        match(LBRACKET);
        tree = newDeclNode(VarDeclK);
        tree->attribute.name = copyString(name);
        TreeNode *num = newExprNode(ConstK);
        num->attribute.val = atoi(tokenString);
        tree->childrens[0] = num;
        match(NUMBER);
        match(RBRACKET);
    } else if (token == LPAREN) {
        match(LPAREN);
        tree = newDeclNode(FunDeclK);
        tree->attribute.name = copyString(name);
        tree->childrens[0] = params();
        match(RPAREN);
        tree->childrens[1] = compound_stmt();
    }
    
    return tree;
}
static TreeNode *var_declaration()
{
    TreeNode *tree = newDeclNode(VarDeclK);

    if (token == INT || token == VOID) {
        match(token);
    }
    
    tree->attribute.name = copyString(tokenString);
    match(ID);
    
    if (token == SEMI) {
        match(SEMI);
    } else {
        match(LBRACKET);
        TreeNode *num = newExprNode(ConstK);
        num->attribute.val = atoi(tokenString);
        tree->childrens[0] = num;
        match(NUMBER);
        match(RBRACKET);
    }
    
    return tree;
}
static TreeNode *fun_declaration()
{
    TreeNode *tree = newDeclNode(FunDeclK);
    
    if (token == INT || token == VOID) {
        match(token);
    }
    
    if (tree != NULL) {
        TreeNode *name = newExprNode(IdK);
        name->attribute.name = copyString(tokenString);
        tree->childrens[0] = name;
        match(ID);
        
        match(LPAREN);
        tree->childrens[1] = params();
        match(RPAREN);
        
        tree->childrens[2] = compound_stmt();
    }
    
    return tree;
}

static TreeNode *params()
{
    TreeNode *tree = NULL;
    
    if (token != VOID) {
        tree = param_list();
    }
    
    return tree;
}

static TreeNode *param_list()
{
    TreeNode *tree = param();
    TreeNode *temp = tree;
    
    while (tree !=NULL && token == COMMA) {
        match(COMMA);
        TreeNode *ptr = param();
        temp->sibling = ptr;
        temp = ptr;
    }
    
    return tree;
}
static TreeNode *param()
{
    TreeNode *tree = newExprNode(IdK);
    
    if (token == INT || token == VOID) {
        match(token);
    }
    tree->attribute.name = copyString(tokenString);
    match(ID);
    if (token == LBRACKET) {
        match(LBRACKET);
        match(RBRACKET);
    }
    return tree;
}

static TreeNode *compound_stmt()
{
    TreeNode *tree = newStmtNode(CompK);
    
    match(LBRACE);
    tree->childrens[0] = local_var_declaration();
    tree->childrens[1] = stmt_list();
    match(RBRACE);
    
    return tree;
}
static TreeNode *local_var_declaration()
{
    TreeNode *tree = NULL;
    if (token == INT || token == VOID) {
        tree = var_declaration();
    }

    if (tree != NULL) {
        
        TreeNode *temp = tree;
        
        while (token == INT || token == VOID) {
            TreeNode *ptr = var_declaration();
            temp->sibling = ptr;
            temp = ptr;
        }
    }
    return tree;
}

static TreeNode *stmt_list()
{
    TreeNode *tree = NULL;
    if (token != RBRACE) {
        tree = stmt();
        TreeNode *temp = tree;
        
        while (token != RBRACE) {
            TreeNode *ptr = stmt();
            temp->sibling = ptr;
            temp = ptr;
        }
    }
    
    return tree;
}


static TreeNode *stmt()
{
    TreeNode *tree = NULL;
    
    switch (token) {
        case IF:
            tree = selection_stmt();
            break;
        case WHILE:
            tree = iteration_stmt();
            break;
        case RETURN:
            tree = return_stmt();
            break;
        case LBRACE:
            tree = compound_stmt();
            break;
        case ID:
        case SEMI:
        case LPAREN:
        case NUMBER:
            tree = expression_stmt();
            break;
        default:
            reportSuntaxError();
            break;
    }
    
    return tree;
}

static TreeNode *selection_stmt()
{
    match(IF);
    match(LPAREN);
    TreeNode *tree = newStmtNode(IfK);
    if (tree != NULL) {
        tree->childrens[0] = expression();
    }
    match(RPAREN);
    if (tree != NULL) {
        tree->childrens[1] = stmt();
    }
    if (token == ELSE) {
        match(ELSE);
        tree->childrens[2] = stmt();
    }
    return tree;
}

static TreeNode *iteration_stmt()
{
    TreeNode *tree = newStmtNode(WhileK);
    match(WHILE);
    match(LPAREN);
    if (tree != NULL) {
        tree->childrens[0] = expression();
    }
    match(RPAREN);
    if (tree != NULL) {
        tree->childrens[1] = stmt();
    }
    
    return tree;
}

static TreeNode *return_stmt()
{
    match(RETURN);
    TreeNode *tree = newStmtNode(ReturnK);
    if (tree != NULL) {
        if (token != SEMI) {
            tree->childrens[0] = expression();
        }
    }
    match(SEMI);
    return tree;
}

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
            reportSuntaxError();
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






































