#include "decl.h"
#include "data.h"
#include "defs.h"
#include <stdio.h>

int parse_type(int type) {
  switch (type) {
  case T_VOID:
    return P_VOID;
  case T_INT:
    return P_INT;
  case T_CHAR:
    return P_CHAR;
  default:
    fatald("rllegal type, token", type);
  }

  return P_NONE;
}

void var_declaration() {

  // match(T_INT, "int");
  // match(T_CHAR, "char");
  int type = parse_type(Token.token);
  scan(&Token);
  ident();

  int id = addglob(Text, type, S_VARIABLE);
  genglobsym(id);
  semi();
}

struct ASTnode *function_declaration() {
  struct ASTnode *tree = NULL;

  int nameslot = 0;
  match(T_VOID, "void");
  // int type = parse_type(Token.token);
  ident();
  nameslot = addglob(Text, P_VOID, S_FUNCTION);

  lparen();
  rparen();

  tree = compound_statement();

  return mkastunary(A_FUNCTION, P_VOID, tree, nameslot);
}
