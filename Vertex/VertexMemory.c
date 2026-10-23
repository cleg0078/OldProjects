/********************************************************/
/*                     VERTEX 3D V1.0                   */
/*                      UNIX VERSION                    */
/*------------------------------------------------------*/
/*                     Vertex Cone File                 */
/*                  NO COPYRIGHT May 1995               */
/*------------------------------------------------------*/
/*  Concept And Design: C.LEGENDRE && S.MAUFROY         */
/********************************************************/
 
#include"VertexStruct.h"
#include"VertexGUI.h"
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<malloc.h>
#define DP(a,b) (a.X*b.X+a.Y*b.Y+a.Z*b.Z)
#define ABS(x) x>0.0?x:-x

extern SCENE3D global_scene;
extern XShell *toplevel;
extern XWidget *objlist,*msg;
extern float minx,maxx,miny,maxy,minz,maxz;
extern int nbobj;
extern Display *dpy;
extern Window win;
extern float screen_dist; 
extern int nbobj,nbdisk,nbcube,nbpyra,nbsphere,nbrevol,nbmount,nbcyl,nbtorus,nbcone;
extern int nbsurf;


static PTList *ptail;
static FCList *ftail;
 
int AllocOBJ3D(OBJ3D *Ptr,int Npoints,int Nfaces)
{
 PT3D *ptrpoints;
 FACE3D *ptrfaces;
 RGBColor *color;

 if( (ptrpoints=(PT3D *)calloc(Npoints*sizeof(PT3D),1)) != 0)
   {
    if( (ptrfaces=(FACE3D *)calloc(Nfaces*sizeof(FACE3D),1)) != 0)
      {
       if( (color=(RGBColor *)calloc(Nfaces*sizeof(RGBColor),1)) != 0 )
         { 
          Ptr->PTtable=ptrpoints;
          Ptr->FCtable=ptrfaces;
          Ptr->NBPT3D=Npoints;
          Ptr->NBFACE=Nfaces;
          Ptr->Color = color;
          return(1);
        }
       else
        {
         free(ptrpoints);
         free(ptrfaces);
        }
       return(0);
      }
    else
      {
       free(ptrpoints);
       return(0);
      }
   }
 else
    return(0);
}   
   
void FreeOBJ3D(OBJ3D *Ptr)
{
 free(Ptr->PTtable);
 free(Ptr->FCtable);
 free(Ptr->Color);
 Ptr->Color = NULL;

 if( Ptr->MMObj1 != NULL)
   {
    free(Ptr->MMObj1->PTtable);
    free(Ptr->MMObj1->FCtable); 
    free(Ptr->MMObj1);
   }

  if( Ptr->MMObj2 != NULL)
    {
     free(Ptr->MMObj2->PTtable);
     free(Ptr->MMObj2->FCtable);
     free(Ptr->MMObj2);
    }
}

OBJ3D *CreateOBJ3D()
{
 return(OBJ3D *)calloc(sizeof(OBJ3D),1);
}


OBJ3D *FindNextPlace()
{
 TREE_OBJ3D *ltmp,*next_tree;
 OBJ3D *ob;
 
 ltmp = global_scene.Tail;
 next_tree = AllocTREE_OBJ3D();
 if(next_tree== 0)
    return(0);

 ob = CreateOBJ3D();
 if(ob == NULL)
   {
    FreeTREE_OBJ3D(next_tree);
    return(0);
   }
 else
   {
     next_tree->Actual = ob;
     if(ltmp != NULL)
        ltmp->Brother = next_tree;
     else
        global_scene.Head = next_tree;

     global_scene.Tail = next_tree;
   }

 return(ob); 
}

OBJ3D *FindNamedOBJ(const char *txt,int pl)
{
 TREE_OBJ3D *ltmp;
 int i;
 for(i=0,ltmp=global_scene.Head ; ltmp ; i++,ltmp = ltmp->Brother)
     if( strcmp(ltmp->Actual->Name,txt) == 0 && i==pl)
        return(ltmp->Actual);
 
 return(NULL);
}

void DestroyOBJ3D(OBJ3D *Ptr)
{
  free(Ptr);
}

TREE_OBJ3D *AllocTREE_OBJ3D()
{
 return(TREE_OBJ3D *)calloc(sizeof(TREE_OBJ3D),1);
}

void FreeTREE_OBJ3D(TREE_OBJ3D *list)
{
 if( list->Actual )
    FreeOBJ3D(list->Actual); /* Free the Object 3D memory */
 free(list);
}

