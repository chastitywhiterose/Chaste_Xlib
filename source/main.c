/*
    Simple Xlib application for creating a window and drawing a box in it.
    gcc main.c -o main -lX11
*/
 
#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int width=1280,height=720;
Display *display;
int s; /*screen*/
Window window;
XEvent event;
GC gc; /*graphics context*/

#include "xlib_checkerboard.h"
 
int main(void)
{
 char *msg = "Hello, World!";
 
 /*open connection to the server*/
 display = XOpenDisplay(NULL);
 if (display == NULL)
 {
  fprintf(stderr, "Cannot open display\n");
  exit(1);
 }
 
 s = DefaultScreen(display);
 gc=DefaultGC(display, s);
 
 /*create window*/
 window = XCreateSimpleWindow(display, RootWindow(display, s), 0, 0, width, height, 0,
                                 BlackPixel(display, s), WhitePixel(display, s));
 
 /*select kind of events we are interested in*/
 XSelectInput(display, window, ExposureMask | KeyPressMask);

 /*set up the checkerboard vars*/
 init_checkerboard();
 init_checkerboard();
 main_check.rectsize=16;
/* main_check.x_end=width/2;
 main_check.y_end=height/2;*/
 
 /*map (show) the window*/
 XMapWindow(display, window);
 
 
 /*event loop*/
 for(;;)
 {
  XNextEvent(display, &event);
  /*draw or redraw the window*/
  if (event.type == Expose)
  {
   xlib_chaste_checker();
   XFillRectangle(display,window,gc,200,200,100,100);
   XDrawString(display,window,gc,50,50,msg,strlen(msg));
  }
  /*exit on key press*/
  if (event.type == KeyPress){break;}
 }
 
  /*close connection to the server*/
  XCloseDisplay(display);
  
  return 0;
 }
 
 
 /*
 Functions used:
 https://www.x.org/releases/current/doc/libX11/libX11/libX11.html#XFillRectangle
 
 */
