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
