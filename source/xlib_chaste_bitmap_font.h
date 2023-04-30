/*xlib_chaste_bitmap_font.h*/

/*
this file tries to make a basic font library similar to my SDL version except that it uses entirely my own functions in "xlib_chaste_bitmap.h" to load a Windows BMP file into my own "chaste_bitmap" structure defined in the other file. This takes the place of an SDL surface but always assumes that the surface is loaded as a 32 bit unsigned integer.
*/

/*
texture font structure
*/
struct chaste_font
{
 int char_width; /*width of a char*/
 int char_height; /*height of a char*/
 struct chaste_bitmap surface; /*the surface of the image of loaded font*/

};

/*global fonts that will be reused many times*/
struct chaste_font main_font,font_8,font_pico8;


/*rectangle structure meant to replace the one in SDL*/
struct chaste_rect
{
 int x,y,w,h;
};


/*function to load a font and return a structure with the needed data to draw later*/
struct chaste_font chaste_font_load(char *s)
{
 struct chaste_font new_font;
 printf("Loading font: %s\n",s);

 /*load to RGBA32 pixel format for easy handling for later functions*/
 new_font.surface=chaste_LoadBMP(s);
 
 /*
  by default,font height is detected by original image height
  but the font width is the width of the image divided by 95
  because there are exactly 95 characters in the font format that I created.
 */
 new_font.char_width=new_font.surface.w/95; /*there are 95 characters in my font files*/
 new_font.char_height=new_font.surface.h;

 if(new_font.char_height==0)
 {
  printf("Something went horribly wrong loading the font from file:\n%s\n",s);
 }
 else
 {
  /*printf("Font loaded correctly\n");*/
  printf("Size of loaded font is %d,%d\n",new_font.char_width,new_font.char_height);
 }

 return new_font;
}









/*
this uses direct pixel access of the source font surface to draw only when the source pixel is not black. But this one is "special" because it can optionally change the color for each scaled pixel!
*/
void chaste_font_draw_string_scaled(char *s,int cx,int cy,int scale)
{
 int x,y,i,c,cx_start=cx;
 uint32_t *ssp; /*ssp is short for Source Surface Pointer*/
 int sx,sy,sx2,sy2,dx,dy; /*x,y coordinates for both source and destination*/
 uint32_t pixel/*,r,g,b*/; /*pixel that will be read from*/
 int source_surface_width;
 struct chaste_rect rect_source,rect_dest;

 source_surface_width=main_font.surface.w;

 ssp=(uint32_t*)main_font.surface.pixels;
  
 i=0;
 while(s[i]!=0)
 {
  c=s[i];
  if(c=='\n'){ cx=cx_start; cy+=main_font.char_height*scale;}
  else
  {
   x=(c-' ')*main_font.char_width;
   y=0*main_font.char_height;

   /*set up source rectangle where this character will be copied from*/
   rect_source.x=x;
   rect_source.y=y;
   rect_source.w=main_font.char_width;
   rect_source.h=main_font.char_height;

 
   /*Now for the ultra complicated stuff that only Chastity can read and understand!*/
   sx2=rect_source.x+rect_source.w;
   sy2=rect_source.y+rect_source.h;
   
   dx=cx;
   dy=cy;
   
   sy=rect_source.y;
   while(sy<sy2)
   {
    dx=cx;
    sx=rect_source.x;
    while(sx<sx2)
    {
     pixel=ssp[sx+sy*source_surface_width];

      pixel&=0xFFFFFF;
     
     /*printf("pixel 0x%06X %d,%d\n",pixel,sx,sy);*/
     if(pixel!=0) /*only if source pixel is nonzero(not black) draw square to destination*/
     {
      /*set up the rectangle to draw to*/
      rect_dest.x=dx;
      rect_dest.y=dy;
      rect_dest.w=scale;
      rect_dest.h=scale;
      
      /*pixel=chaste_palette[chaste_palette_index];*/
      
      /*r=(pixel&0xFF0000)>>16;
      g=(pixel&0x00FF00)>>8;
      b=(pixel&0x0000FF);*/
      
      /*SDL_SetRenderDrawColor(renderer,r,g,b,255);
      SDL_RenderFillRect(renderer,&rect_dest);*/
      
      FillRectangle(rect_dest.x,rect_dest.y,rect_dest.w,rect_dest.h);
      
     }
     
     sx++;
     dx+=scale;
    }
    sy++;
    dy+=scale;
   }
   /*End of really complicated section*/
   cx+=main_font.char_width*scale;
  }
  i++;
 }

}






