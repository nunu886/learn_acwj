#include "decl.h"
#include "defs.h"

#include <errno.h>
#include <stdio.h>

int Line;			// Current line number
int Putback;			// Character put back by scanner
FILE *Infile;			// Input and output files
FILE *Outfile;
struct token Token;		// Last token scanned
#define TEXTLEN 512
char Text[TEXTLEN + 1];		// Last identifier scanned

#define NSYMBOLS 1024
struct symtable Gsym[NSYMBOLS];
int Globs;

// Initialise global variables
static void init() {
  Line = 1;
  Putback = '\n';
  Globs = -1;
  struct symtable Gsym[NSYMBOLS] = {};
}

// Print out a usage if started incorrectly
static void usage(char *prog) {
  fprintf(stderr, "Usage: %s infile\n", prog);
  exit(1);
}

// Main program: check arguments and print a usage
// if we don't have an argument. Open up the input
// file and call scanfile() to scan the tokens in it.
int main(int argc, char *argv[]) {

  if (argc != 2)
    usage(argv[0]);

  init();

  // Open up the input file
  if ((Infile = fopen(argv[1], "r")) == NULL) {
    fprintf(stderr, "Unable to open %s: %s\n", argv[1], strerror(errno));
    exit(1);
  }
  // Create the output file
  if ((Outfile = fopen("out.s", "w")) == NULL) {
    fprintf(stderr, "Unable to create out.s: %s\n", strerror(errno));
    exit(1);
  }

  scan(&Token);			// Get the first token from the input
  genpreamble();		// Output the preamble
  struct ASTnode *tree = NULL;
  while (1) {
    tree = function_declaration();			// Parse the statements in the input
    genAST(tree, -1, 0);
    if (Token.token == T_EOF) {
        break;
    }
  }
  fclose(Outfile);		// Close the output file and exit
  fclose(Infile);
  return 0;
}
