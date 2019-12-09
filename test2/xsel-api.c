

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <sys/time.h>
#include <setjmp.h>
#include <signal.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>

//#include "../lib/xsel/xsel.c"
//#include "../lib/xsel/xsel.h"
#include "./xsel-utils.c"


/* The name we were invoked as(argv[0]) */
static char * progname;

/* Verbosity level for debugging */
//static int debug_level = DEBUG_LEVEL;

/* Our X Display and Window */
static Display * display;
static Window window;

/* Maxmimum request size supported by this X server */
static long max_req;

/* Our timestamp for all operations */
static Time timestamp;

static Atom timestamp_atom; /* The TIMESTAMP atom */
static Atom multiple_atom; /* The MULTIPLE atom */
static Atom targets_atom; /* The TARGETS atom */
static Atom delete_atom; /* The DELETE atom */
static Atom incr_atom; /* The INCR atom */
static Atom null_atom; /* The NULL atom */
static Atom text_atom; /* The TEXT atom */
static Atom utf8_atom; /* The UTF8 atom */
static Atom compound_text_atom; /* The COMPOUND_TEXT atom */

/* Number of selection targets served by this.
 *(MULTIPLE, INCR, TARGETS, TIMESTAMP, DELETE, TEXT, UTF8_STRING and STRING)
 * NB. We do not currently serve COMPOUND_TEXT; we can retrieve it but do not
 * perform charset conversion.
 */
#define MAX_NUM_TARGETS 9
static int NUM_TARGETS;
static Atom supported_targets[MAX_NUM_TARGETS];



/* do_zeroflush: Use only last zero-separated part of input.
 * All previous parts are discarded */
//static Bool do_zeroflush = False;

/* do_follow: Follow mode for output */
//static Bool do_follow = False;

/* nodaemon: Disable daemon mode if True. */
//static Bool no_daemon = False;

/* logfile: name of file to log error messages to when detached */
//static char logfile[MAXFNAME];

/* fstat() on stdin and stdout */
/* static struct stat in_statbuf, out_statbuf;

static int total_input = 0;
static int current_alloc = 0;

static long timeout = 0; 
*/
static struct itimerval timer;
static struct itimerval zerot;
 

#define USEC_PER_SEC 1000000

static int saved_argc;
static char ** saved_argv;




/*
 * main(argc, argv)
 * =================
 *
 * Parse user options and set behaviour.
 *
 * By default the current selection is output and not modified if both
 * standard input and standard output are terminals(ttys). Otherwise,
 * the current selection is output if standard output is not a terminal
 *(tty), and the selection is set from standard input if standard input
 * is not a terminal(tty). If any input or output options are given then
 * the program behaves only in the requested mode.
 *
 * If both input and output is required then the previous selection is
 * output before being replaced by the contents of standard input.
 */
