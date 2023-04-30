/*xlib_chaste_bitmap.h*/

/*
 this header file is meant to allow loading bitmap files into a way that can be used in Xlib
 However, nothing specific to Xlib is in this file. These functions are but a tiny selection of my massive Binary Bit Map library from years ago which used nothing but the C standard library.
*/

/*
include this header file to allow fixed width integer types in C
my functions will assume that the data type for each pixel with be a 32 bit unsigned integer
*/
#include <stdint.h>

/*You can change black and white to other colors.*/
uint32_t u32bw[]={0x000000,0xFFFFFF};



/*
 Allocates memory for the pixels which should be 4 bytes/32 bits per pixel. Uses standard library function malloc.
 uint32_t is a 32 bit unsigned integer type. This is why stdint.h is always included.
 I never need more than 32 bits and any more would waste memory.
*/
uint32_t* BBM_malloc(uint32_t width,uint32_t height)
{
 uint32_t *pointer;
 int length=width*height;
 pointer=(uint32_t*)malloc(length*sizeof(*pointer));
 if(pointer==NULL){printf("Error: malloc failed,\n");}
 return pointer;
}


/*
frees the memory the pointer points to, but only if the pointer is not already NULL.
*/
void BBM_free(uint32_t *pointer)
{
 if(pointer!=NULL){free(pointer);pointer=NULL;}
}


/*
A quick dirty replacement for never needing to use fwrite to write my integers! However the catch is that it only works for little endian. For this reason I choose little endian as the format the binary integers will take in my own image formats I am creating. It writes integer i to file pointer fp using count bytes. Most PCs these days can't actually do more than 32 or 64 bits.

This function was necessary so that my code won't completely fail if I do programming on a different CPU in which case fwrite would spit out the big endian byte order. Besides it wasn't too hard to write and requires only 3 arguments instead of 4.
*/
void fputint(unsigned long int i,FILE *fp,int count)
{
 while(count>0)
 {
  fputc(i,fp);
  i>>=8;
  count--;
 }
}

/*The reverse process gets an integer of the specified number of bytes in little endian format.*/
unsigned long int fgetint(FILE *fp,int count)
{
 unsigned long int i=0,c,x;
 
 x=0;
 while(count>0)
 {
  c=fgetc(fp);
  i+=c<<=x;
  x+=8;
  count--;
 }

 return i;
}





/*
I originally copied this function from my main BBM pixel loading function. It has been heavily modified so that it can load the pixels from monochrome BMP files when called from the function which follows it. It's sensitive and may not work reliably but I have had some success with it.
I also added support for 24 bit RGB images.
11-12-2019 update: added support for loading 32 bpp images.
*/
void BBM_Load_BMP_Pixels(uint32_t *p,uint32_t width,uint32_t height,FILE* fp,uint32_t bpp)
{
 uint32_t x,y,pixel,x2,c,bitcount,bits,r,g,b;

 if(bpp==1)
 {
  y=height;
  while(y>0)
  {
   y--;
   bitcount=0;
   x=0;
   while(x<width)
   {
    if(bitcount%8==0)
    {
     c=fgetc(fp);
     if(feof(fp))
     {
      printf("Error: End of file reached.\n");
      free(p); p=NULL; return;
     }
    }
   
    bits=c >> (8-bpp);
    c<<=bpp;
    c&=255;
    bitcount+=bpp;

    /*convert gray into a 24 bit RGB equivalent.*/
    pixel=0;
    x2=0;
    while(x2<24)
    {
     pixel<<=bpp;
     pixel|=bits;
     x2+=bpp;
    }

    p[x+y*width]=pixel;
    x++;
   }

   while(bitcount%32!=0)
   {
    if(bitcount%8==0)
    {
     c=fgetc(fp);
     if(feof(fp))
     {
      printf("Error: End of file reached.\n");
      printf("While skipping row padding.\n");
     }
    }

    bitcount++;
   }
  }
 } /*end of 1 bpp section*/

 else if(bpp==24)
 {
  printf("Type of image is 24 bpp or full color RGB\n");


  y=height;
  while(y>0)
  {
   y--;
   bitcount=0;
   x=0;
   while(x<width)
   {

    pixel=0;
    b=fgetc(fp);
    g=fgetc(fp);
    r=fgetc(fp);

    pixel|=r<<16;
    pixel|=g<<8;
    pixel|=b;


    p[x+y*width]=pixel;

    bitcount+=bpp;

    x++;
   }

   /*printf("bitcount==%d\n",bitcount);*/

   while(bitcount%32!=0)
   {
    c=fgetc(fp);
    if(feof(fp))
    {
     printf("Error: End of file reached.\n");
     printf("While skipping row padding.\n");
    }
    bitcount+=8;
   }

  }
 } /*end of 24 bit code*/

else if(bpp==32)
 {
  printf("Type of image is 32 bpp or full color RGB\n");

  y=height;
  while(y>0)
  {
   y--;
   bitcount=0;
   x=0;
   while(x<width)
   {
    pixel=0;
    b=fgetc(fp);
    g=fgetc(fp);
    r=fgetc(fp);
    fgetc(fp);   /*skip the alpha channel*/

    pixel|=r<<16;
    pixel|=g<<8;
    pixel|=b;

    p[x+y*width]=pixel;

    bitcount+=bpp;
    x++;
   }
  }
 } /*end of 32 bit code*/

 else
 {
  printf("Cannot load image of %d Bits Per Pixel\n",bpp);
 }



}





