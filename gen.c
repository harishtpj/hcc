// AST -> Assembly code

#include "defs.h"
#include "data.h"
#include "decl.h"

// Generate and return a new label number
static int label() {
  static int id = 1;
  return id++;
}

// Generate the code for an IF statement and an optional ELSE clause
static int genIF(struct ASTnode *n) {
  int Lfalse, Lend;

  Lfalse = label();
  if (n->right)
    Lend = label();

  genAST(n->left, Lfalse, n->op);
  genfreeregs();

  genAST(n->mid, NOREG, n->op);
  genfreeregs();

  if (n->right)
    cgjump(Lend);

  cglabel(Lfalse);

  if (n->right) {
    genAST(n->right, NOREG, n->op);
    genfreeregs();
    cglabel(Lend);
  }

  return NOREG;
}

// Generate the code for a WHILE statement and an optional ELSE clause
static int genWHILE(struct ASTnode *n) {
  int Lstart, Lend;

  Lstart = label();
  Lend = label();
  cglabel(Lstart);

  genAST(n->left, Lend, n->op);
  genfreeregs();

  genAST(n->right, NOREG, n->op);
  genfreeregs();

  cgjump(Lstart);
  cglabel(Lend);
  return NOREG;
}

// Given an AST, the register (if any) that holds
// the previous rvalue, and the AST op of the parent,
// generate assembly code recursively.
// Return the register id with the tree's final value
int genAST(struct ASTnode *n, int reg, int parentASTop) {
  int leftreg, rightreg;

  switch (n->op) {
    case A_IF:
      return genIF(n);
    case A_WHILE:
      return genWHILE(n);
    case A_GLUE:
      genAST(n->left, NOREG, n->op);
      genfreeregs();
      genAST(n->right, NOREG, n->op);
      genfreeregs();
      return NOREG;
    case A_FUNCTION:
      cgfuncpreamble(Gsym[n->v.id].name);
      genAST(n->left, NOREG, n->op);
      cgfuncpostamble();
      return NOREG;
  }

  if (n->left)
      leftreg = genAST(n->left, NOREG, n->op);
  if (n->right)
      rightreg = genAST(n->right, leftreg, n->op);

  switch (n->op) {
    case A_ADD:
        return cgadd(leftreg, rightreg);
    case A_SUBTRACT:
        return cgsub(leftreg, rightreg);
    case A_MULTIPLY:
        return cgmul(leftreg, rightreg);
    case A_DIVIDE:
        return cgdiv(leftreg, rightreg);
    case A_EQ:
    case A_NE:
    case A_LT:
    case A_GT:
    case A_LE:
    case A_GE:
        if (parentASTop == A_IF || parentASTop == A_WHILE)
	        return cgcompare_and_jump(n->op, leftreg, rightreg, reg);
        else
	        return cgcompare_and_set(n->op, leftreg, rightreg);
    case A_INTLIT:
        return cgloadint(n->v.intvalue, n->type);
    case A_IDENT:
        return cgloadglob(n->v.id);
    case A_LVIDENT:
        return cgstorglob(reg, n->v.id);
    case A_ASSIGN:
        return rightreg;
    case A_PRINT:
        genprintint(leftreg);
        genfreeregs();
        return NOREG;
    case A_WIDEN:
      return cgwiden(leftreg, n->left->type, n->type);
    default:
        fatald("Unknown AST operator", n->op);
    }
}

void genpreamble() {
  cgpreamble();
}
void genfreeregs() {
  freeall_registers();
}
void genprintint(int reg) {
  cgprintint(reg);
}
void genglobsym(int id) {
  cgglobsym(id);
}