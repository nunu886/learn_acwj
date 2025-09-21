#include "decl.h"
#include "data.h"
#include "defs.h"
#include <stdio.h>
#include <stdlib.h>
// Miscellaneous functions
// Copyright (c) 2019 Warren Toomey, GPL3

// Ensure that the current token is t,
// and fetch the next token. Otherwise
// throw an error
void match(int t, char *what) {
  if (Token.token == t) {
    scan(&Token);
  } else {
    printf("%s expected on line %d\n", what, Line);
    exit(1);
  }
}

// Match a semicon and fetch the next token
void semi(void) {
  match(T_SEMI, ";");
}

void ident() {
    match(T_IDENT, "identifier");
}

void lparen() {
    match(T_LPAREN, "(");
}
void rparen() {
    match(T_RPAREN, ")");
}

void lbrace() {
    match(T_LABRCE, "{");
}

void rbrace() {
    match(T_RABRCE, "}");
}

void fatal(char *s) {
    fprintf(stderr, "%s on line %d\n", s, Line);
    exit(1);
}

void fatals(char* s1, char* s2) {
    fprintf(stderr, "%s:%s on line %d\n", s1, s2, Line);
    exit(1);
}

void fatald(char *s, int t) {
    fprintf(stderr, "%s:%d on line %d\n", s, t, Line);
    exit(1);
}

void fatalc(char* s, int c) {
    fprintf(stderr, "%s:%d on line %d\n", s, c, Line);
    exit(1);
}
