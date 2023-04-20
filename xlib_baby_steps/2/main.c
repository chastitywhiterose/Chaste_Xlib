/* gcc -Wall -ansi -pedantic main.c -o main -lX11 -lm && ./main */

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
 
 /* create window of size width*height */
 window=XCreateSimpleWindow(display,RootWindow(display,screen),0, 0,width,height,0,0,0);
 
 /* Select events used.*/
 XSelectInput(display,window,ExposureMask|KeyPressMask);
 
 /*set drawing color*/
 XSetForeground(display,gc,0x808080);
 
 /*map (show) the window*/
 XMapWindow(display,window);
 
 /*event loop*/
 while(loop)
 {
  XNextEvent(display,&event);

  if(event.type==Expose)
  {
   XSetForeground(display,gc,0x000000);
   XFillRectangle(display,window,gc,0,0,width,height);
   
   /*draw a single rectangle with my function*/
   XSetForeground(display,gc,0xFF0000);
   FillRectangle(100,100,400,400);
   
   XSetForeground(display,gc,0x0000FF);
   x=800;y=100;
   FillTriangle(x,y,x+200,y+400,x-200,y+400);
  }

  if(event.type==KeyPress)
  {
   printf("event.type==KeyPress\n");
   break;
  }

 }
 
 XCloseDisplay(display);
 return 0;
}

