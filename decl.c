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
    break;
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

void var_declaration(int type) {

  // match(T_INT, "int");
  // match(T_CHAR, "char");
  int id = 0;
  while (1) {
    id = addglob(Text, type, S_VARIABLE, 0);
    genglobsym(id);

    // if current is `;` or `,`
    if (Token.token == T_SEMI) {
      semi();
      return;
    } else if (Token.token == T_COMMA) {
      scan(&Token);
      ident();
      continue;
    } else {
      fatald("missing, or ; after identifier.", Token.token);
    }
  }
}

struct ASTnode *function_declaration(int type) {
  struct ASTnode *tree = NULL;

  // match(T_VOID, "void");
  int endlabel = label();
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

void global_declarations() {
  struct ASTnode *tree = NULL;
  int type = P_NONE;

  // int a; or int func(){}
  while (1) {
    type = parse_type(Token.token);
    ident(); // match identifier

    // current token "("
    if (Token.token == T_LPAREN) {
      tree = function_declaration(type);
      genAST(tree, -1, 0);
    } else {
      var_declaration(type);
    }

    if (Token.token == T_EOF) {
      break;
    }
  }
}
