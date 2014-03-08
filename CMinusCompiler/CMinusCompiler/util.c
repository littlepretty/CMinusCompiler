//
//  util.c
//  CMinusCompiler
//
//  Created by Littlepretty Yan on 2/16/14.
//  Copyright (c) 2014 littlepretty. All rights reserved.
//

#include <stdio.h>
#include <string.h>

#include "util.h"
#include "globals.h"

void printToken(TokenType token, char *string)
{
    switch (token) {
        case IF:
        case ELSE:
        case INT:
        case RETURN:
        case WHILE:
        case VOID:
            fprintf(Listing, "Reserved Word: %s",string);
            break;
        case ID:
            fprintf(Listing, "ID: name = %s", string);
            break;
        case NUMBER:
            fprintf(Listing, "NUMBER: value = %s", string);
            break;
        case ERROR:
            fprintf(Listing, "ERROR: %s", string);
            break;
        case ASSIGN:
            fprintf(Listing, "=");
            break;
        case PLUS:
            fprintf(Listing, "+");
            break;
        case MINUS:
            fprintf(Listing, "-");
            break;
        case MUL:
            fprintf(Listing, "*");
            break;
        case DIV:
            fprintf(Listing, "/");
            break;
        case LPAREN:
            fprintf(Listing, "(");
            break;
        case RPAREN:
            fprintf(Listing, ")");
            break;
        case LBRACKET:
            fprintf(Listing, "[");
            break;
        case RBRACKET:
            fprintf(Listing, "]");
            break;
        case LBRACE:
            fprintf(Listing, "{");
            break;
        case RBRACE:
            fprintf(Listing, "}");
            break;
        case EQ:
            fprintf(Listing, "==");
            break;
        case NEQ:
            fprintf(Listing, "!=");
            break;
        case GT:
            fprintf(Listing, ">");
            break;
        case GE:
            fprintf(Listing, ">=");
            break;
        case LT:
            fprintf(Listing, "<");
            break;
        case LE:
            fprintf(Listing, "<=");
            break;
        case SEMI:
            fprintf(Listing, ";");
            break;
        case COMMA:
            fprintf(Listing, ",");
        default:
            break;
    }
    fprintf(Listing, "\n");
}

TreeNode *allocateNode()
{
    TreeNode *t = malloc(sizeof(TreeNode));
    
    if (t == NULL) {
        fprintf(Listing, "Cannot Allocate Memory at Line %d", LineNumber);
    } else {
        for (int i = 0 ; i < MAX_NODE_CHILDREN; ++i) {
            t->childrens[i] = NULL;
        }
        t->sibling = NULL;
        t->lineNumber = LineNumber;
    }
    
    return t;
}

TreeNode *newStmtNode(StmtKind kind)
{
    TreeNode *t = allocateNode();
    if (t != NULL) {
        t->nodeKind = StmtK;
        t->kind.stmtKind = kind;
    }
    return t;
}

TreeNode *newExprNode(ExprKind kind)
{
    TreeNode *t = allocateNode();
    if (t != NULL) {
        t->nodeKind = ExprK;
        t->kind.exprKind = kind;
    }
    return t;
}

TreeNode *newDeclNode(DeclKind kind)
{
    TreeNode *t = allocateNode();
    if (t != NULL) {
        t->nodeKind = DeclK;
        t->kind.declKind = kind;
    }
    return t;
}

char *copyString(const char* src)
{
    size_t len = strlen(src);
    
    char *dst = (char *)malloc(len * sizeof(char));
    if (dst != NULL) {
        strcpy(dst, src);
    } else {
        fprintf(Listing, "Cannot Allocate Memory When Copying String");
        exit(1);
    }
    return dst;
}

static int indent_number = 0;

static void INDENT()
{
    indent_number += 4;
}

static void UNINDENT()
{
    indent_number -= 4;
}

static void printIndent()
{
    for (int i = 0; i < indent_number; ++i) {
        fprintf(Listing, " ");
    }
}

char *dataTypeName(ExprType type)
{
    static char i[] = "int";
    static char v[] = "void";
    static char invalid[] = "<<invalid type>>";
    
    switch (type) {
        case Integer:
        case Boolean:
            return i;
            break;
        case Void:
            return v;
        default:
            return invalid;
            break;
    }
    
}

void printSyntaxTree(TreeNode *tree)
{
    INDENT();
    
    while (tree != NULL) {
        printIndent();
        
        if (tree->nodeKind == DeclK) {
            switch (tree->kind.declKind) {
                case ScalarDeclK:
                    fprintf(Listing, "[%s Type Scalar Declaration: %s]\n", tree->attribute.name, dataTypeName(tree->varDataType));
                    break;
                case ArrayDeclK:
                    fprintf(Listing, "[%s Type %d Size Array Declaration: %s]\n", tree->attribute.name, tree->arrayDim, dataTypeName(tree->varDataType));
                    break;
                case FunDeclK:
                    fprintf(Listing, "[%s Return Type Function Declaration: %s]\n", tree->attribute.name, dataTypeName(tree->varDataType));
                    break;
                default:
                    fprintf(Listing, "[Unknown Declaration\n]");
                    break;
            }
            
        } else if (tree->nodeKind == ExprK) {
            switch (tree->kind.exprKind) {
                case OpK:
                    fprintf(Listing, "[Operator: ");
                    printToken(tree->attribute.op, "");
                    fprintf(Listing, "]\n");
                    break;
                case ConstK:
                    fprintf(Listing, "[Constant: %d]\n", tree->attribute.val);
                    break;
                case CallK:
                    fprintf(Listing, "[Function Call: %s()]\n", tree->attribute.name);
                    break;
                case AssignK:
                    fprintf(Listing, "[Assignment]\n");
                    break;
                case IdK:
                    fprintf(Listing, "[Identifier: %s", tree->attribute.name);
                    if (tree->arrayDim != 0) {
                        fprintf(Listing, "[ %d ]", tree->arrayDim);
                    }
                    fprintf(Listing, "]\n");
                    break;
                default:
                    fprintf(Listing, "[Unknown Expression\n]");
                    break;
            }
        } else if (tree->nodeKind == StmtK) {
            switch (tree->kind.stmtKind) {
                case IfK:
                    fprintf(Listing, "[If Stmt]\n");
                    break;
                case WhileK:
                    fprintf(Listing, "[While Stmt]\n");
                    break;
                case ReturnK:
                    fprintf(Listing, "[Return Stmt]\n");
                    break;
                case CompK:
                    fprintf(Listing, "[Compound Stmt]\n");
                    break;
                default:
                    fprintf(Listing, "[Unknown Stmt\n]");
                    break;
            }
        } else {
            fprintf(Listing, "[Unknown Node\n]");
        }
        
        for (int i = 0 ; i < MAX_NODE_CHILDREN; ++i) {
            printSyntaxTree(tree->childrens[i]);
        }
        
        tree = tree->sibling;
        
    }
    
    UNINDENT();
}















