/*
xlib_chaste_lib.h
*/

void FillRectangle(int x,int y,int w,int h)
{
 XFillRectangle(display,window,gc,x,y,w,h);
}

/*array of 3 points which is enough to draw any triangle*/
XPoint vertices[3];

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

