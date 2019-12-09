 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/keysym.h> 

static char *progname = NULL;
static char *displayname = NULL;
static Window window = 0;
static Display *display = NULL;
static char keyname[1024];
static int shift = 0;
static int keysym = 0;
static int debug = 1;

int	ErrorHandler(Display *my_display, XErrorEvent *event)
{
    fprintf(stderr, "%s: XSendEvent(0x%lx) failed.\n", progname, window);
    return 1;
}

void SendEvent(XKeyEvent *event)
{
    XSync(display, False);
    XSetErrorHandler(ErrorHandler);
    XSendEvent(display, window, True, KeyPressMask, (XEvent*)event);
    XSync(display, False);
    XSetErrorHandler(NULL);
}

XKeyEvent createEvent(){
    XKeyEvent event;
    event.display	= display;
    event.window	= window;
    event.root		= RootWindow(display, 0); // XXX nonzero screens?
    event.subwindow	= None;
    event.time		= CurrentTime;
    event.x		= 1;
    event.y		= 1;
    event.x_root	= 1;
    event.y_root	= 1;
    event.same_screen	= True;
    event.type		= KeyPress;
    event.state		= 0;
    return event;
}
void SendKeyPressedEvent(KeySym keysym, unsigned int shift)
{
    XKeyEvent event = createEvent();
  /*   XKeyEvent		event;
    event.display	= display;
    event.window	= window;
    event.root		= RootWindow(display, 0); // XXX nonzero screens?
    event.subwindow	= None;
    event.time		= CurrentTime;
    event.x		= 1;
    event.y		= 1;
    event.x_root	= 1;
    event.y_root	= 1;
    event.same_screen	= True;
    event.type		= KeyPress;
    event.state		= 0;  */ 

    //
    // press down shift keys one at a time...
    //

/*      if (shift & ShiftMask) {
	    event.keycode = XKeysymToKeycode(display, XK_Shift_L);
        printf("SHIFT %d", event.keycode);
	    SendEvent(&event);
	    event.state |= ShiftMask;
    } */
    //if (shift & ControlMask) {
        event.keycode = XKeysymToKeycode(display, XK_Control_L);
        //printf("ControlMask %d", event.keycode);
        SendEvent(&event);
        event.state |= ControlMask;
    //}
/*      if (shift & Mod1Mask) {
        event.keycode = XKeysymToKeycode(display, XK_Alt_L);
        SendEvent(&event);
        event.state |= Mod1Mask;
    }
    if (shift & meta_mask) {
        event.keycode = XKeysymToKeycode(display, XK_Meta_L);
        SendEvent(&event);
        event.state |= meta_mask;
    } */
    
    //  Now with shift keys held down, send event for the key itself...
    // fprintf(stderr, "sym: 0x%x, name: %s\n", keysym, keyname);
    if (keysym != NoSymbol) {
        event.keycode = XKeysymToKeycode(display, keysym);
        // fprintf(stderr, "code: 0x%x, %d\n", event.keycode, event.keycode );
        SendEvent(&event);

        event.type = KeyRelease;
        SendEvent(&event);
    }

    //
    // Now release all the shift keys...
    //

/*     if (shift & ShiftMask) {
        event.keycode = XKeysymToKeycode(display, XK_Shift_L);
        SendEvent(&event);
        event.state &= ~ShiftMask;
    } */
    if (shift & ControlMask) {
        event.keycode = XKeysymToKeycode(display, XK_Control_L);
        SendEvent(&event);
        event.state &= ~ControlMask;
    }
/*     if (shift & Mod1Mask) {
        event.keycode = XKeysymToKeycode(display, XK_Alt_L);
        SendEvent(&event);
        event.state &= ~Mod1Mask;
    }
    if (shift & meta_mask) {
        event.keycode = XKeysymToKeycode(display, XK_Meta_L);
        SendEvent(&event);
        event.state &= ~meta_mask;
    } */
}


void getDisplay(char *displayname){
    if(displayname == NULL){
	    displayname = getenv("DISPLAY");
    }

    if(displayname == NULL){
	    displayname = ":0.0";
    }
    display = XOpenDisplay(displayname);
    //return display
}

typedef struct KeyData {
        KeySym keysym;
        unsigned int shift;
} KeyData;

// st_name is the name of the struct
void print(KeyData *st, const char *st_name)
{
    printf("shift %d", st->shift);
    printf("keysym %s", st->keysym);
}


int	main(int argc, char **argv){
    keysym = 0;
    shift  = 0;
    char *argval = NULL;
    int ii, Junk;

    printf("argv %d", argv[0]);
    printf("argv %d", argv[1]);
    printf("argv %d", argv[2]);

    getDisplay(displayname);
    /* 
    if(displayname == NULL){
	    displayname = getenv("DISPLAY");
    }

    if(displayname == NULL){
	    displayname = ":0.0";
    }
    display = XOpenDisplay(displayname); */

    if (window == 0){
        XGetInputFocus(display, &window, &Junk);
    }
    if (window == (Window)-1){
        window = RootWindow(display, 0); // XXX nonzero screens?
    }
    shift |= ControlMask;
    keysym = XK_V;
    
     const struct KeyData res = { keysym, shift };
     print(&res, "aaa");
/*      */
    printf("keysym %d", keysym);
    printf("keysymsss %s", keysym);
    SendKeyPressedEvent(keysym, shift);
    XCloseDisplay(display);

    return 0;
}

//parseArgs(){}
    //int ii;
  /*   for (ii=1; ii<argc; ii++) {
        argval = argv[ii];
        printf("argv %s", ii);
        printf("argv %d", argv[ii]);
        printf("argval %d", argval);
        
    }  
    for (int i = 0; i < argc; i++) {
        //argval = argv[i];
        printf("argv %s", i);
        printf("argv %d", argv[i]);
        //printf("argval %d", argval);
        
    }  */