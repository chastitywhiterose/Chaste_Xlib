/*
This is a part of my code for making convex regular polygons. It has been copied and modified from my original chaste library.
*/


struct polygon
{
 double cx,cy;
 int sides;
 double radius;
 double radians;
 int color;
 int step; /*used only in star polygons of 5 or more sides*/
};

struct polygon main_polygon;


void init_polygon()
{
 main_polygon.cx=width/2;
 main_polygon.cy=height/2;
 main_polygon.sides=3;
 main_polygon.radius=height/2;
 main_polygon.radians=0;
 main_polygon.color=0xFFFFFF;
 main_polygon.step=2;
}

/*these point arrays are temporary and not included in the polygon structure*/
int polygon_xpoints[0x1000],polygon_ypoints[0x1000];

/*Define PI the same as M_PI in math.h*/
 #define PI 3.14159265358979323846f

/*
 this function created because the standard round function did not exist in C until 1999 standard.
 I only use 1989 standard. It is sometimes helpful to round to neared integer when the data type requires it.
 Original source here: https://en.cppreference.com/w/c/numeric/math/round
*/
double chaste_round(double x)
{
 return x<0 ? ceil(x - 0.5) : floor(x + 0.5);
}


void chaste_polygon_points()
{
 double angle,x,y;
 int i=0;
 while(i<main_polygon.sides)
 {
  angle=2*PI*i/main_polygon.sides+main_polygon.radians;
  x=main_polygon.cx+sin(angle)*main_polygon.radius;
  y=main_polygon.cy-cos(angle)*main_polygon.radius;
  polygon_xpoints[i]=chaste_round(x);
  polygon_ypoints[i]=chaste_round(y);
  i++;
 }
}



/*a function pointer that points to whichever function I currently use to get the points
 for the next polygon drawn. This is for conveniently testing new functions.
*/
void (*poly_points_func)()=chaste_polygon_points;




/*
Draw a filled polygon using xlib
*/
void xlib_chaste_polygon()
{
 int i,i1;
 poly_points_func();

 i=0;
 while(i<main_polygon.sides)
 {
  i1=(i+main_polygon.step)%main_polygon.sides;
  vertices[0].x=main_polygon.cx;
  vertices[0].y=main_polygon.cy;
  vertices[1].x=polygon_xpoints[i];
  vertices[1].y=polygon_ypoints[i];
  vertices[2].x=polygon_xpoints[i1];
  vertices[2].y=polygon_ypoints[i1];
  XFillPolygon(display,window,gc,vertices,3,Convex,CoordModeOrigin);
  i+=1;
 }
}