void DeleteOBJ3D(BOOL redisplay)
{
 TREE_OBJ3D *ltmp,*tmp;
 OBJ3D *ob;
 int pl;
 char *txt;
 
 pl = GetActiveListItem(objlist,&txt);
 if(pl<0)
    return;

 ob = FindNamedOBJ(txt,pl);
 if(ob == NULL)
     return;
 
 tmp=ltmp=global_scene.Head;
 while( ltmp && (ltmp->Actual != ob) )
      { 
        tmp=ltmp;
        ltmp=ltmp->Brother;
      } 
 if( ltmp == NULL )
    return;   /* ob is not in the scene list (M-05) */
 
 if(ltmp !=(global_scene.Head) )  
   {
    if(ltmp == global_scene.Tail )
         global_scene.Tail = tmp;
   
    if(tmp != NULL && ltmp != NULL)
       tmp->Brother = ltmp->Brother; 
    else
       if(tmp !=NULL)
          tmp->Brother = NULL;

    FreeOBJ3D(ltmp->Actual);
    DestroyOBJ3D(ltmp->Actual);
    free(ltmp);              /* the spliced scene-tree node (M-04) */
   }
 else
   {
    FreeOBJ3D(ltmp->Actual);
    DestroyOBJ3D(ltmp->Actual);
    global_scene.Head = ltmp->Brother;
    if(ltmp->Brother != NULL)
       {
        TREE_OBJ3D *promoted = ltmp->Brother;
	ltmp->Actual = promoted->Actual;
        ltmp->Brother = promoted->Brother;
        if( ltmp->Brother == NULL )
           global_scene.Tail = ltmp;   /* the promoted node was the Tail */
        free(promoted);        /* node folded into the head (M-04) */
       }
    else
       {
         ltmp->Brother = NULL;
         global_scene.Tail = NULL;	
         free(ltmp);          /* last node of the scene (M-04) */
       }
   }       
 DestroyListItem(objlist,pl);
 nbobj--;

 if(redisplay)
    VertexReDisplay();
} 


void VertexDelOBJ()
{
 DeleteOBJ3D(1);
}

void VertexNEWAll()
{
 int i;
 
 while( (i=ActivePosListItem(objlist,0) ) == 1 )
        DeleteOBJ3D(0);

 nbobj = nbdisk = nbcube = nbpyra = nbsphere = nbrevol = 0;
 nbsurf = nbmount = nbcyl = nbcone = nbtorus = 0;
 maxx=maxy=maxz = 1000.0;
 minx=miny= -1000.0;
 minz=-2000.0;
 screen_dist = 350.0;

 VertexReDisplay();
}     

void *AllocList(int size)
{
 return((void *)calloc(size,1));
}

void FreeList(void *p)
{
 free(p);
}

int NbPoint(PTList *l)
{
 int nb;
 PTList *tmp;
 
 for(tmp=l,nb=0; tmp ; nb++, tmp=tmp->Next);

 return(nb);
}

int NbFace(FCList *l)
{
 int nb;
 FCList *tmp;

 for(tmp=l,nb=0; tmp ; nb++, tmp=tmp->Next );

 return(nb);
}

int FindPoint(PTList *p,float x,float y,float z)
{
 PTList *tmp;
 int nb;

 for(nb=0,tmp=p; tmp ; nb++,tmp=tmp->Next )
     if( tmp->Info.X == x && tmp->Info.Y == y && tmp->Info.Z == z )
        return(nb);
 
 return(-1);
}

int FindPointXY(PTList *p,float x,float y,float eps)
{
 PTList *tmp;
 int nb;

 for(nb=0,tmp=p; tmp ; nb++,tmp=tmp->Next )
     if( (tmp->Info.X-eps) <= x && (tmp->Info.X+eps) >= x )
        if( (tmp->Info.Y-eps) <= y && (tmp->Info.Y+eps) >= y )
           return(nb);

 return(-1);
}

int FindPointXZ(PTList *p,float x,float z,float eps)
{
 PTList *tmp;
 int nb;

 for(nb=0,tmp=p; tmp ; nb++,tmp=tmp->Next )
     if( (tmp->Info.X-eps) <= x && (tmp->Info.X+eps) >= x )
        if( (tmp->Info.Z-eps) <= z && (tmp->Info.Z+eps) >= z )
           return(nb);

 return(-1);
}

