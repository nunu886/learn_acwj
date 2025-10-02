#ifndef DEFS_H
#define DEFS_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure and enum definitions
// Copyright (c) 2019 Warren Toomey, GPL3

#define TEXTLEN 512 // Length of symbols in input

// Token types
enum {
  T_EOF,
  T_PLUS,
  T_MINUS,
  T_STAR,
  T_SLASH,
  T_EQ,
  T_NE, //== ,!=
  T_LT,
  T_GT,
  T_LE,
  T_GE, //<,>,<=,>=
  T_INTLIT,
  T_SEMI,
  T_PRINT,
  T_INT,
  T_EQUALS,
  T_IDENT,  // int, =, "var"
  T_LABRCE, //{ }
  T_RABRCE,
  T_LPAREN,
  T_RPAREN, //( )
  T_IF,
  T_ELSE,
  T_WHILE,
  T_FOR,
  T_VOID,
  T_CHAR,
};

enum {
  P_NONE,
  P_VOID,
  P_INT,
  P_CHAR,
};

// Token structure
struct token {
  int token;    // Token type, from the enum list above
  int intvalue; // For T_INTLIT, the integer value
};

// AST node types
enum {
  A_ADD = 1,
  A_SUBTRACT,
  A_MULTIPLY,
  A_DIVIDE,
  A_EQ,
  A_NE,
  A_LT,
  A_GT,
  A_LE,
  A_GE,
  A_INTLIT,
  A_LVIDENT,
  A_ASSIGN,
  A_IDENT,
  A_GLUE,
  A_IF,
  A_WHILE,
  A_PRINT,
  A_FUNCTION,
  A_WIDEN,
};

enum {
  S_VARIABLE,
  S_FUNCTION,
};

// Abstract Syntax Tree structure
struct ASTnode {
  int op;   // "Operation" to be performed on this tree
  int type; //
  int stype;
  struct ASTnode *left; // Left and right child trees
  struct ASTnode *mid;
  struct ASTnode *right;
  union {
    int intvalue; // For A_INTLIT, the integer value
    int id;       // For A_IDENT, the symbol slot number
  } v;
};

// Symbol table
struct symtable {
  char *name;
  int type;
  int stype;
};

#endif
