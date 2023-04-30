/*
gcc -Wall -ansi -pedantic main.c -o main -lX11 -lm && ./main
*/
 
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

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
int event_mask=ExposureMask|KeyPressMask;
GC gc; /*graphics context*/

/*variables for keyboard events*/
KeySym keysym;
char buffer[20];
int bufsize = 20;
int charcount;
XComposeStatus compose;

#include "xlib_chaste_bitmap.h"

#include "xlib_chaste_lib.h"
#include "xlib_checkerboard.h"
#include "xlib_polygon.h"

struct chaste_bitmap bitmap; /*possible bitmap I defined that may be used*/
 
int main(int argc, char **argv)
{
 int x,y;

 /*open connection to the server*/
 display=XOpenDisplay(NULL);
 if(display==NULL)
 {
  fprintf(stderr,"Cannot open display\n");
  exit(1);
 }
 
 screen = DefaultScreen(display);
 gc=DefaultGC(display,screen);
 
/*create window of size width*height*/
window=XCreateSimpleWindow(display,RootWindow(display,screen),0, 0,width,height,0,0,0);


 
/*Select kind of events we are interested in. Otherwise may not be able to close program.*/
XSelectInput(display,window,event_mask);
 
/*set drawing color*/
XSetForeground(display,gc,0x808080);
 
/*map (show) the window*/
XMapWindow(display,window);
 
 
  /*set up the checkerboard vars*/
 init_checkerboard();
 main_check.rectsize=16;
 main_check.x_end=width;
 main_check.y_end=height;
 
 /*set up the regular polygon vars*/
 init_polygon();
  main_polygon.sides=5;
  main_polygon.step=2;
  
  main_polygon.cx=width*13/16;
 main_polygon.cy=height/2;
 main_polygon.radius=height/3;
 
 
 bitmap=chaste_LoadBMP("./font/FreeBASIC Font 8.bmp");
 
 
 
 
 /*event loop*/
 while(loop)
 {
  /*printf("beginning of loop\n");*/
 
  XNextEvent(display,&event);
  

  

  

  if(event.type==Expose)
  {
   /*XSetBackground(display,gc,0xF08000);
   XClearWindow(display,window);*/

   /*XClearArea (display, window, 0, 0, 0, 0, True);*/
   
   
   XSetForeground(display,gc,0xF08000);
   FillRectangle(0,0,width,height);
   
   
   /*set color of squares that will be drawn for the checkerboard*/
   XSetForeground(display,gc,0xFFFFFF);
   xlib_chaste_checker();

   XSetForeground(display,gc,0xFF0000);
   x=200;y=100;
   FillTriangle(x,y,x+200,y+400,x-200,y+400);
   
   XSetForeground(display,gc,0x00FF00);
   FillRectangle(400,100,400,400);
   
   XSetForeground(display,gc,0xFF00FF);
   FillCircle(400,100,400,400);

   XSetForeground(display,gc,0xFFFF00);
   FillCircle1(main_polygon.cx,main_polygon.cy,main_polygon.radius);
   
   XSetForeground(display,gc,0x0000FF);
   xlib_chaste_polygon();
   

   main_polygon.radians+=PI/180;

  }
  
  
 
  /*exit on key press*/
  if(event.type==KeyPress)
  {
   charcount=XLookupString(&event.xkey, buffer, bufsize, &keysym,&compose);
   printf("Keysym: %lX\n", keysym);

   
   /*printf("Keycode: %X\n", event.xkey.keycode);
   printf("Key String: %s\n", buffer);*/
   

   
   if(keysym==XK_Escape)
   {
    loop=0;
   }
  }
 }
 
  XCloseDisplay(display);
  
  return 0;
 }
 
 
/*
 Xlib Functions used:
 
 https://www.x.org/releases/current/doc/libX11/libX11/libX11.html#XCreateSimpleWindow
 https://www.x.org/releases/current/doc/libX11/libX11/libX11.html#XSetBackground
 https://www.x.org/releases/current/doc/libX11/libX11/libX11.html#XSetForeground
 https://www.x.org/releases/current/doc/libX11/libX11/libX11.html#XFillRectangle
 https://www.x.org/releases/current/doc/libX11/libX11/libX11.html#XFillPolygon
 https://www.x.org/releases/current/doc/libX11/libX11/libX11.html#XFillArc
*/

