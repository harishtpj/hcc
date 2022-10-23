// Declarations Parser

#include "defs.h"
#include "data.h"
#include "decl.h"

// Parse the declaration of a variable
void var_declaration(void) {
  match(T_INT, "int");
  ident();
  addglob(Text);
  genglobsym(Text);
  semi();
}