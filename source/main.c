/*
    Simple Xlib application for creating a window and drawing a box in it.
    gcc main.c -o main -lX11
*/
 
#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int width=1280,height=720;
Display *display;
int s; /*screen*/
Window window;
XEvent event;
GC gc; /*graphics context*/

#include "xlib_checkerboard.h"
#include "xlib_polygon.h"

int black,white;
 
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
 
 black=BlackPixel(display, s);
 white=WhitePixel(display, s);
 
 printf("black=%06X\n",black);
 printf("white=%6X\n",white);

/* 
    XSetForeground(display, gc, BlackPixel(display, s));
    XSetBackground(display, gc, WhitePixel(display, s));
 */
 

 
 /*create window*/
 window=XCreateSimpleWindow(display,RootWindow(display,s),0, 0,width,height,0,                               BlackPixel(display, s), WhitePixel(display, s));
 
/*
 Select kind of events we are interested in. Otherwise may not be able to close program.
*/
XSelectInput(display, window, ExposureMask | KeyPressMask);
 
/*set drawing color*/
XSetForeground(display,gc,0x808080);
 
/*map (show) the window*/
XMapWindow(display, window);
 
 
  /*set up the checkerboard vars*/
 init_checkerboard();
 main_check.rectsize=16;
 main_check.x_end=width/2;
 main_check.y_end=height/2;
 
 /*set up the regular polygon vars*/
 init_polygon();
  main_polygon.sides=5;
  main_polygon.step=2;
 
 /*event loop*/
 while(1)
 {
  XNextEvent(display, &event);
  /*draw or redraw the window*/
  if (event.type == Expose)
  {
   /*draw a rectangle to cover entire screen*/
   XSetForeground(display,gc,0x000000);
   XFillRectangle(display,window,gc,0,0,width,height);
   
   /*set color of squares that will be drawn for the checkerboard*/
   XSetForeground(display,gc,0xFF00FF);
   xlib_chaste_checker();
   
   XSetForeground(display,gc,0x00FF00);
   XFillRectangle(display,window,gc,200,200,100,100);
   
   XSetForeground(display,gc,0x000000);
   XDrawString(display,window,gc,50,50,msg,strlen(msg));
   
   XSetForeground(display,gc,0x0000FF);
   xlib_chaste_polygon();
   main_polygon.radians+=PI/180;
   
    main_check.x_begin=200;

  }
  

  
  /*exit on key press*/
  if (event.type == KeyPress){break;}
  
 }
 
  /*close connection to the server*/
  XCloseDisplay(display);
  
  return 0;
 }
 
 
/*
 Xlib Functions used:
 
 https://www.x.org/releases/current/doc/libX11/libX11/libX11.html#XSetForeground
 https://www.x.org/releases/current/doc/libX11/libX11/libX11.html#XFillRectangle
 https://www.x.org/releases/current/doc/libX11/libX11/libX11.html#XFillPolygon
*/

