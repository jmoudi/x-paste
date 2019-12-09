 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include "./send.c"


int	main(int argc, char **argv){

    printf("running main");
    printf("argv %s", argv[0]);
    printf("argv %d", argv[1]);
    printf("argv %d", argv[2]);
    DoSend();

    fprintf(stdout, "dsdfdgdfgfdg");
    exit(0);
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