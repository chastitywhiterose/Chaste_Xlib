/*
sdl_chaste_opengl_core_checkerboard.h

This file shows my own personal method of drawing a checkerboard in modern OpenGL.
By building on my other header files and converting my legacy OpenGL code, I made a checkerboard function which is highly optimized.

*/

struct checkerboard
{
 double x_begin,y_begin,x_end,y_end;
 int rectsize;
 double radius;
 double radians;
};

struct checkerboard main_check;

/*set up initial state to allow drawing checkerboard to whole screen*/
void init_checkerboard()
{
 main_check.x_begin=0;
 main_check.y_begin=0;
 main_check.x_end=width;
 main_check.y_end=height;
 main_check.rectsize=32;
}




/*
 this function draws a checkerboard
 it draws every square of the checkerboard and switches between black and white
*/
void xlib_chaste_checker()
{
 int x,y,index,index1;
 index=0;
 y=main_check.y_begin;
 while(y<main_check.y_end)
 {
  index1=index;
  x=main_check.x_begin;
  while(x<main_check.x_end)
  {
   if(index==1) {XFillRectangle(display,window,gc,x,y,main_check.rectsize,main_check.rectsize);}
   index^=1;
   x+=main_check.rectsize;
  }
  index=index1^1;
  y+=main_check.rectsize;
 }

}


































