// Global variables
// Copyright (c) 2019 Warren Toomey, GPL3
#ifndef DATA_H
#define DATA_H
#include "defs.h"
#include <stdio.h>
extern int Line;			// Current line number
extern int Putback;			// Character put back by scanner
extern FILE *Infile;			// Input and output files
extern FILE *Outfile;
extern struct token Token;		// Last token scanned
#define TEXTLEN 512
extern char Text[TEXTLEN + 1];		// Last identifier scanned

#define NSYMBOLS 1024
extern struct symtable Gsym[NSYMBOLS];
extern int Globs;
extern int FunctionId;
#endif
