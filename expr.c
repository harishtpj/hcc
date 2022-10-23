// Expressions Parser

#include "defs.h"
#include "data.h"
#include "decl.h"

// Parse a primary factor and return an AST node representing it.
static struct ASTnode *primary() {
    struct ASTnode *n;
    int id;

    switch (Token.token) {
    case T_INTLIT:
        n = mkastleaf(A_INTLIT, Token.intvalue);
        break;
    case T_IDENT:
        id = findglob(Text);
        if (id == -1)
            fatals("Unknown variable", Text);

        n = mkastleaf(A_IDENT, id);
        break;
    default:
        fatald("Syntax error, token", Token.token);
    }

    scan(&Token);
    return n;
}

// Converts Token into AST operation
int arithop(int tokentype) {
    switch (tokentype) {
    case T_PLUS:
        return A_ADD;
    case T_MINUS:
        return A_SUBTRACT;
    case T_STAR:
        return A_MULTIPLY;
    case T_SLASH:
        return A_DIVIDE;
    default:
        fatald("Syntax error, token", tokentype);
    }
}

// Operator precedence for each token
static int OpPrec[] = { 0, 10, 10, 20, 20, 0 };

// Check that we have a binary operator and return its precedence.
static int op_precedence(int tokentype) {
  int prec = OpPrec[tokentype];
  if (prec == 0)
    fatald("Syntax error, token", tokentype);
  return prec;
}


// Return an AST tree whose root is a binary operator
struct ASTnode *binexpr(int ptp) {
    struct ASTnode *left, *right;
    int tokentype;

    left = primary();
    
    tokentype = Token.token;
    if (tokentype == T_SEMI)
        return left;
    
    while (op_precedence(tokentype) > ptp) {
        scan(&Token);
        right = binexpr(OpPrec[tokentype]);
        left = mkastnode(arithop(tokentype), left, right, 0);

        tokentype = Token.token;
        if (tokentype == T_SEMI)
            return left;
    }

    return left;
}