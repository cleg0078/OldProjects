#include<stdio.h>
#include<math.h>
#define imageWidth 256
#define imageHeight 256

unsigned char image[3*imageWidth*imageHeight];

void main()
{
 FILE *f;
 unsigned int i,j;
 float ti,tj;

 f = fopen("ess.tga","wb");
 if( f == NULL)
   {
     fprintf(stderr,"No File\n");
     exit(0);
    }

 fprintf(f,"%c",0); fprintf(f,"%c",0);
 fprintf(f,"%c",2);
 fprintf(f,"%c",0); fprintf(f,"%c",0);
 fprintf(f,"%c",0); fprintf(f,"%c",0);
 fprintf(f,"%c",0);
 fprintf(f,"%c",0); fprintf(f,"%c",0);
 fprintf(f,"%c",0); fprintf(f,"%c",0);
 fprintf(f,"%c",(unsigned char)(imageWidth%256));
 fprintf(f,"%c",(unsigned char)(imageWidth/256));
 fprintf(f,"%c",(unsigned char)(imageHeight%256));
 fprintf(f,"%c",(unsigned char)(imageHeight/256));
 fprintf(f,"%c",24);
 fprintf(f,"%c",0x20);



 for(i = 0; i < imageWidth; i++)
    {
     ti = (2*3.14159265*i)/imageWidth;

     for(j = 0; j < imageHeight; j++)
        {
         tj = (2*3.14159265*j)/imageHeight;
 /*
        fprintf(stderr,"%d  %d  %d\n",(char)(255*(1.0+sin(ti))),(char)(255*(1.0+cos(2*tj))),(char)(255*(1.0+cos(ti+tj))));
 */
        putc((char)(127*(1.0+cos(ti+tj))),f);
        putc((char)(127*(1.0+cos(2*tj))),f);
        putc((char)(127*(1.0+sin(ti))),f);

        }
    }

 fclose(f);
 exit(0);
}
