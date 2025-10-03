
#include "data.h"
#include "decl.h"
#include <string.h>

int findglob(char *s) {
  for (int i = 0; i <= Globs; i++) {
    if ((Gsym[i].name != NULL) && !strcmp(s, Gsym[i].name)) {
      return i;
    }
  }
  return (-1);
}

int newglob() {
  Globs = Globs + 1;
  int p = Globs;
  if (p >= NSYMBOLS) {
    fatal("Too many global symbols");
  }
  return p;
}

int addglob(char *name, int type, int stype, int endlabel) {
  int y = findglob(name);
  if (y != -1) {
    return y;
  }

  y = newglob();
  Gsym[y].name = strdup(name);
  Gsym[y].type = type;
  Gsym[y].stype = stype;
  Gsym[y].endlabel = endlabel;
  return y;
}