int FindPointYZ(PTList *p,float y,float z,float eps)
{
 PTList *tmp;
 int nb;

 for(nb=0,tmp=p; tmp ; nb++,tmp=tmp->Next )
     if( (tmp->Info.Y-eps) <= y && (tmp->Info.Y+eps) >= y )
        if( (tmp->Info.Z-eps) <= z && (tmp->Info.Z+eps) >= z )
           return(nb);

 return(-1);
}

int DelPoint(PTList **p,float x,float y,float z)
{
 int i,nb;
 PTList *tmp,*act;

 if( (nb=FindPoint(*p,x,y,z)) >= 0 )
   {
    for(act=tmp=*p,i=0;i<nb;i++,act=tmp,tmp=tmp->Next);
    if(nb==0)
       *p = tmp->Next;
    else
       act->Next = tmp->Next;
    
    if(tmp==ptail)
       ptail = (*p==NULL) ? NULL : act; /* no dangling tail after emptying (M-06) */

    FreeList((void *)tmp);
    return(1);
   }
 
 return(-1);
}
   
int FindFace(FCList *p,int x,int y,int z)
{
 FCList *tmp;
 int nb;

 for(nb=0,tmp=p; tmp ; nb++,tmp=tmp->Next )
    if( (tmp->Info.I1 == x && tmp->Info.I2 == y && tmp->Info.I3 == z) ||
        (tmp->Info.I1 == x && tmp->Info.I2 == z && tmp->Info.I3 == y) ||
        (tmp->Info.I1 == y && tmp->Info.I2 == x && tmp->Info.I3 == z) ||
        (tmp->Info.I1 == y && tmp->Info.I2 == z && tmp->Info.I3 == x) ||
        (tmp->Info.I1 == z && tmp->Info.I2 == x && tmp->Info.I3 == y) ||
        (tmp->Info.I1 == z && tmp->Info.I2 == y && tmp->Info.I3 == x)
      )
      return(nb);

 return(-1);
}


int DelFace(FCList **p,int i,int j,int k)
{
 int x,nb;
 FCList *tmp,*act;

 if( (nb=FindFace(*p,i,j,k)) >=0 )
   {
    for(act=tmp=*p,x=0;x<nb;x++,act=tmp,tmp=tmp->Next);
    
    if(nb==0)
       *p = tmp->Next;
    else
       act->Next = tmp->Next;
    
    ftail = NULL;   /* the removed node may have been the tail (M-06/M-08) */

    FreeList((void *)tmp);
    return(1);
   }

 return(-1);
}


int AddPoint(PTList **p,float x,float y,float z)
{
 PTList *tmp,*act;

 tmp = NULL;
 act = ptail;

 if(*p==0)
   {
     *p = (PTList *)AllocList(sizeof(PTList)); /* Alloc The Head List */
     tmp = *p;
   }
 else
     tmp = (PTList *)AllocList(sizeof(PTList));
 
 if(tmp == 0)
    return(-1);
  
 if(act)
    act->Next = tmp;

 tmp->Next = 0;
 tmp->Info.X = x;
 tmp->Info.Y = y;
 tmp->Info.Z = z;
 ptail = tmp; 
 return(1);
}

int AddFace(FCList **p,int x,int y,int z)
{
 FCList *tmp,*act,*lact;
 int i;
 
 tmp = NULL;

 /* Append at the tracked tail; walk to it when the tail is unknown (M-08) */
 act = ftail;
 if( act == NULL )
   {
    for(lact=act=*p,i=NbFace(*p) ; act && i ; i--,lact=act,act=act->Next );
    act = lact;
   }

 if(*p==NULL)
   {
     tmp = (FCList *)AllocList(sizeof(FCList));
     *p = tmp;
    }
 else
     tmp = (FCList *)AllocList(sizeof(FCList));
 
 if(tmp == NULL)
    return(-1);

 if(act)
    act->Next = tmp;
 else
    act = tmp;

 tmp->Next = NULL;
 tmp->Info.I1 = x;
 tmp->Info.I2 = y;
 tmp->Info.I3 = z;
 ftail = tmp;
 
 return(1);
}
 
void DestroyPTList(PTList **p)
{
 int nbp = NbPoint(*p);
 int i;
 PTList *tmp,*act;

 for(tmp=*p,i=0; i<nbp ; i++)
    {
     act = tmp->Next;
     FreeList((void *)tmp);
     tmp = act;
    }

 ptail = NULL;
 *p = NULL;
}

