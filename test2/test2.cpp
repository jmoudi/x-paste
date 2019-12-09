
#include <iostream> 
#include <vector> 
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include "./xsel.c"

int main(int argc, char *argv[]){
    //static const std::string arr[] = {"aaa","bbb","ccc"};
    static char * arr[] = {"aaa","bbb","ccc"};
    static char * inparr[] = {"--clipboard","--input","ccc"};
    doXsel(argc, inparr);
}