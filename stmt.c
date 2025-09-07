#include "decl.h"
#include "defs.h"
// Parsing of statements
// Copyright (c) 2019 Warren Toomey, GPL3

// statements: statement
//      | statement statements
//      ;
//
// statement: 'print' expression ';'
//            'int' identifier ';
//            identifier '=' expression ';'
//            | if_statement;
// identifier: T_IDENT
//      ;
// if_statement: 'if' '(' true_or_false_expression ')''{' (statement |
// statements)* '}'
//                ['else' '{' (statement | statements)* '}']

// Parse one or more statements
struct ASTnode *statements() { return compound_statement(); }
