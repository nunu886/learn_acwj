#include "decl.h"
#include "data.h"
#include "defs.h"

void var_declaration() {
  match(T_INT, "int");
  ident();

  addglob(Text);
  genglobsym(Text);
  semi();
}

struct ASTnode *function_declaration() {
  struct ASTnode *tree = NULL;

  int nameslot = 0;
  match(T_VOID, "void");
  ident();
  nameslot = addglob(Text);

  lparen();
  rparen();

  tree = compound_statement();

  return mkastunary(A_FUNCTION, tree, nameslot);
}
