/********************************************************/
/*                     VERTEX 3D V1.0                   */
/*                      UNIX VERSION                    */
/*------------------------------------------------------*/
/*               Vertex "File Operations" File          */
/*                  NO COPYRIGHT May 1995               */
/*------------------------------------------------------*/
/*  Concept And Design: C.LEGENDRE && S.MAUFROY         */
/********************************************************/
 
#include"VertexFile.h"
#include"VertexMemory.h"
#define reset(a,b) memset(a,0,b)
 
extern Display *dpy;
extern XGUI gui;
extern XShell *toplevel;
extern XWidget *objlist,*msg;
extern SCENE3D global_scene;
extern float maxx,maxy,maxz,minx,miny,minz,screen_dist;
extern int nbobj;
extern PT3D tab[300];
extern int nbpt;


char fname[255];

/* IO guard (2026): a file read that a well-formed file must satisfy is
   checked here; on a short read print a short error and let the caller take
   its existing rejection path (fclose + return 0, like the corrupt-count
   guards I-05/I-07 above). */
static int ReadField(FILE *in,void *p,size_t n,const char *what)
{
 if( fread(p,1,n,in) != n )
   {
    fprintf(stderr,"File error: short %s read in %s\n",what,fname);
    return(0);
   }
 return(1);
}

void WriteHeader(FILE *out,int m,int nbob)
{
 int v;

 v=m;
 fwrite(&v,1,sizeof(int),out);
 v=nbob;
 fwrite(&v,1,sizeof(int),out);
}

void WriteOBJ(FILE *out,OBJ3D *ob,char *n,int nb)
{
 int nbpt,nbfc;
 char buff[12]={0};
 
 strncpy(buff,n,12);   /* the format's name field is 12 bytes (I-04) */
 fwrite(buff,12,sizeof(char),out);
 nbpt = ob->NBPT3D;
 fwrite(&nbpt,1,sizeof(int),out);
 nbfc = ob->NBFACE;
 fwrite(&nbfc,1,sizeof(int),out);
 fwrite(ob->PTtable,nbpt*sizeof(PT3D),1,out);
 fwrite(ob->FCtable,nbfc*sizeof(FACE3D),1,out);
}

void WriteHeaderASCII(FILE *out,int m,int nbob)
{
 fprintf(out,"#           VERTEX 3D OBJECTS                 #\n");
 fprintf(out,"# Concept And Design: C.LEGENDRE && S MAUFROY #\n\n");
 fprintf(out,"#NB OBJECTS %d\n",nbob);
}

void WriteOBJASCII(FILE *out,OBJ3D *ob,char *n,int nb)
{
 int i;
 PT3D *pt;
 FACE3D *fc;

 pt = ob->PTtable;
 fc = ob->FCtable;

 fprintf(out,"#OBJECT No %d\n",nb);
 fprintf(out,"\n%d %d %d\n",ob->NBPT3D,ob->NBFACE,3*ob->NBFACE);

 for(i=0; i<ob->NBPT3D ; i++ )
     fprintf(out,"%f %f %f\n",pt[i].X,pt[i].Y,pt[i].Z);

 for(i=0; i<ob->NBFACE ; i++ )
     fprintf(out,"3 %d %d %d\n",(int)fc[i].I1+1,(int)fc[i].I2+1,(int)fc[i].I3+1);

 fprintf(out,"\n\n");
}

int FctFileSave(void wobj(FILE *,OBJ3D *,char *,int),void wh(FILE *,int,int))
{
 FILE *out;
 char *name;
 OBJ3D *ob;
 TREE_OBJ3D *ltmp;
 int i=0,pl,nbo;
 
 if( !(out = fopen(fname,"wb")) )
    return(0);

 ltmp = global_scene.Head;

 for(nbo=0,ltmp = global_scene.Head; ltmp ; ltmp=ltmp->Brother )
      nbo++;
 
 if( (pl = GetActiveListItem(objlist,&name)) == -1 )
   {
     wh(out,MagicFile,nbo);
     for(i=0,ltmp = global_scene.Head; ltmp ; ltmp=ltmp->Brother, i++ )
        {
          ob = ltmp->Actual;
          wobj(out,ob,ob->Name,i);
        }
   }
 else
   {
     wh(out,MagicFile,1);
     ob = FindNamedOBJ(name,pl);
     if(ob != NULL)
        wobj(out,ob,name,i);
    }  

 fclose(out);
 return(1);
}
 
