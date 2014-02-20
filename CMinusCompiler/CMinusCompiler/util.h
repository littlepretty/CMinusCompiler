//
//  util.h
//  CMinusCompiler
//
//  Created by Littlepretty Yan on 2/16/14.
//  Copyright (c) 2014 littlepretty. All rights reserved.
//

#ifndef CMinusCompiler_util_h
#define CMinusCompiler_util_h

#include "globals.h"

void printToken(TokenType token, char *string);

TreeNode *newStmtNode(StmtKind kind);
TreeNode *newExprNode(ExprKind kind);
TreeNode *newDeclNode(DeclKind kind);

char *copyString(const char* src);

void printSyntaxTree(TreeNode *tree);

#endif
