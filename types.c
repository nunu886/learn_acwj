
#include "decl.h"
#include "defs.h"
int type_compatiable(int *left, int *right, int onlyright) {
  if (*left == P_VOID || *right == P_VOID) {
    return 0; // false
  }
  if (*left == *right) {
    *left = 0;
    *right = 0;
    return 1; // true
  }

  if (*left == P_CHAR && *right == P_INT) {
    *left = A_WIDEN;
    *right = 0;
    return 1;
  }

  if (*left == P_INT && *right == P_CHAR) {
    if (onlyright) {
      return 0;
    }

    *left = 0;
    *right = A_WIDEN;
    return 1;
  }

  return 1; // true
}

// type to pointer
int pointer_to(int type) {
  int newtype;
  switch (type) {
  case P_VOID:
    newtype = P_VOIDPTR;
    break;
  case P_CHAR:
    newtype = P_CHARPTR;
    break;
  case P_INT:
    newtype = P_INTPTR;
    break;
  case P_LONG:
    newtype = P_LONGPTR;
    break;
  default:
    fatald("unrecognised in pointer to: %d", type);
  }

  return newtype;
}

int valaue_at(int ptrtype) {
  int newtype;
  switch (ptrtype) {
  case P_VOIDPTR:
    newtype = P_VOID;
    break;
  case P_CHARPTR:
    newtype = P_CHAR;
    break;
  case P_INTPTR:
    newtype = P_INT;
    break;
  case P_LONGPTR:
    newtype = P_LONG;
    break;
  default:
    fatald("unrecognised in pointer to: %d", ptrtype);
  }

  return newtype;
}