int BadFaces(OBJ3D *ob)
{
 FACE3D *fc;
 int i;

 for(i=0,fc=ob->FCtable ; i<ob->NBFACE ; i++,fc++)
     if( fc->I1 < 0 || fc->I1 >= ob->NBPT3D ||
         fc->I2 < 0 || fc->I2 >= ob->NBPT3D ||
         fc->I3 < 0 || fc->I3 >= ob->NBPT3D )
        return(1);   /* an index outside the point table (I-06) */
 return(0);
}

int LoadVertex()
{
 char buff[60];
 FILE *in;
 OBJ3D *ob;
 int magic,nbob;
 int nbpt,nbfc,tnbfc,tnbpt;
 int i,res;

 in = fopen(fname,"rb");
 if(in==0)
     return(0);

 if( ReadField(in,&magic,sizeof(int),"header") == 0 )
    {   /* truncated header: refuse, like the magic mismatch below */
     fclose(in);
     return(0);
    }
 switch((int)magic)
       {
	case((int)MagicFile): break;
        default:              return(0);
       }	  

 if( ReadField(in,&nbob,sizeof(int),"object count") == 0 )
    {
     fclose(in);
     return(0);
    }

 if( nbob < 1 || nbob > 10000 )
   {   /* a corrupt count must not drive the loop (I-05) */
    fclose(in);
    return(0);
   }

 for(tnbpt=tnbfc=i=0 ; i<nbob && feof(in) == 0 ; i++ )
    {
     ob = FindNextPlace();
     if(ob == NULL)
       {
         fclose(in);
         return(0);
       }

     nbpt = nbfc =0;
     if( ReadField(in,buff,12,"name") == 0 )
       {
        fclose(in);
        return(0);
       }
     buff[12] = 0;   /* the name field is NUL-terminated for strcpy below */

     if( ReadField(in,&nbpt,sizeof(int),"point count") == 0 ||
         ReadField(in,&nbfc,sizeof(int),"face count") == 0 )
       {
        fclose(in);
        return(0);
       }
     if( nbpt < 0 || nbfc < 0 || nbpt > 1000000 || nbfc > 1000000 )
       {   /* corrupt per-object counts (I-05) */
        fclose(in);
        return(0);
       }
     tnbfc+=nbfc;  tnbpt+=nbpt;

     res = (int)AllocOBJ3D(ob,nbpt,nbfc);

     if( res != 0 )
       {
         if( ReadField(in,(char *)ob->PTtable,nbpt*sizeof(PT3D),"point table") == 0 ||
             ReadField(in,(char *)ob->FCtable,nbfc*sizeof(FACE3D),"face table") == 0 )
           {   /* truncated tables would render garbage (I-05 policy) */
            fclose(in);
            return(0);
           }
         if( BadFaces(ob) )
           {   /* a face index outside the point table would read out
                  of bounds at render time (I-06): refuse the file */
            fclose(in);
            return(0);
           }
         memset(ob->Name,0,25);
         strcpy(ob->Name,buff);
       }
      else
        break;

     AddListItem(objlist,ob->Name);
    }

 sprintf(buff,"Vertex File Loaded: %d Points, %d Faces",tnbpt,tnbfc);
 SetTextString(msg,buff);  
 fclose(in);
 VertexReDisplay(); 
 return(1);
}


unsigned long buildlong(unsigned char t[4])
{
 unsigned long tmp;

 tmp = (unsigned long)(t[0]<<24 | t[1]<< 16 | t[2]<<8 | t[3]);
 return(tmp);
}

float buildfloat(unsigned char t[4])
{
 float res;
 unsigned long *tmp;
 unsigned char v;

 tmp = (unsigned long *)&res;
 v = t[3] & 128;

 t[3] = (t[3]<<1) + (t[2]%2);
 t[2] |= v;
 *tmp = t[0]<<24 | t[1]<< 16 | t[2]<<8 | t[3];
 return(30*res);
}

