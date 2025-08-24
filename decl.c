#include "decl.h"
#include "data.h"

void var_declaration() {
  match(T_INT, "int");
  ident();

  addglob(Text);
  genglobsym(Text);
  semi();
}

void print_statement(void) {
  struct ASTnode *tree = NULL;
  int reg = 0;

  // Match a 'print' as the first token
  match(T_PRINT, "print");

  // Parse the following expression and
  // generate the assembly code
  tree = binexpr(0);
  reg = genAST(tree, -1);
  // printf("<<<< %d \n", reg);
  genprintint(reg);
  genfreeregs();

  // Match the following semicolon
  semi();
}

// identifier '=' expression ';'
void assignment_statement() {
  struct ASTnode *left = NULL;
  struct ASTnode *right = NULL;
  struct ASTnode *tree = NULL;

  ident();
  int id = findglob(Text);
  if (id == -1) {
    fatals("Undeclared variable", Text);
  }
  right = mkastleaf(A_LVIDENT, 0);
  match(T_EQUALS, "=");

  left = binexpr(0);

  tree = mkastnode(A_ASSIGN, left, right, 0);
  // generate the assembly code for the assignment
  genAST(tree, -1);
  genfreeregs();

  semi();
}
