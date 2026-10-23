#include<stdio.h>
#include"VertexStruct.h"

typedef struct segm
{
 int i1,i2;
}SEGM;

typedef struct Fce
{
 int s1,s2,s3;
}FCE;
 
SEGM *segtmp;
float *ptmp;
int *ftmp;
FCE *fcind;

void findnum(int r)
{
 int j,t1,t2,t3;

 t1 = segtmp[fcind[r].s1].i1;
 t2 = segtmp[fcind[r].s1].i2;

 if( segtmp[fcind[r].s2].i1 != t1 && segtmp[fcind[r].s2].i1 != t2)
    t3=segtmp[fcind[r].s2].i1;
 else
    t3=segtmp[fcind[r].s2].i2;

 ftmp[3*r]=t1;
 ftmp[3*r+1]=t2;
 ftmp[3*r+2]=t3;
    
}

void main(int argc,char **argv)
{
 int nbpt;
 int nbseg;
 int nbface;
 int i,j,ind,nb;
 float x1,x2,x3;
 int color,i1,i2,i3,i4;
 int mag = MagicFile;
 char buff[22]={0};

 if(argc<2)
   {
    fprintf(stderr,"Needs one argument\n");
    exit(0);
   }

 fscanf(stdin,"%d",&nbpt);
 fprintf(stderr,"Number Of Points: %d\n",nbpt);
 if( !(ptmp = (float *)malloc(3*sizeof(float)*nbpt)) )
   {
    fprintf(stderr,"Out Of Memory for Points\n");
    exit(0);
   }

 for(j=i=0;i<nbpt;i++)
    {
     fscanf(stdin,"%d %f %f %f",&ind,&x1,&x2,&x3);
     ptmp[j++] = x1; ptmp[j++] = x2; ptmp[j++] = x3; 
    }
 
 fscanf(stdin,"%d",&nbseg);
 fprintf(stderr,"Number Of Segments: %d\n",nbseg);
 if( !(segtmp = (SEGM *)malloc(sizeof(SEGM)*nbseg)) )
   {
    fprintf(stderr,"Out Of Memory for Points\n");
    exit(0);
   }

 for(j=i=0;i<nbseg;i++)
    {
     fscanf(stdin,"%d %d %d %d",&ind,&color,&i1,&i2);
     segtmp[i].i1 = i1; segtmp[i].i2 = i2;
    }

 fscanf(stdin,"%d",&nbface);
 fprintf(stderr,"Number Of Faces: %d\n",nbface);
 if( !(fcind = (FCE *)malloc(sizeof(FCE)*nbface)) )
   {
    fprintf(stderr,"Out Of Memory for Faces\n");
    exit(0);
   }

 for(j=i=0;i<nbface;i++)
    {
     fscanf(stdin,"%d %d %d %d %d %d",&ind,&color,&nb,&i1,&i2,&i3);
     fcind[i].s1 = i1; fcind[i].s2 = i2; fcind[i].s3 = i3;
    }

 if( !(ftmp = (int *)malloc(3*sizeof(int)*nbface)) )
   {
    fprintf(stderr,"Out Of Memory for Final Faces\n");
    exit(0);
   }

 for(i=0;i<nbface;i++)
     findnum(i);

 mag = MagicFile;
 fwrite(&mag,1,sizeof(int),stdout);
 mag = 1;
 fwrite(&mag,1,sizeof(int),stdout);
 strncat(buff,argv[1],12);
 fprintf(stderr,"name: %s\n",buff);
 fwrite(buff,12,1,stdout);
 fwrite(&nbpt,1,sizeof(int),stdout);
 fwrite(&nbface,sizeof(int),1,stdout);
 fwrite(ptmp,nbpt*3*sizeof(float),1,stdout);
 fwrite(ftmp,nbface*3*sizeof(int),1,stdout);

 exit(0);
}

  