void ReplaceInFace(OBJ3D *ob,int act,int new)
{
 FACE3D *tmp;
 int i,max;

 for(i=0,max=ob->NBFACE,tmp=ob->FCtable; i<max ; i++)
    {
      if( tmp[i].I1 == act )
         tmp[i].I1 = new;
      else 
        {
         if( tmp[i].I2 == act )
           tmp[i].I2 = new;
         else
            if( tmp[i].I3 == act )
               tmp[i].I3 = new;
        } 
    }
} 
 
 
int BuildPTList(PTList **list,OBJ3D *ob)
{
 int i,nb;
 PT3D *pt;

 if(*list)
    DestroyPTList(list);
 ptail = NULL;   /* never inherit a previous list's tail (M-06) */

 for(nb=ob->NBPT3D,pt=ob->PTtable,i=0;i<nb;i++)
      if( AddPoint(list,pt[i].X,pt[i].Y,pt[i].Z) == -1 )
        {
          DestroyPTList(list);
          return(-1);
        }

 return(1);
}

void DestroyFCList(FCList **p)
{
 int nbf = NbFace(*p);
 int i;
 FCList *tmp,*act;

 for(tmp=*p,i=0; i<nbf ; i++)
    {
     act = tmp->Next;
     FreeList((void *)tmp);
     tmp = act;
    }

 ftail = NULL;
 *p = NULL;
}

int BuildFCList(FCList **list,FACE3D *pt,int nb)
{
 int i;
 
 if(*list)
    DestroyFCList(list);
 ftail = NULL;   /* never inherit a previous list's tail (M-06) */

 *list=NULL;

 for(i=0;i<nb;i++)
    if( AddFace(list,pt[i].I1,pt[i].I2,pt[i].I3) == -1 )
      {
       DestroyFCList(list);
       return(-1);
      }

 return(1);
}


int ReBuildOBJ(OBJ3D **ob,PTList *ptl,FCList *fcl)
{
 int nbp,nbf,i;
 OBJ3D *tmp;
 PTList *p;
 FCList *f;
 int *ftmp;
 float *ptmp;

 tmp=(OBJ3D *)calloc(sizeof(OBJ3D),1);
 if( !tmp )
    return(-1);

 nbf=NbFace(fcl);
 nbp=NbPoint(ptl);

 if( !AllocOBJ3D(tmp,nbp,nbf)  )
   {
    free(tmp);
    return(-1);
   }

 for(ptmp=(float *)tmp->PTtable,p=ptl,i=0; i<nbp ; i++,p=p->Next)
    {
     *ptmp++ =  p->Info.X;
     *ptmp++ =  p->Info.Y;
     *ptmp++ =  p->Info.Z;
    }

 for(ftmp=(int *)tmp->FCtable,f=fcl,i=0; i<nbf ; i++,f=f->Next)
    {
     *ftmp++ =  f->Info.I1;
     *ftmp++ =  f->Info.I2;
     *ftmp++ =  f->Info.I3;
    }

 {
  RGBColor *oldcolor = (*ob)->Color;
  int oldnbf = (*ob)->NBFACE;

  (*ob)->Color = NULL;        /* detach: keep the colors across the rebuild */
  FreeOBJ3D(*ob);

  (*ob)->NBFACE = nbf;
  (*ob)->NBPT3D = nbp;
  (*ob)->PTtable = tmp->PTtable;
  (*ob)->FCtable = tmp->FCtable;
  (*ob)->Color = tmp->Color;  /* freshly sized for nbf (M-03) */
  (*ob)->MMObj1 = NULL;
  (*ob)->MMObj2 = NULL;

  if( oldcolor )
    {
     for(i=0; i<oldnbf && i<nbf ; i++)
         (*ob)->Color[i] = oldcolor[i];
     free(oldcolor);
    }

  tmp->PTtable = 0;
  tmp->FCtable = 0;
  tmp->Color = 0;
  free(tmp);
 }
 return(1); 
}

void DelAllFaceWith(FCList **f,int n)
{
 FCList *fc,*tmp;
 int max=NbFace(*f);
 int del=0;

 for(fc=*f; fc ; fc=tmp)
     if( fc->Info.I1 == n || fc->Info.I2 == n || fc->Info.I3 == n )
       { 
        tmp = fc->Next;
        DelFace(f,fc->Info.I1,fc->Info.I2,fc->Info.I3); 
        del = 1;  
        }
      else
        tmp = fc->Next;

 max = NbFace(*f);
 for(fc=*f; fc ; fc=fc->Next)
    if(del) 
      {
        if(fc->Info.I1 > n)
           fc->Info.I1--;   
        if(fc->Info.I2 > n)
           fc->Info.I2--;
        if(fc->Info.I3 > n)
           fc->Info.I3--;
      }
}

