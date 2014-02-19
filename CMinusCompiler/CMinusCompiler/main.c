//
//  main.c
//  CMinusCompiler
//
//  Created by Littlepretty Yan on 2/15/14.
//  Copyright (c) 2014 littlepretty. All rights reserved.
//

#include <stdio.h>
#include <string.h>


#include "scan.h"
#include "parse.h"
#include "globals.h"


int LineNumber = 0;

FILE *Source;
FILE *Listing;
FILE *Code;

int EchoSource = TRUE;
int TraceScan = TRUE;
int TraceParse = TRUE;
int GlobalError = FALSE;


int setupFromCommandParameters(const char *argv[])
{
    char sourceFileName[256];
    
    if (argv[1] != NULL) {
        strcpy(sourceFileName, argv[1]);
        Source = fopen(sourceFileName, "r");
        if (Source == NULL) {
            fprintf(Listing, "Error: Cannot Open Source File %s", sourceFileName);
            return FALSE;
        }
    }
    
    Listing = stdout;
    
    fprintf(Listing, "C- COMPILATION: %s\n", sourceFileName);
    
    return TRUE;
}

#define NO_PARSE 1

int main(int argc, const char * argv[])
{
    if (argc < 2) {
        fprintf(Listing, "C- Compiler Usage: %s <filename>\n", argv[0]);
    }
    
    if (setupFromCommandParameters(argv)) {
        
#if NO_PARSE
        while (getNextToken() != END_FILE) {
            
        }
        
#else
        TreeNode *syntaxTree = parse();
        if (syntaxTree != NULL) {
            fprintf(listing, "\nSyntax Tree: \n");
            printTree(syntaxTree);
        }
#endif
        
        
        
    } else {
        exit(1);
    }
    
    return 0;
}