/*
This function has the limited ability to load certain types of BMP files. Currently it works with the 12 or 40 byte DIB header. Even then, it only works with monochrome AKA 1 bit per pixel images or 24 bit per pixel rgb images. These are the most useful types of files in my experience. It uses the above function for the pixel loading but only after extracting all the variables with image needed for displaying the data.
*/
void BBM_Load_BMP(uint32_t **p,uint32_t *width,uint32_t *height,const char *filename)
{
 int bpp,data,dib_size;
 FILE* fp;
 fp=fopen(filename,"rb");
 /*printf("This function loads a Microsoft BMP file into memory.\n");*/
 if(fp==NULL){printf("Failed to read file \"%s\": Doesn't exist.\n",filename); *p=NULL; return;}
 else{printf("File \"%s\" opened for reading.\n",filename);}

 fseek(fp,0xE,SEEK_SET); /*seek to location of header size*/
 dib_size=fgetint(fp,4);
 printf("dib_size=%d\n",dib_size);

 if(dib_size==12)
 {
  printf("dib header of %d bytes is supported!\n",dib_size);
  fseek(fp,0xA,SEEK_SET);
  data=fgetint(fp,4);
  printf("Pixel Data Starts at address=%d\n",data);
  fseek(fp,0x12,SEEK_SET); /*Seek to width and height location*/
  *width=fgetint(fp,2);
  *height=fgetint(fp,2);
/*
  printf("Width=%d\n",*width);
  printf("Height=%d\n",*height);
*/
  fseek(fp,0x18,SEEK_SET);
  bpp=fgetint(fp,2);
  printf("Bits Per Pixel=%d\n",bpp);
  fseek(fp,data,SEEK_SET);
 }
 else if(dib_size==40||dib_size==124||dib_size==108)
  {
  printf("dib header of %d bytes is supported!\n",dib_size);
  fseek(fp,0xA,SEEK_SET);
  data=fgetint(fp,4);
  printf("Pixel Data Starts at address=%d\n",data);
  fseek(fp,0x12,SEEK_SET); /*Seek to width and height location*/
  *width=fgetint(fp,4);
  *height=fgetint(fp,4);
  printf("Width=%d\n",*width);
  printf("Height=%d\n",*height);
  fseek(fp,0x1C,SEEK_SET);
  bpp=fgetint(fp,2);
  printf("Bits Per Pixel=%d\n",bpp);
  fseek(fp,data,SEEK_SET);
 }
 else
 {
  printf("Because the DIB header size was not an understood size by my code\n");
  printf("It may be a valid file but I need to write code for it\n");
  return;
 }

 /*
  As a speed optimization, only allocate memory if the pointer is NULL.
  Otherwise assume it has enough memory already. Otherwise it allocates and frees every time which is slow for hundreds of frames in a row.
 */
 if(*p==NULL)
 {
  *p=BBM_malloc(*width,*height);
 }

 

 BBM_Load_BMP_Pixels(*p,*width,*height,fp,bpp);

 fclose(fp);
}


/*a structure which contains the minimal data to hold an image or bitmap*/
struct chaste_bitmap
{
 uint32_t w; /*width of the image*/
 uint32_t h; /*height of the image*/
 uint32_t *pixels; /*pointer to array of pixels. this must be allocated somehow*/
};



struct chaste_bitmap chaste_LoadBMP(const char *filename)
{
 struct chaste_bitmap new_bitmap;
 
 /*attempt to allocate memory ahead of time big enough for entire window*/
 new_bitmap.pixels=BBM_malloc(width,height);
 
 printf("attempting to load file %s\n",filename);
 BBM_Load_BMP(&new_bitmap.pixels,&new_bitmap.w,&new_bitmap.h,filename);
 
 printf("end of load file %s\n",filename);

 
 return new_bitmap;
}

void TestBitmap(struct chaste_bitmap bitmap)
{
 int x,y;
 /*checking the validity of pixels*/
 
 y=0;
 while(y<bitmap.h)
 {
  x=0;
  while(x<bitmap.w)
  {
   printf("%d,%d = %X\n",x,y,bitmap.pixels[x+y*bitmap.w]);
   x++;
  }
  y++;
 }
 

}

void DrawBitmap(struct chaste_bitmap bitmap)
{
 int x,y,pixel;
 /*checking the validity of pixels*/
 
 y=0;
 while(y<bitmap.h)
 {
  x=0;
  while(x<bitmap.w)
  {
   pixel=bitmap.pixels[x+y*bitmap.w];
   /*printf("%d,%d = %X\n",x,y,bitmap.pixels[x+y*bitmap.w]);*/
   if(pixel!=0)
   {
    DrawPoint(x,y);
    /*FillRectangle(x,y,1,1);*/
   }
   x++;
  }
  y++;
 }
 
}