void FindMinMax(OBJ3D *ob)
{
 int i;
 float *ptmp;
 float mx,my,mz,mix,miy,miz;

 mx = my = mz = mix = miy = miz = 0.0;

 for(ptmp=(float *)ob->PTtable,i=ob->NBPT3D; i ; i-- )
    {
     if( mx < *ptmp )
        mx = *ptmp;
     else 
       if(mix > *ptmp )
          mix = *ptmp;
     ptmp++;

     if( my < *ptmp )
        my = *ptmp;
     else
       if( miy > *ptmp )
          miy = *ptmp;
     ptmp++;

     if( mz < *ptmp )
        mz = *ptmp;
     else
       if(miz > *ptmp )
          miz = *ptmp;
     ptmp++; 
    }

 ob->Min.X = mix;
 ob->Min.Y = miy;
 ob->Min.Z = miz;

 ob->Max.X = mx;
 ob->Max.Y = my;
 ob->Max.Z = mz;
}

OBJ3D *BuildMinMaxOBJ(OBJ3D *parent)
{
 OBJ3D *ret;
 float maxX,maxY,maxZ,minX,minY,minZ;
 float *ptmp;
 int *ftmp;

 FindMinMax(parent);

 ret = CreateOBJ3D();
 if( ret == NULL )
    return(NULL);
 else
     if( AllocOBJ3D(ret,8,12) == 0 )
       {
        free(ret);
        return(NULL);
       }
 

 maxX = parent->Max.X;  minX = parent->Min.X;
 maxY = parent->Max.Y;  minY = parent->Min.Y;
 maxZ = parent->Max.Z;  minZ = parent->Min.Z;

 ptmp = (float *)ret->PTtable;
 ftmp = (int *)ret->FCtable;

 /* Init the point coefficients in order X,Y,Z */
 *ptmp++ = minX;  *ptmp++ = maxY;  *ptmp++ = maxZ;
 *ptmp++ = maxX;  *ptmp++ = maxY;  *ptmp++ = maxZ;
 *ptmp++ = maxX;  *ptmp++ = minY;  *ptmp++ = maxZ;
 *ptmp++ = minX;  *ptmp++ = minY;  *ptmp++ = maxZ;
 *ptmp++ = minX;  *ptmp++ = maxY;  *ptmp++ = minZ;
 *ptmp++ = maxX;  *ptmp++ = maxY;  *ptmp++ = minZ;
 *ptmp++ = maxX;  *ptmp++ = minY;  *ptmp++ = minZ;
 *ptmp++ = minX;  *ptmp++ = minY;  *ptmp++ = minZ;

/*And now the face: I1,I2,I3 (see the VertexStruct.h) */
 *ftmp++=0;  *ftmp++=1;  *ftmp++=1;
 *ftmp++=1;  *ftmp++=2;  *ftmp++=2;
 *ftmp++=2;  *ftmp++=3;  *ftmp++=3;
 *ftmp++=3;  *ftmp++=0;  *ftmp++=0;
 *ftmp++=4;  *ftmp++=0;  *ftmp++=0;
 *ftmp++=3;  *ftmp++=7;  *ftmp++=7;
 *ftmp++=2;  *ftmp++=6;  *ftmp++=6;
 *ftmp++=1;  *ftmp++=5;  *ftmp++=5;
 *ftmp++=4;  *ftmp++=5;  *ftmp++=5;
 *ftmp++=5;  *ftmp++=6;  *ftmp++=6;
 *ftmp++=6;  *ftmp++=7;  *ftmp++=7;
 *ftmp++=7;  *ftmp++=4;  *ftmp++=4;

 return(ret);
} 
    
int CopyOBJ3D(OBJ3D *src,OBJ3D *dest)
{
 int nbf,nbpt;
 OBJ3D *ob1,*ob2;

 ob1 = src;
 ob2 = dest;

 if(ob1 == NULL || ob2 == NULL)
    return(0);

 nbf = ob1->NBFACE;
 nbpt= ob1->NBPT3D;

 if( ob2->NBFACE == nbf && ob2->NBPT3D == nbpt )
   {
    memcpy(ob2->PTtable,ob1->PTtable,nbpt*sizeof(PT3D));
    memcpy(ob2->FCtable,ob1->FCtable,nbf*sizeof(FACE3D));
    return(1);
   }
 else
   return(0);
} 