int LoadLWOB()
{
 FILE *in;
 unsigned char buff[64],buff1[12];   /* 64: holds the full loaded-status below (-Wformat-overflow) */
 unsigned long val;
 float x,y,z,*ptmp;
 int i,nbpt,nbfc,pt,fc,*ftmp;
 unsigned short i0,i1,i2,i3,i4;
 PT3D *ptptr;
 FACE3D *fcptr;
 OBJ3D *ob;

 in = fopen(fname,"rb");
 if( in == NULL)
    return(0);

 /* Verify The File Format */
 reset(buff,5);
 if( fread(buff,4,1,in) != 1 )   /* truncated header: refuse, like the FORM check */
    return(0);
 if(strcmp(buff,"FORM") != 0 )
    return(0);

 /* Read The File Size */
 reset(buff,5);
 if( fread(buff,4,1,in) != 1 )   /* truncated header: give up cleanly */
    return(0);

 /* Read The File Identifier */
 reset(buff,5);
 if( fread(buff,4,1,in) != 1 )   /* truncated header: refuse, like the LWOB check below */
    return(0);
 if( strcmp(buff,"LWOB") != 0 )
   {
    fclose(in);
    return(0);
   }

 pt=fc=0;
 ptptr = 0;
 fcptr = 0;
 nbpt = nbfc = 0;
 while( (pt==0 || fc==0) && feof(in) == 0)
      {
        reset(buff1,10);
        if( ReadField(in,buff1,4,"chunk id") == 0 )
           goto LWShort;   /* truncated chunk header: stop the parse cleanly */

        reset(buff,10);
        if( ReadField(in,buff,4,"chunk size") == 0 )
           goto LWShort;
        val = buildlong(buff);

        if( val > 67108864UL )
          {   /* a corrupt chunk length: give up cleanly (I-07) */
           free(ptptr);
           free(fcptr);
           fclose(in);
           return(0);
          }

        if( strncmp(buff1,"PNTS",4) == 0 )
          {
           ptptr = (PT3D *)calloc(val/12*sizeof(PT3D),1);
           if( ptptr == NULL)
             {
              if( fcptr != NULL )  free(fcptr);
              fclose(in);
              return(0);
             }

           for(nbpt=val/12,ptmp=(float *)ptptr,i=0 ; i<nbpt ; i++)
              {
               reset(buff,15);
               if( ReadField(in,buff,4,"point x") == 0 ) goto LWShort;
               x = buildfloat(buff);
               if( ReadField(in,buff,4,"point y") == 0 ) goto LWShort;
               y = buildfloat(buff);
               if( ReadField(in,buff,4,"point z") == 0 ) goto LWShort;
               z = buildfloat(buff);
               ptptr[i].X = x;
               ptptr[i].Y = y;
               ptptr[i].Z = z;
              }
           pt=1;
          }
        else
         if( strncmp(buff1,"POLS",4) == 0 )
           { 
            fcptr = (FACE3D *)calloc(val/20*sizeof(FACE3D),1);
            if( fcptr == NULL)
              {
               if(ptptr != NULL)  free(ptptr);
               fclose(in);
               return(0);
              }

            for(nbfc=val/20,ftmp=(int *)fcptr,i=0 ; i< nbfc ; )
               {
                if( ReadField(in,&i0,2,"face size") == 0 || ReadField(in,&i1,2,"face v1") == 0 ||
                    ReadField(in,&i2,2,"face v2") == 0 || ReadField(in,&i3,2,"face v3") == 0 ||
                    ReadField(in,&i4,2,"face pad") == 0 ) goto LWShort;

                /* Flip The Octet: Conversion MOTOROLA-INTEL */
                i1 = (unsigned short)i1<<8 | (unsigned short)i1>>8;
                i2 = (unsigned short)i2<<8 | (unsigned short)i2>>8;
                i3 = (unsigned short)i3<<8 | (unsigned short)i3>>8;
 
                fcptr[i].I1 = i1;
                fcptr[i].I2 = i2;
		fcptr[i++].I3 = i3;

                if( ReadField(in,&i0,2,"face size") == 0 || ReadField(in,&i1,2,"face v1") == 0 ||
                    ReadField(in,&i2,2,"face v2") == 0 || ReadField(in,&i3,2,"face v3") == 0 ||
                    ReadField(in,&i4,2,"face pad") == 0 ) goto LWShort;
               }
            fc=1;
           }
         else
           for(i=0 ; i<val; i++)
              if( ReadField(in,buff,1,"chunk data") == 0 )
                 goto LWShort;   /* skipped chunk truncated: abort the parse cleanly */
     }

 fclose(in);

 ob = FindNextPlace();
 if(ob == NULL)
    goto End;
 
 if( AllocOBJ3D(ob,nbpt,nbfc)  == 0 )
   {
    End:
     free(ptptr);
     free(fcptr);
     return(0);
   }


 memcpy(ob->PTtable,ptptr,nbpt*sizeof(PT3D));
 memcpy(ob->FCtable,fcptr,nbfc*sizeof(FACE3D));

 sprintf(&buff[0],"OBJ%d",nbobj++);
 memset(ob->Name,0,25);
 strcpy(ob->Name,buff);
 AddListItem(objlist,buff);

 sprintf(buff,"LightWave File Loaded: %d Points, %d Faces",nbpt,nbfc);
 SetTextString(msg,buff);

 FindMinMax(ob);
/*
 maxx = ob->Max.X; maxy = ob->Max.Y; maxz = ob->Max.Z;
 minx = ob->Min.X; miny = ob->Min.Y; minz = ob->Min.Z;
*/
 free(ptptr);
 free(fcptr);
      
 VertexReDisplay();
 return(1);

 LWShort:   /* a truncated chunk: give up like the corrupt-length bail (I-07) */
 free(ptptr);
 free(fcptr);
 fclose(in);
 return(0);
}


