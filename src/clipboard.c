
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

#include "../lib/xsel/xsel.h"

Bool show_version = False;
Bool show_help = False;
Bool do_append = False, do_clear = False;
Bool do_keep = False, do_exchange = False;
Bool do_input = False, do_output = False;
Bool force_input = False, force_output = False;
Bool want_clipboard = False, do_delete = False;
Window root;
//Display display;
static Display * display;
Atom selection = XA_PRIMARY, test_atom;
int black;
int i, s=0;
unsigned char * old_sel = NULL, * new_sel = NULL;
const char * display_name = NULL;
long timeout_ms = 0L;


void doInput(Atom selection){
    old_sel = get_selection_text(selection);
    if (old_sel){
         printf("%s", old_sel);
         if (*old_sel != '\0' && old_sel[xs_strlen(old_sel) - 1] != '\n'){
// is termnal
 //&& isatty(1) 
             fflush(stdout);
             fprintf(stderr, "\n\\ No newline at end of selection\n");
           }
      }
}

void doOutput(Atom selection){
    if (do_output || force_output) fflush(stdout);
    if (do_append) {
      if (!old_sel){
          old_sel = get_selection_text(selection);
      }
      new_sel = copy_sel(old_sel);
    }
    new_sel = initialise_read(new_sel);
    new_sel = read_input(new_sel, False);
    set_selection__daemon(selection, new_sel);
}

int main(int argc, char *argv[]){

    display = XOpenDisplay(display_name);
    selection = XInternAtom(display, "CLIPBOARD", False);
    doInput(selection);

}