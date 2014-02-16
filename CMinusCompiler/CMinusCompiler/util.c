//
//  util.c
//  CMinusCompiler
//
//  Created by Littlepretty Yan on 2/16/14.
//  Copyright (c) 2014 littlepretty. All rights reserved.
//

#include <stdio.h>
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

