int FileSave3DS()
{
 FILE *out;
 char *name;
 char name2[50];
 OBJ3D *ob;
 int i,pl;

 pl=GetActiveListItem(objlist,&name);

 if( pl == -1 )
    return(0);

 memset(name2,0,50);
 strcpy(name2,name);
 strcat(name2,".asc");
 puts(name2);

 if( !(out = fopen(name2,"wt")) )
    return(0);

 ob = FindNamedOBJ(name,pl);
 if( ob == NULL )
   {
    fclose(out);
    return(0);
   }

 fprintf(out,"Named object: \"VOBJ0\"%c%c",0XD,0XA);
 fprintf(out,"Tri-mesh, Vertices: %d     Faces: %d%c%c",ob->NBPT3D,ob->NBFACE,0XD,0XA);
 fprintf(out,"Vertex list:%c%c",0XD,0XA);
 for(i=0;i<ob->NBPT3D;i++)
    {
     fprintf(out,"Vertex %d:  X: %f     Y: %f     Z: %f%c%c",i,ob->PTtable[i].X,ob->PTtable[i].Y,ob->PTtable[i].Z,0XD,0XA);
    }
 
 fprintf(out,"Face List:%c%c",0XD,0XA);
 for(i=0;i<ob->NBFACE;i++)
     fprintf(out,"Face %d:  A: %d B: %d C: %d AB: 1 BC: 1 CA:1%c%c",i,ob->FCtable[i].I1,ob->FCtable[i].I2,ob->FCtable[i].I3,0XD,0XA);

 fclose(out);
 return(1);
}


int FileSave()
{
 return(FctFileSave(WriteOBJ,WriteHeader));
}
 
int FileSaveASCII()
{
 return(FctFileSave(WriteOBJASCII,WriteHeaderASCII));
}

/*
void FileLoadGEN()
{
 FILE *in;
 char *name;
 int i;
 
 VertexRevolNew();
 XtVaGetValues(sname,XtNstring,&name,NULL);
 if( !(in = fopen(name,"rb")) )
    return;

 fread(&nbpt,sizeof(SHORT),1,in);
 
 for(i=0;i<nbpt;i++)
     fread(&tab[i],sizeof(PT3D),1,in);

 fclose(in);
 XtPopdown(FileShell);
 VertexRevolDispALL();
}
*/

