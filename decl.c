#include "decl.h"
#include "data.h"
#include "defs.h"

int parse_type(int type) {
  int newtype = P_NONE;
  switch (type) {
  case T_VOID:
    newtype = P_VOID;
    break;
  case T_INT:
    newtype = P_INT;
    break;
  case T_CHAR:
    newtype = P_CHAR;
    break;
  default:
    fatald("rllegal type, token", type);
  }

  while (1) {
    scan(&Token);
    if (Token.token != T_STAR) {
      break;
    }
    newtype = pointer_to(newtype);
  }
  return newtype;
}

void var_declaration() {

  // match(T_INT, "int");
  // match(T_CHAR, "char");
  int type = parse_type(Token.token);
  ident();

  int id = addglob(Text, type, S_VARIABLE, 0);
  genglobsym(id);
  semi();
}

struct ASTnode *function_declaration() {
  struct ASTnode *tree = NULL;

  // match(T_VOID, "void");
  int type = parse_type(Token.token);

  int endlabel = label();
  ident();
  int nameslot = addglob(Text, type, S_FUNCTION, endlabel);
  FunctionId = nameslot;

  lparen();
  rparen();

  tree = compound_statement();
  if (type != P_VOID) {
    struct ASTnode *finalstmt = NULL;
    if (tree->op == A_GLUE) {
      finalstmt = tree->right;
    } else {
      finalstmt = tree;
    }
    if (finalstmt == NULL || finalstmt->op != A_RETURN) {
      fatal("no return for function with non-void type.");
    }
  }

  return mkastunary(A_FUNCTION, type, tree, nameslot);
}
