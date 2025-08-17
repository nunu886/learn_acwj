#include "data.h"
#include "decl.h"
// Parsing of statements
// Copyright (c) 2019 Warren Toomey, GPL3

// statements: statement
//      | statement statements
//      ;
//
// statement: 'print' expression ';'
//            'int' identifier ';
//            identifier '=' expression ';'
// identifier: T_IDENT
//      ;

// Parse one or more statements
void statements(void) {
  while (1) {
    switch (Token.token) {
    case T_PRINT:
      print_statement();
      break;
    case T_INT:
      var_declaration();
      break;
    case T_IDENT:
      assignment_statement();
      break;
    case T_EOF:
      return;
    default:
      fatald("Syntax error, token", Token.token);
      break;
    }
  }
}