/*
void FileSaveGEN()
{
 FILE *out;
 char *name;
 int i;

 XtVaGetValues(sname,XtNstring,&name,NULL);
 if( !(out = fopen(name,"wb")) )
    return;

 fwrite(&nbpt,sizeof(SHORT),1,out);

 for(i=0;i<nbpt;i++)
     fwrite(&tab[i],sizeof(PT3D),1,out);

 fclose(out);
 XtPopdown(FileShell);
}
*/
 
void LoadFile(char *name)
{
 char *tmp;

 memset(fname,0,255);

 tmp = FileRequest(&gui,name,0,0,(int)(250*XGUIScreenScale(gui.dpy)),(int)(210*XGUIScreenScale(gui.dpy)));
 if( tmp == NULL)
    return;

 strcpy(fname,tmp);
 if( strlen(fname) )
   {
    if( LoadVertex() == 0 )
       if( LoadLWOB() == 0 )
          SetTextString(msg,"Not A Reconizable Object");
   }
}

int FileSavePOV()
{
 FILE *out;
 OBJ3D *ob;
 TREE_OBJ3D *ltmp;
 int j,i;
 int nbf,*ftmp;
 PT3D *pt;

 if( !(out = fopen(fname,"wb")) )
    return(0);

 fprintf(out,"#include \"colors.inc\"\n");
 fprintf(out,"#include \"textures.inc\"\n");
 fprintf(out,"#include \"shapes.inc\"\n\n");
 fprintf(out,"camera{\n");
 fprintf(out,"        location  < 0.0 , 0.0, %f >\n",minz);
 fprintf(out,"        direction < 0.0 , 0.0, 1.0 >\n");
 fprintf(out,"      }\n");
 

 fprintf(out,"light_source{\n");
 fprintf(out,"              < 0.0 , 0.0, %f >\n",minz);
 fprintf(out,"              color White\n");
 fprintf(out,"            }\n");

 ltmp = global_scene.Head;

 for(i=0,ltmp = global_scene.Head; ltmp ; ltmp=ltmp->Brother, i++ )
    {
     ob = ltmp->Actual;
     nbf = ob->NBFACE;
     
     fprintf(out,"union {\n");
   
     for(pt=ob->PTtable,ftmp=(int *)ob->FCtable,j=0; j<nbf ; j++)
        {
         fprintf(out,"triangle {\n");
         fprintf(out,"          < %f, %f, %f >,",pt[*ftmp].X,pt[*ftmp].Y,pt[*ftmp].Z);
         ftmp++;
         fprintf(out,"< %f, %f, %f >,",pt[*ftmp].X,pt[*ftmp].Y,pt[*ftmp].Z);
         ftmp++;
         fprintf(out,"< %f, %f, %f >\n",pt[*ftmp].X,pt[*ftmp].Y,pt[*ftmp].Z);
         ftmp++;
         fprintf(out,"          texture { Tom_Wood }\n");
         fprintf(out,"         }\n");
        }
      fprintf(out,"}\n\n");
     }

 fclose(out);
 return(1);
}

void SaveFile(char *name,int fct())
{
 char *tmp;
 memset(fname,0,255);

 tmp = FileRequest(&gui,name,0,0,(int)(250*XGUIScreenScale(gui.dpy)),(int)(210*XGUIScreenScale(gui.dpy)));
 if( tmp == NULL)
    return;

 strcpy(fname,tmp);

 if( strlen(fname) != 0)
   {
    if( fct() == 0 )
       SetTextString(msg,"Error During Writting Object");
   }
 else
   SetTextString(msg,"Can't Save NULL Name Object");
}

void VertexLoad()
{
 LoadFile("Vertex LOAD File");
}

void VertexSave()
{
 SaveFile("Vertex SAVE File",FileSave);
}

void VertexSaveASCII()
{
 SaveFile("Vertex SAVE ASCII",FileSaveASCII);
}

/*
void VertexFileGEN()
{
 SaveFile("Vertex Revolution File",FileLoadGEN,1,"SAVE",FileSaveGEN);
}
*/

void VertexSave3DS()
{
 SaveFile("Vertex SAVE 3DS",FileSave3DS);
}

void VertexSavePOV()
{
 SaveFile("Vertex Save POV",FileSavePOV);
}

