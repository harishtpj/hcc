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
        if ((Token.intvalue) >= 0 && (Token.intvalue < 256))
	        n = mkastleaf(A_INTLIT, P_CHAR, Token.intvalue);
        else
	        n = mkastleaf(A_INTLIT, P_INT, Token.intvalue);
        break;

    case T_IDENT:
        id = findglob(Text);
        if (id == -1)
            fatals("Unknown variable", Text);

        n = mkastleaf(A_IDENT, Gsym[id].type, id);
        break;

    default:
        fatald("Syntax error, token", Token.token);
    }

    scan(&Token);
    return n;
}

// Converts Token into AST operation
static int arithop(int tokentype) {
    if (tokentype > T_EOF && tokentype < T_INTLIT)
        return tokentype;
    fatald("Syntax error, token", tokentype);
}

// Operator precedence for each token
static int OpPrec[] = {
  0, 10, 10,			// T_EOF, T_PLUS, T_MINUS
  20, 20,			    // T_STAR, T_SLASH
  30, 30,			    // T_EQ, T_NE
  40, 40, 40, 40		// T_LT, T_GT, T_LE, T_GE
};

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
    int lefttype, righttype;
    int tokentype;

    left = primary();
    
    tokentype = Token.token;
    if (tokentype == T_SEMI || tokentype == T_RPAREN)
        return left;
    
    while (op_precedence(tokentype) > ptp) {
        scan(&Token);
        right = binexpr(OpPrec[tokentype]);
        
        lefttype = left->type;
        righttype = right->type;
        if (!type_compatible(&lefttype, &righttype, 0))
            fatal("Incompatible types");
        
        if (lefttype)
            left = mkastunary(lefttype, right->type, left, 0);
        if (righttype)
            right = mkastunary(righttype, left->type, right, 0);

        left = mkastnode(arithop(tokentype), left->type, left, NULL, right, 0);

        tokentype = Token.token;
        if (tokentype == T_SEMI || tokentype == T_RPAREN)
            return left;
    }

    return left;
}