int
main(int argc, char *argv[])
{
  Bool show_version = False;
  Bool show_help = False;
  Bool do_append = False, do_clear = False;
  Bool do_keep = False, do_exchange = False;
  Bool do_input = False, do_output = False;
  Bool force_input = False, force_output = False;
  Bool want_clipboard = False, do_delete = False;
  Window root;
  Atom selection = XA_PRIMARY, test_atom;
  int black;
  int i, s=0;
  unsigned char * old_sel = NULL, * new_sel = NULL;
  char * display_name = NULL;
  long timeout_ms = 0L;

  zerot.it_value.tv_sec = 0;
  zerot.it_value.tv_usec = 0;
  zerot.it_interval.tv_sec = 0;
  zerot.it_interval.tv_usec = 0;

  progname = argv[0];



/* #define OPT(s)(strcmp(argv[i],(s)) == 0)
 
    if (OPT("--input") || OPT("-i")) {
      force_input = True;
      do_output = False;
    } else if (OPT("--clear") || OPT("-c")) {
      do_output = False;
      do_clear = True;
    } else if (OPT("--output") || OPT("-o")) {
      do_input = False;
      force_output = True;
    } 
     */
    force_input = True;
    do_output = False;
    want_clipboard = True;
    timeout_ms = 10;
    //if (OPT("--nodetach") || OPT("-n")) {
    no_daemon = True;
      timeout = timeout_ms * 1000;

    display = XOpenDisplay(display_name);
      //do_exchange = True;
        //if (OPT("--display")) {
      //display_name = argv[i];
 
/*       timeout_ms = strtol(argv[i],(char **)NULL, 10);
      if (timeout_ms < 0) timeout_ms = 0; */

     /* if (OPT("--delete") || OPT("-d")) {
      do_output = False;
      do_delete = True; */
    //} else if (OPT("--logfile") || OPT("-l")) {
      //i++; if (i >= argc) goto usage_err;
      //_xs_strncpy(logfile, argv[i], MAXFNAME);
 
 


  if (display==NULL) {
    exit_err("Can't open display: %s\n",
              display_name ? display_name : "(null)");
  }
  root = XDefaultRootWindow(display);
  
  /* Create an unmapped window for receiving events */
  black = BlackPixel(display, DefaultScreen(display));
  window = XCreateSimpleWindow(display, root, 0, 0, 1, 1, 0, black, black);

  print_debug(D_INFO, "Window id: 0x%x(unmapped)", window);

  /* Get a timestamp */
  XSelectInput(display, window, PropertyChangeMask);
  timestamp = get_timestamp();

  print_debug(D_OBSC, "Timestamp: %lu", timestamp);

  /* Get the maximum incremental selection size in bytes */
  /*max_req = MAX_SELECTION_INCR(display);*/
  max_req = 4000;

  print_debug(D_OBSC, "Maximum request size: %ld bytes", max_req);

  /* Consistency check */
  test_atom = XInternAtom(display, "PRIMARY", False);
  if (test_atom != XA_PRIMARY){
    print_debug(D_WARN, "XA_PRIMARY not named \"PRIMARY\"\n");}
  test_atom = XInternAtom(display, "SECONDARY", False);
  if (test_atom != XA_SECONDARY){
    print_debug(D_WARN, "XA_SECONDARY not named \"SECONDARY\"\n");}

  NUM_TARGETS=0;

  /* Get the TIMESTAMP atom */
  timestamp_atom = XInternAtom(display, "TIMESTAMP", False);
  supported_targets[s++] = timestamp_atom;
  NUM_TARGETS++;

  /* Get the MULTIPLE atom */
  multiple_atom = XInternAtom(display, "MULTIPLE", False);
  supported_targets[s++] = multiple_atom;
  NUM_TARGETS++;

  /* Get the TARGETS atom */
  targets_atom = XInternAtom(display, "TARGETS", False);
  supported_targets[s++] = targets_atom;
  NUM_TARGETS++;

  /* Get the DELETE atom */
  delete_atom = XInternAtom(display, "DELETE", False);
  supported_targets[s++] = delete_atom;
  NUM_TARGETS++;

  /* Get the INCR atom */
  incr_atom = XInternAtom(display, "INCR", False);
  supported_targets[s++] = incr_atom;
  NUM_TARGETS++;

  /* Get the TEXT atom */
  text_atom = XInternAtom(display, "TEXT", False);
  supported_targets[s++] = text_atom;
  NUM_TARGETS++;

  /* Get the UTF8_STRING atom */
  utf8_atom = XInternAtom(display, "UTF8_STRING", True);
  if (utf8_atom != None) {
    supported_targets[s++] = utf8_atom;
    NUM_TARGETS++;
  } else {
    utf8_atom = XA_STRING;
  }

  supported_targets[s++] = XA_STRING;
  NUM_TARGETS++;

  if (NUM_TARGETS > MAX_NUM_TARGETS) {
    exit_err("internal error num-targets(%d) > max-num-targets(%d)\n",
              NUM_TARGETS, MAX_NUM_TARGETS);
  }

  /* Get the NULL atom */
  null_atom = XInternAtom(display, "NULL", False);

  /* Get the COMPOUND_TEXT atom.
   * NB. We do not currently serve COMPOUND_TEXT; we can retrieve it but
   * do not perform charset conversion.
   */
  compound_text_atom = XInternAtom(display, "COMPOUND_TEXT", False);

  sigemptyset(&exit_sigs);
  sigaddset(&exit_sigs, SIGALRM);
  sigaddset(&exit_sigs, SIGINT);
  sigaddset(&exit_sigs, SIGTERM);

  /* handle selection keeping and exit if so */
/*   if (do_keep) {
    keep_selections();
    _exit(0);
  } */

  /* handle selection exchange and exit if so */
  if (do_exchange) {
    exchange_selections();
    _exit(0);
  }

  /* Find the "CLIPBOARD" selection if required */
  if (want_clipboard) {
    selection = XInternAtom(display, "CLIPBOARD", False);
  }

  /* handle output modes */
  if (do_output || force_output) {
    /* Get the current selection */
    old_sel = get_selection_text(selection);
    if (old_sel)
      {
         printf("%s", old_sel);
         if (!do_append && *old_sel != '\0' && isatty(1) &&
             old_sel[xs_strlen(old_sel) - 1] != '\n')
           {
             fflush(stdout);
             fprintf(stderr, "\n\\ No newline at end of selection\n");
           }
      }
  }

  /* handle input and clear modes */
  if (do_delete) {
    get_selection(selection, delete_atom);
  } else if (do_clear) {
    clear_selection(selection);
  }
  else if (do_input || force_input) {
    if (do_output || force_output) fflush(stdout);
    if (do_append) {
      if (!old_sel) old_sel = get_selection_text(selection);
      new_sel = copy_sel(old_sel);
    }
    new_sel = initialise_read(new_sel);
    if (!do_follow)
      new_sel = read_input(new_sel, False);
    set_selection__daemon(selection, new_sel);
  }
  
  exit(0);


}
