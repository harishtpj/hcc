// Statements Parser

#include "defs.h"
#include "data.h"
#include "decl.h"

void statements() {
    struct ASTnode *tree;
    int reg;

    while (1) {
        match(T_PRINT, "print");

        tree = binexpr(0);
        reg = genAST(tree);
        genprintint(reg);
        genfreeregs();

        semi();
        if (Token.token == T_EOF)
            return;
    }
    
}