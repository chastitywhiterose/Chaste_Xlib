/*
    Simple Xlib application for creating a window and drawing a box in it.
    gcc main.c -o main -lX11
*/
 
#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int width=1280,height=720; /*global width and height of window*/
int loop=1;
Display *display; 
int screen; /*screen*/
Window window;
XEvent event;
GC gc; /*graphics context*/

#include "xlib_chaste_lib.h"
#include "xlib_checkerboard.h"
#include "xlib_polygon.h"

int black,white;
 
int main(int argc, char **argv)
{
 int x;

 char *msg = "Hello, World!";
 
 /*open connection to the server*/
 display=XOpenDisplay(NULL);
 if(display==NULL)
 {
  fprintf(stderr,"Cannot open display\n");
  exit(1);
 }
 
 screen = DefaultScreen(display);
 gc=DefaultGC(display,screen);
 
 black=BlackPixel(display,screen);
 white=WhitePixel(display,screen);
 
 printf("black=%06X\n",black);
 printf("white=%6X\n",white);

/*create window of size width*height*/
window=XCreateSimpleWindow(display,RootWindow(display,screen),0, 0,width,height,0,0,0);
 
/*
 Select kind of events we are interested in. Otherwise may not be able to close program.
*/
XSelectInput(display,window,ExposureMask|KeyPressMask|StructureNotifyMask);
 
/*set drawing color*/
XSetForeground(display,gc,0x808080);
 
/*map (show) the window*/
XMapWindow(display,window);
 
 
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
 while(loop)
 {
  printf("beginning of loop\n");
 
  XNextEvent(display,&event);
  
  /*XPeekEvent(display,&event);*/
  
  printf("after event check\n");
  
  /*draw or redraw the window*/
  if(event.type==Expose)
  {
   printf("event.type==Expose\n");

   /*draw a rectangle to cover entire screen*/
   XSetForeground(display,gc,0x000000);
   XFillRectangle(display,window,gc,0,0,width,height);
   
   /*set color of squares that will be drawn for the checkerboard*/
   XSetForeground(display,gc,0xFF00FF);
   xlib_chaste_checker();
   
   /*draw a single rectangle with official function*/
   XSetForeground(display,gc,0x00FF00);
   XFillRectangle(display,window,gc,200,200,100,100);
   
   /*draw a single rectangle with my function*/
   XSetForeground(display,gc,0xFF0000);
   FillRectangle(800,100,100,100);
   
      
   XSetForeground(display,gc,0x000000);
   XDrawString(display,window,gc,50,50,msg,strlen(msg));
   
   XSetForeground(display,gc,0x0000FF);
   xlib_chaste_polygon();
   main_polygon.radians+=PI/180;
   
   XSetForeground(display,gc,0xFFFF00);
   x=100;
   FillTriangle(x,650,x+100,650,x+50,550);

   
    main_check.x_begin=200;
    
    printf("%f\n",main_polygon.radians);

  }
  
  
  if(event.type==MapNotify)
  {
   printf("event.type==MapNotify\n");
  }
  
  /*exit on key press*/
  if(event.type==KeyPress)
  {
   printf("event.type==KeyPress\n");
   break;
  }
  
  /*printf("event.type==none\n");*/
   
 }
 
  /*close connection to the server*/
  XCloseDisplay(display);
  
  return 0;
 }
 
 
/*
 Xlib Functions used:
 
 https://www.x.org/releases/current/doc/libX11/libX11/libX11.html#XCreateSimpleWindow
 https://www.x.org/releases/current/doc/libX11/libX11/libX11.html#XSetForeground
 https://www.x.org/releases/current/doc/libX11/libX11/libX11.html#XFillRectangle
 https://www.x.org/releases/current/doc/libX11/libX11/libX11.html#XFillPolygon
*/

