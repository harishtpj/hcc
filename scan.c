// Lexical scanning

#include "defs.h"
#include "data.h"
#include "decl.h"

// Return the position of character c in string s, or -1 if c not found
static int chrpos(char *s, int c) {
  char *p;
  p = strchr(s, c);
  return (p ? p - s : -1);
}

// Get the next character from the input file.
static int next() {
    int c;

    if (Putback) {
        c = Putback;
        Putback = 0;
        return c;
    }

    c = fgetc(Infile);
    if ('\n' == c)
        Line++;
    return c;
}

// Put back an unwanted character
static void putback(int c) {
  Putback = c;
}

// Skips unwanted characters
static int skip() {
  int c;

  c = next();
  while (' ' == c || '\t' == c || '\n' == c || '\r' == c || '\f' == c) {
    c = next();
  }
  return c;
}

// Scan and return an integer literal value from the input file
static int scanint(int c) {
  int k, val = 0;

  // Convert each character into an int value
  while ((k = chrpos("0123456789", c)) >= 0) {
    val = val * 10 + k;
    c = next();
  }

  // We hit a non-integer character, put it back.
  putback(c);
  return val;
}

// Scan an identifier from the input file and
// store it in buf[]. Return the identifier's length
static int scanident(int c, char *buf, int lim) {
  int i = 0;

  // Allow digits, alpha and underscores
  while (isalpha(c) || isdigit(c) || '_' == c) {
    // Error if we hit the identifier length limit,
    // else append to buf[] and get next character
    if (lim - 1 == i) {
      fatal("Identifier too long");
    } else if (i < lim - 1) {
      buf[i++] = c;
    }
    c = next();
  }
  // We hit a non-valid character, put it back.
  // NUL-terminate the buf[] and return the length
  putback(c);
  buf[i] = '\0';
  return i;
}

// Return the matching keyword token number or 0 if it's not a keyword.
static int keyword(char *s) {
  switch (*s) {
    case 'e':
      if (!strcmp(s, "else"))
        return T_ELSE;
      break;
    case 'i':
      if (!strcmp(s, "if"))
        return T_IF;
      if (!strcmp(s, "int"))
        return T_INT;
      break;
    case 'p':
      if (!strcmp(s, "print"))
        return T_PRINT;
      break;
    case 'w':
      if (!strcmp(s, "while"))
        return T_WHILE;
      break;
  }
  return 0;
}

// Scan and return the next token found in the input.
// Return 1 if token valid, 0 if no tokens left.
int scan(struct token *t) {
    int c, tokentype;
    c = skip();

    switch (c) {
    case EOF:
        t->token = T_EOF;
        return 0;
    case '+':
        t->token = T_PLUS;
        break;
    case '-':
        t->token = T_MINUS;
        break;
    case '*':
        t->token = T_STAR;
        break;
    case '/':
        t->token = T_SLASH;
        break;
    case ';':
        t->token = T_SEMI;
        break;
    case '{':
        t->token = T_LBRACE;
        break;
    case '}':
        t->token = T_RBRACE;
        break;
    case '(':
        t->token = T_LPAREN;
        break;
    case ')':
        t->token = T_RPAREN;
        break;
    case '=':
        if ((c = next()) == '=') {
          t->token = T_EQ;
        } else {
          putback(c);
          t->token = T_ASSIGN;
        }
        break;
    case '!':
        if ((c = next()) == '=') {
          t->token = T_NE;
        } else {
          fatalc("Unrecognised character", c);
        }
        break;
    case '<':
        if ((c = next()) == '=') {
          t->token = T_LE;
        } else {
          putback(c);
          t->token = T_LT;
        }
        break;
    case '>':
        if ((c = next()) == '=') {
          t->token = T_GE;
        } else {
          putback(c);
          t->token = T_GT;
        }
        break;
    default:
        if (isdigit(c)){
            t->intvalue = scanint(c);
            t->token = T_INTLIT;
            break;
        } else if (isalpha(c) || '_' == c) {
          scanident(c, Text, TEXTLEN);

          if (tokentype = keyword(Text)) {
	          t->token = tokentype;
	          break;
	        }
          
          t->token = T_IDENT;
          break;
        }

        fatalc("Unrecognised character", c);
    }

    return 1;
}