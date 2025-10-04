#include "data.h"
#include "decl.h"
#include "defs.h"
#include <stdio.h>

// Parsing of expressions
// Copyright (c) 2019 Warren Toomey, GPL3

// Parse a primary factor and return an
// AST node representing it.
struct ASTnode *primary() {
  struct ASTnode *n;
  int id;
  // For an INTLIT token, make a leaf AST node for it
  // and scan in the next token. Otherwise, a syntax error
  // for any other token type.
  switch (Token.token) {
  case T_INTLIT:
    if (Token.intvalue >= 0 && Token.intvalue <= 255) {
      n = mkastleaf(A_INTLIT, P_CHAR, Token.intvalue);
    } else {
      n = mkastleaf(A_INTLIT, P_INT, Token.intvalue);
    }
    break;
  case T_IDENT:
    id = findglob(Text);
    if (id == -1) {
      fatals("Unknown variable", Text);
    }
    scan(&Token);
    if (Token.token == T_LPAREN) {
      return func_call();
    }
    reject_token(&Token);
    n = mkastleaf(A_IDENT, Gsym[id].type, id);
    break;
  default:
    fatald("Syntax error on token %d", Token.token);
  }
  scan(&Token);
  return (n);
}

struct ASTnode *func_call() {
  struct ASTnode *tree = NULL;
  int id = findglob(Text);
  if (id == -1) {
    fatals("Unknown function.", Text);
  }
  lparen();
  tree = binexpr(0);

  tree = mkastunary(A_FUNCCALL, Gsym[id].type, tree, id);

  rparen();
  return tree;
}

// Convert a binary operator token into an AST operation.
static int arithop(int tokentype) {
  switch (tokentype) {
  case T_PLUS:
    return (A_ADD);
  case T_MINUS:
    return (A_SUBTRACT);
  case T_STAR:
    return (A_MULTIPLY);
  case T_SLASH:
    return (A_DIVIDE);
  case T_EQ:
    return (A_EQ);
  case T_NE:
    return A_NE;
  case T_LT:
    return (A_LT);
  case T_GT:
    return A_GT;
  case T_GE:
    return A_GE;
  case T_LE:
    return A_LE;
  default:
    fprintf(stderr, "syntax error on line %d, token %d\n", Line, tokentype);
    exit(1);
  }
}

// Operator precedence for each token
static int OpPrec[] = {
    0,  10, 10, // eof, +, -,
    20, 20,     // *, /, %
    30,         // ==, !=
    40,         // <, >, <=, >=
};

// Check that we have a binary operator and
// return its precedence.
static int op_precedence(int tokentype) {
  int prec = 0;
  switch (tokentype) {
  case T_PLUS:
    prec = 10;
    break;
  case T_MINUS:
    prec = 10;
    break;
  case T_STAR:
    prec = 20;
    break;
  case T_SLASH:
    prec = 20;
    break;
  case T_EQ:
  case T_NE:
    prec = 30;
    break;
  case T_LT:
  case T_LE:
  case T_GT:
  case T_GE:
    prec = 40;
    break;
  default:
    prec = 0;
    break;
  }
  if (prec == 0) {
    printf("->>>>>%d\n", tokentype);
    fprintf(stderr, "syntax error on line %d, token %d\n", Line, tokentype);
    exit(1);
  }
  return (prec);
}

// Return an AST tree whose root is a binary operator.
// Parameter ptp is the previous token's precedence.
struct ASTnode *binexpr(int ptp) {
  struct ASTnode *left, *right;
  int tokentype;

  // Get the integer literal on the left.
  // Fetch the next token at the same time.
  left = prefix();

  // If we hit a semicolon, return just the left node
  tokentype = Token.token;
  if (tokentype == T_SEMI || tokentype == T_RPAREN)
    return (left);

  // While the precedence of this token is
  // more than that of the previous token precedence
  while (op_precedence(tokentype) > ptp) {
    // Fetch in the next integer literal
    scan(&Token);

    // Recursively call binexpr() with the
    // precedence of our token to build a sub-tree
    int prec = op_precedence(tokentype);
    right = binexpr(prec);

    int lefttype = left->type;
    int righttype = right->type;

    if (!type_compatiable(&lefttype, &righttype, 0)) {
      fatal("incompatible types");
    }

    if (lefttype) {
      left = mkastunary(lefttype, righttype, left, 0);
    }
    if (righttype) {
      right = mkastunary(righttype, lefttype, right, 0);
    }

    // Join that sub-tree with ours. Convert the token
    // into an AST operation at the same time.
    left = mkastnode(arithop(tokentype), left->type, left, NULL, right, 0);

    // Update the details of the current token.
    // If we hit a semicolon, return just the left node
    tokentype = Token.token;
    if (tokentype == T_SEMI || tokentype == T_RPAREN)
      return (left);
  }

  // Return the tree we have when the precedence
  // is the same or lower
  return (left);
}

struct ASTnode *prefix() {
  struct ASTnode *tree;
  switch (Token.token) {
  case T_LONGAND:
  case T_AMPER:
    scan(&Token);
    tree = prefix();
    if (tree->op != A_IDENT) {
      fatal("& operator must be followed by an identifier.");
    }

    tree->op = A_ADDR;
    tree->type = pointer_to(tree->type);
    break;
  case T_STAR:
    scan(&Token);
    tree = prefix();

    if (tree->op != A_IDENT && tree->op != A_DEREF) {
      fatal("* operator must be followed by an identifier or `*`.");
    }
    tree = mkastunary(A_DEREF, valaue_at(tree->type), tree, 0);
    break;
  default:
    tree = primary();
  }

  return tree;
}
