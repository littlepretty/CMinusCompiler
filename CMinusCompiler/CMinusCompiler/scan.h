//
//  scan.h
//  CMinusCompiler
//
//  Created by Littlepretty Yan on 2/15/14.
//  Copyright (c) 2014 littlepretty. All rights reserved.
//

#ifndef CMinusCompiler_scan_h
#define CMinusCompiler_scan_h

#include "globals.h"

TokenType getNextToken();

#define MAX_TOKEN_LENGTH 64

char tokenString[MAX_TOKEN_LENGTH];

#endif
