 

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

int	ErrorHandler(Display *my_display, XErrorEvent *event){
    fprintf(stderr, "%s: XSendEvent(0x%lx) failed.\n", progname, window);
    return 1;
}

void SendEvent(XKeyEvent *event){
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


void PressShift(){
    /*      if (shift & ShiftMask) {
	    event.keycode = XKeysymToKeycode(display, XK_Shift_L);
        printf("SHIFT %d", event.keycode);
	    SendEvent(&event);
	    event.state |= ShiftMask;
    } */
}
void PressControl(){
    /*    
    if (shift & ControlMask) {
        event.keycode = XKeysymToKeycode(display, XK_Control_L);
        //printf("ControlMask %d", event.keycode);
        SendEvent(&event);
        event.state |= ControlMask;
    //}
    */
}
void PressAlt(){
    /*      
if (shift & Mod1Mask) {
        event.keycode = XKeysymToKeycode(display, XK_Alt_L);
        SendEvent(&event);
        event.state |= Mod1Mask;
    }
    */
}
void PressMeta(){
    /*    
    if (shift & meta_mask) {
        event.keycode = XKeysymToKeycode(display, XK_Meta_L);
        SendEvent(&event);
        event.state |= meta_mask;
    }
    */
}

void SendKeyPressedEvent(KeySym keysym, unsigned int shift){
    XKeyEvent event = createEvent();

    event.keycode = XKeysymToKeycode(display, XK_Control_L);
    SendEvent(&event);
    event.state |= ControlMask;
   
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
    //return displayname;
    //return display
}
void getWindow(char *displayname){
    if(displayname == NULL){
	    displayname = getenv("DISPLAY");
    }

    if(displayname == NULL){
	    displayname = ":0.0";
    }
    //return displayname;
    //return display
}
 
int	DoSend()
{
    keysym = 0;
    shift  = 0;
    char *argval = NULL;
    int ii, Junk;
    getDisplay(displayname);
    display = XOpenDisplay(displayname);

    if(window == 0){
        XGetInputFocus(display, &window, &Junk);
    }
    if(window == (Window)-1){
        window = RootWindow(display, 0); // XXX nonzero screens?
    }

    shift |= ControlMask;
    keysym = XK_V;
    SendKeyPressedEvent(keysym, shift);
    XCloseDisplay(display);
    //XFree();
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