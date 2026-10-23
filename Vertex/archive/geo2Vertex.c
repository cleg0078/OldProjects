#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"VertexStruct.h"


void main(int argc,char **argv)
{
 int nbpt;
 int realface;
 int nbface,nbligne=0;
 int i,j,ind,nb;
 float x1,x2,x3;
 int i1,i2,i3,i4,i5;
 int mag = MagicFile;
 char buff[256];
 float *ftmp;
 PT3D *ptmp;
 float coeff;
 
 if(argc<3)
   {
    fprintf(stderr,"Needs one argument\n");
    exit(0);
   }
   
 coeff = atof(argv[1]);
 if(coeff<1)
   {
    fprintf(stderr,"Coeff >1\n");
    exit(0);
   }
     
 nbligne = 0;
 while(fgets(buff,256,stdin) )
        nbligne++;
 
 rewind(stdin);

 fscanf(stdin,"%s",buff);
 if( strcmp(buff,"3DG1") != 0)
   {
    fprintf(stderr,"Not A GEO Object\n");
    exit(0);
   }

 fscanf(stdin,"%d",&nbpt);
 fprintf(stderr,"Number Of Points: %d\n",nbpt);
 if( !(ptmp = (PT3D *)malloc((int)sizeof(PT3D)*nbpt)) )
   {
    fprintf(stderr,"Out Of Memory for Points\n");
    exit(0);
   }
   
 for(j=i=0;i<nbpt;i++)
    {
     fscanf(stdin,"%.4f %.4f %.4f",&x1,&x2,&x3);
     ptmp[i].X=coeff*x1; ptmp[i].Y=coeff*x2; ptmp[i].Z=coeff*x3; 
    }
 
 nbface = nbligne - nbpt - 2;
 realface = nbface / 2;
 fprintf(stderr,"Number Of Faces: %d Real=%d\n",nbface,realface);
 fprintf(stderr,"Number lines =%d\n",nbligne);
 mag = (long)MagicFile;
 fwrite(&mag,1,sizeof(int),stdout);
 mag = 1;
 fwrite(&mag,1,sizeof(int),stdout);
 memset(buff,0,20);
 strcpy(buff,(char *)argv[2]);
 fprintf(stderr,"name: %s\n",buff);
 fwrite((char *)buff,12,1,stdout);
 fwrite((char *)&nbpt,sizeof(int),1,stdout);
 fwrite((char *)&realface,sizeof(int),1,stdout);   
 fwrite((char *)ptmp,sizeof(PT3D)*nbpt,1,stdout);
 fprintf(stderr,"Writing Points.... DONE\n");
 fprintf(stderr,"size=%d\n",nbpt*(int)sizeof(PT3D));
 
 for(i=0;i<realface;i++)
    {
     fscanf(stdin,"%d %d %d %d %d",&i1,&i2,&i3,&i4,&i5);
     fwrite((char *)&i2,sizeof(int),1,stdout);
     fwrite((char *)&i3,sizeof(int),1,stdout);
     fwrite((char *)&i4,sizeof(int),1,stdout);
     if(i<10)
        fprintf(stderr,"%d %d %d\n",i2,i3,i4);
        
     fscanf(stdin,"%d %d %d %d %d",&i1,&i2,&i3,&i4,&i5);
    }
    
 exit(0);
}
      
 
