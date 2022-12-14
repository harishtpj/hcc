// Symbol Table Functions

#include "defs.h"
#include "data.h"
#include "decl.h"

static int Globs = 0;

// Determine if the symbol s is in the global symbol table.
int findglob(char *s) {
    for (int i = 0; i < Globs; i++) {
        if (*s == *Gsym[i].name && !strcmp(s, Gsym[i].name))
            return i;
    }
    return -1;
}

// Get the position of a new global symbol slot
static int newglob() {
    int p;
    if ((p = Globs++) >= NSYMBOLS)
        fatal("Too many Global Symbols");
    return p;
}

// Add a global symbol to the symbol table. Return the slot number in the symbol table
int addglob(char *name, int type, int stype) {
    int y;
    if ((y = findglob(name)) != -1)
        return y;

    y = newglob();
    Gsym[y].name = strdup(name);
    Gsym[y].type = type;
    Gsym[y].stype = stype;
    return y;
}