void PerformGC(OBJ3D *obj)
{
 register int i,nbpt;
 register float *ptmp,X,Y,Z;

 if( obj == NULL)
    return;

 X = Y = Z = 0.0;
 ptmp = (float *)obj->PTtable;

 for(i=0,nbpt = obj->NBPT3D; i<nbpt ; i++)
    {
     X += (*ptmp++)/(float)nbpt;
     Y += (*ptmp++)/(float)nbpt;
     Z += (*ptmp++)/(float)nbpt; 
    }

 obj->Center.X = X;
 obj->Center.Y = Y;
 obj->Center.Z = Z;
}

PT3D GetNormal(PT3D pt[3])
{
 register PT3D res,v1,v2;
 register float Angle;
 v1.X = pt[1].X-pt[0].X; v1.Y = pt[1].Y-pt[0].Y; v1.Z = pt[1].Z-pt[0].Z;
 v2.X = pt[2].X-pt[1].X; v2.Y = pt[2].Y-pt[1].Y; v2.Z = pt[2].Z-pt[1].Z;

 res.X = v1.Y * v2.Z - v1.Z * v2.Y;
 res.Y = v1.Z * v2.X - v1.X * v2.Z;
 res.Z = v1.X * v2.Y - v1.Y * v2.X;

 return(res);
}


float DotProduct(PT3D v1,PT3D v2)
{
 register float temp;

 if((v1.X == 0.0 && v1.Y == 0.0))
    return(acos(0.0));

 temp = sqrt(v1.X * v1.X + v1.Y * v1.Y );
 if(temp != 0.0)
   {
    v1.X = v1.X / temp;
    v1.Y = v1.Y / temp;
   }

 temp = sqrt(v2.X * v2.X + v2.Y * v2.Y );
 if(temp != 0.0 )
   {
    v2.X = v2.X / temp;
    v2.Y = v2.Y / temp;
   }

 temp = v1.X*v2.X + v1.Y*v2.Y;

 if(temp >=0.0)
    temp = fmod(temp, 1.0);
 if(temp < 0.0)
    temp = -fmod(-temp, 1.0);

 return(acos(temp));
}

void FlipNormal(OBJ3D *ob)
{
 int i,nbf;
 int tmp;
 FACE3D *ftmp;
 
 nbf = ob->NBFACE;
 ftmp = ob->FCtable;
 for(i=0; i<nbf ; i++)
    {
     tmp = ftmp[i].I2;
     ftmp[i].I2 = ftmp[i].I1;
     ftmp[i].I1 = tmp;
    }
}

void VertexFlipNormal()
{
 char *buff;
 int pl;
 OBJ3D *ob;

 if( (pl=GetActiveListItem(objlist,&buff)) == -1)
   {
     SetTextString(msg,"Select One Object Before Normal Flipping!");
     return;
   }
 SetTextString(msg,"Normal Flipped");
 ob = FindNamedOBJ(buff,pl);
 if( ob == NULL )
    return;   /* stale list selection (M-09) */
 FlipNormal(ob);
}


void UnifyObject(OBJ3D *obj,PT3D L)
{
 register int i,tmp,r;
 PT3D Normal,pt[3];
 float ang;

 for(r=0 ; r<4; r++ )
    {
    RotatePT3D((float *)obj->PTtable,obj->NBPT3D,0.0,90.0,0.0);
    for(i=0 ; i<obj->NBFACE ; i++ )
       {
        pt[0] = obj->PTtable[obj->FCtable[i].I1];
        pt[1] = obj->PTtable[obj->FCtable[i].I2];
        pt[2] = obj->PTtable[obj->FCtable[i].I3];
        Normal = GetNormal(pt);
        ang = L.X * Normal.X + L.Y * Normal.Y + L.Z * Normal.Z;

        if(ang < 0.0 )
          {
              tmp =obj->FCtable[i].I1;
              obj->FCtable[i].I1 = obj->FCtable[i].I2;
              obj->FCtable[i].I2 = tmp;
          } 
       }       
   } 
}

void VertexNormalUnify()
{
 char *buff;
 int pl;
 OBJ3D *ob;
 PT3D tmp={ 0.0 , maxy , minz };

 if( (pl=GetActiveListItem(objlist,&buff)) == -1)
   {
     SetTextString(msg,"Select One Object Before Unify Object!");
     return;
   }
 SetTextString(msg,"Object Normal Unified");
 ob = FindNamedOBJ(buff,pl);
 if( ob == NULL )
    return;   /* stale list selection (M-09) */
 UnifyObject(ob,tmp);
}


