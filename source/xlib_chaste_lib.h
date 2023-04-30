/*
xlib_chaste_lib.h
*/


/*array of 3 points which is enough to draw any triangle*/
XPoint vertices[3];

/*
draws a filled triangle
*/
void FillTriangle(int x0,int y0,int x1,int y1,int x2,int y2)
{
  vertices[0].x=x0;
  vertices[0].y=y0;
  vertices[1].x=x1;
  vertices[1].y=y1;
  vertices[2].x=x2;
  vertices[2].y=y2;
  XFillPolygon(display,window,gc,vertices,3,Convex,CoordModeOrigin);
}

/*
draws a filled rectangle
*/
void FillRectangle(int x,int y,int w,int h)
{
 XFillRectangle(display,window,gc,x,y,w,h);
}

/*
draws a filled circle defined by bounding rectangle
*/
void FillCircle(int x,int y,int w,int h)
{
 XFillArc(display,window,gc,x,y,w,h, 0*64, 360*64);
}




/*
draws a filled circle defined by x,y of the center and radius
this is meant for making a circle that a regular polygon fits
into perfectly if the same location and radius is passed
*/
void FillCircle1(int x,int y,int r)
{
 XFillArc(display,window,gc,x-r,y-r,r*2,r*2, 0*64, 360*64);
}



