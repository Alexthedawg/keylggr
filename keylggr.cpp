#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#define OPT_FILE "output.txt"
#define OPT_STR  "lr"
#define CLN_OPT  "rm -f output.txt"

void errexit (const char *errmsg) {
  printf ("%s\n", errmsg);
  exit (EXIT_FAILURE);
}

int main (int argc, char **argv) {
  int opt = 0;
  int lflag = 0; // get log of the last keystrokes stored by keylggr
  int rflag = 0; // re-initialize keylggr (clearing out old data)
  
  // clean previous output if it exists.
  system (CLN_OPT);
  
  // parse command line arguments.
  while ((opt = getopt (argc, argv, OPT_STR)) != EOF) {
    switch (opt) {
      case 'l':
        lflag++;
        break;
      case 'r':
        rflag++;
        break;
      default:
        errexit ("INVALID ARGS: usage ./keylggr <-l | -r>");
    }
  }
  
  // validate command line argumets: should be either -l or -r, NOT both.
  if (!(lflag ^ rflag)) {
    errexit ("INVALID ARGS: usage ./keylggr <-1 | -r>");
  }
  
  // if l flag is set, retrieve the log of the last keystrokes stored.
  // TODO create l mode function.
  if (lflag) {
    printf ("L");
  }
  
  // if r mode is set, clear previously stored keystrokes and record new strokes.
  // TODO create r mode function.
  if (rflag) {
    printf ("R");
  }
  
  exit (EXIT_SUCCESS);
  
}
