/******************************************/
/*                Vertex 3D               */
/*            Ray Tracing Module          */
/*               Version 1.0              */
/*----------------------------------------*/
/* Concept And Design:C.LEGENDRE Nov 1995 */
/******************************************/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"VRStruct.h"
#include"BVH.h"
#include<math.h>

extern int nobj;
extern OBJ obj[MAX_OBJ];
extern VECTOR eye,eye_dir,up;
extern LIGHT light[MAX_LIGHT];
extern int nobj,nosph,nosq;
extern int nlight,bgflag;
extern VDBL fov;
extern void MakeMapping(OBJ *,char *);

void ParseInf(FILE *lin,const char *msg)
{
 char buff[64];
 
 if( fscanf(lin,"%63s",buff) != 1 ) buff[0]=0;
 if( buff[0] !='<' )
   {
    fprintf(stderr,"Parse '<' Error: %s\n",msg);
    exit(0);
   }
}

void ParseSup(FILE *lin,const char *msg)
{
 char buff[64];

 if( fscanf(lin,"%63s",buff) != 1 ) buff[0]=0;
 if( buff[0] !='>' )
   {
    fprintf(stderr,"Parse '>' Error: %s\n",msg);
    exit(0);
   }
}

int ParseInteger(FILE *lin,char *msg)
{
 int val;

 if( fscanf(lin,"%d",&val) != 1)
   {
     fprintf(stderr,"Parse Integer Error: %s\n",msg);
     exit(0);
    }

 return(val);
}

VDBL ParseFloat(FILE *lin,const char *msg)
{
 VDBL res;

 if( fscanf(lin,VDBL_FORMAT,&res) != 1 )
   {
    fprintf(stderr,"Parse Float Error: %s\n",msg);
    exit(0);
   }
 
 return(res);
}

COLOR ParseColor(FILE *lin,const char *msg)
{
 COLOR col;
 
 col.R = ParseFloat(lin,msg);
 col.G = ParseFloat(lin,msg);
 col.B = ParseFloat(lin,msg); 

 return(col);
}


void ParseSpecular(FILE *lin)
{
 ParseInf(lin,"SPECULAR BEGIN");
 obj[nobj].specular = ParseColor(lin,"SPECULAR");
 ParseSup(lin,"SPECULAR END ");
}

void ParseReflection(FILE *lin)
{
 ParseInf(lin,"REFLECTION BEGIN");
 obj[nobj].reflection = ParseFloat(lin,"REFLECTION");
 obj[nobj].refl_color = ParseColor(lin,"REFLECTION");
 ParseSup(lin,"REFLECTION END");
}

void ParseDiffuse(FILE *lin)
{
 ParseInf(lin,"DIFFUSE BEGIN");
 obj[nobj].diffuse = ParseColor(lin,"DIFFUSE");
 ParseSup(lin,"DIFFUSE END");
}

void ParseRefraction(FILE *lin)
{
 ParseInf(lin,"REFRACTION");
 obj[nobj].refraction = ParseFloat(lin,"REFRACTION");
 obj[nobj].refr_color = ParseColor(lin,"REFRACTION");
 ParseSup(lin,"REFRACTION");
}

void ParseAmbient(FILE *lin)
{
 ParseInf(lin,"AMBIENT BEGIN");
 obj[nobj].ambient = ParseColor(lin,"AMBIENT");
 ParseSup(lin,"AMBIENT END");
}

void ParseReflDiffuse(FILE *lin)
{
 obj[nobj].refl_diffuse = ParseFloat(lin,"REFLEC_DIFFUSE");
 obj[nobj].refl_diffuse *= M_PI/180.0;
}
    
void ParseRefrDiffuse(FILE *lin)
{
 obj[nobj].refr_diffuse = ParseFloat(lin,"REFRAC_DIFFUSE");
 obj[nobj].refr_diffuse *= M_PI/180.0;
}

void ParseWidth(FILE *lin)
{
 obj[nobj].width = ParseFloat(lin,"WIDTH");
}

void ParseIndex(FILE *lin)
{
 obj[nobj].obindex = ParseFloat(lin,"INDEX"); 
}

void ParseSphere(FILE *lin)
{
 char center,ambient,diffuse,specular;
 char refl,refr,width,index,refld,refrd;
 char map,buff[25];

 if( nobj >= MAX_OBJ )
    {
     fprintf(stderr,"Too many objects (MAX_OBJ=%d)\n",MAX_OBJ);
     exit(0);
    }

 map = center = ambient = diffuse = specular = 1;
 refl = refr = width = index = refrd = refld =1;

 
 obj[nobj].Type=SPHERE_OBJ; 

 ParseInf(lin,"SHPERE BEGIN");
 while( (map||center||ambient||diffuse||specular||refl||refr||width||index||refld||refrd))
      {
       if( fscanf(lin,"%24s",buff) != 1 ) buff[0]=0;
       if( strcmp(buff,"CENTER") == 0 )
         {
          ParseInf(lin,"CENTER BEGIN");
          obj[nobj].data.sphere.Center.X = ParseFloat(lin,"CENTER X");
          obj[nobj].data.sphere.Center.Y = ParseFloat(lin,"CENTER Y");
          obj[nobj].data.sphere.Center.Z = ParseFloat(lin,"CENTER Z");
          obj[nobj].data.sphere.Radius   = ParseFloat(lin,"SPHERE RADIUS"); 
          ParseSup(lin,"CENTER END");
          center=0;  
         }
       else
       if(strcmp(buff,"AMBIENT") == 0 )
         { 
          ParseAmbient(lin); ambient=0;
         }  
       else
       if(strcmp(buff,"DIFFUSE") == 0 )
         {
          ParseDiffuse(lin); diffuse=0;
         }
       else
       if(strcmp(buff,"SPECULAR") == 0 )
         {
          ParseSpecular(lin); specular = 0;
         }
       else
       if(strcmp(buff,"REFLECTION") == 0 )
         {
          ParseReflection(lin); refl=0;
         }   
       else
       if(strcmp(buff,"REFRACTION") == 0 )
         {
          ParseRefraction(lin); refr=0;
         }
       else
       if(strcmp(buff,"WIDTH") == 0 )
         {
          ParseWidth(lin); width=0;
         }
       else
       if(strcmp(buff,"INDEX") == 0 )
         {
          ParseIndex(lin); index=0;                     
         }
       else
       if(strcmp(buff,"REFLEC_DIFFUSE") == 0 )
         {
          ParseReflDiffuse(lin); refld=0; 
         }          
       else
       if(strcmp(buff,"REFRAC_DIFFUSE") == 0 )
         {
          ParseRefrDiffuse(lin);
          refrd=0;
         }
       else 
       if( strcmp(buff,"MAPPING") == 0 )
         {
          if( fscanf(lin,"%24s",buff) != 1 ) buff[0]=0;
          if( strcmp(buff,"NONE") == 0 )
             obj[nobj].Mapping = 0;
          else
           {
            obj[nobj].Mapping = 1;
            obj[nobj].map.Type = SPHERICAL;
            MakeMapping(&obj[nobj],buff);
           }
          map=0;
         }
       else
         {
          fprintf(stderr,"Sphere Parse Error\n");  
          exit(0);
         }
      }

 ParseSup(lin,"END SPHERE");
 nobj++;
}
 
void ParsePlane(FILE *lin)
{
 char point,ambient,diffuse,specular;
 char refl,refr,width,index,refld,refrd;
 char map,buff[25];

 if( nobj >= MAX_OBJ )
    {
     fprintf(stderr,"Too many objects (MAX_OBJ=%d)\n",MAX_OBJ);
     exit(0);
    }

 map = point = ambient = diffuse = specular = 1;
 refl = refr = width = index = refrd = refld =1;

 obj[nobj].Type = PLANE_OBJ;
 
 ParseInf(lin,"PLANE BEGIN");
 while( (map||point||ambient||diffuse||specular||refl||refr||width||index||refld||refrd) )
      {
       if( fscanf(lin,"%24s",buff) != 1 ) buff[0]=0;

       if( strcmp(buff,"BASE") == 0 )
         {
          ParseInf(lin,"BASE BEGIN");
          obj[nobj].data.quad.p1.X = ParseFloat(lin,"BASE Point 1 X");
          obj[nobj].data.quad.p1.Y = ParseFloat(lin,"BASE Point 1 Y");
          obj[nobj].data.quad.p1.Z = ParseFloat(lin,"BASE Point 1 Z");
          obj[nobj].data.quad.p2.X = ParseFloat(lin,"BASE Point 2 X");
          obj[nobj].data.quad.p2.Y = ParseFloat(lin,"BASE Point 2 Y");
          obj[nobj].data.quad.p2.Z = ParseFloat(lin,"BASE Point 2 Z");
          obj[nobj].data.quad.p3.X = ParseFloat(lin,"BASE Point 3 X");
          obj[nobj].data.quad.p3.Y = ParseFloat(lin,"BASE Point 3 Y");
          obj[nobj].data.quad.p3.Z = ParseFloat(lin,"BASE Point 3 Z");
          ParseSup(lin,"BASE END");
          point=0;
         }
       else
       if( strcmp(buff,"AMBIENT") == 0 )
         {
          ParseAmbient(lin);
          ambient=0;
         }
       else
       if(strcmp(buff,"DIFFUSE") == 0 )
         {
          ParseDiffuse(lin); diffuse=0;
         }
       else
       if(strcmp(buff,"SPECULAR") == 0 )
         {
          ParseSpecular(lin); specular = 0;
         }
       else
       if(strcmp(buff,"REFLECTION") == 0 )
         {
          ParseReflection(lin); refl=0;
         }
       else
       if(strcmp(buff,"REFRACTION") == 0 )
         {
          ParseRefraction(lin); refr=0;
         }
       else
       if(strcmp(buff,"WIDTH") == 0 )
         {
          ParseWidth(lin); width=0;
         }
       else
       if(strcmp(buff,"INDEX") == 0 )
         {
          ParseIndex(lin); index=0;
         }
       else
       if(strcmp(buff,"REFLEC_DIFFUSE") == 0 )
         {
          ParseReflDiffuse(lin); refld=0;
         }
       else
       if(strcmp(buff,"REFRAC_DIFFUSE") == 0 )
         {
          ParseRefrDiffuse(lin);
          refrd=0;
         }
       else
        if( strcmp(buff,"MAPPING") == 0 )
         {
          if( fscanf(lin,"%24s",buff) != 1 ) buff[0]=0;
          if( strcmp(buff,"NONE") == 0 )
             obj[nobj].Mapping = 0;
          else
           {
            obj[nobj].Mapping = 1;
            obj[nobj].map.Type = PLANAR;
            MakeMapping(&obj[nobj],buff);
           }
          map=0;
         }
       else
         {
          fprintf(stderr,"Plane Parse Error\n");
          exit(0);
         }
      }

 ParseSup(lin,"END PLANE");
 nobj++;
}
 

void ParseTriangle(FILE *lin)
{
 char vertex,ambient,diffuse,specular;
 char refl,refr,width,index,refld,refrd;
 char map,buff[25];

 if( nobj >= MAX_OBJ )
    {
     fprintf(stderr,"Too many objects (MAX_OBJ=%d)\n",MAX_OBJ);
     exit(0);
    }

 map = vertex = ambient = diffuse = specular = 1;
 refl = refr = width = index = refrd = refld =1;

 obj[nobj].Type = TRIANGLE_OBJ;

 ParseInf(lin,"TRIANGLE BEGIN");
 while( (map||vertex||ambient||diffuse||specular||refl||refr||width||index||refld||refrd) )
      {
       if( fscanf(lin,"%24s",buff) != 1 ) buff[0]=0;

       if( strcmp(buff,"VERTICES") == 0 )
         {
          ParseInf(lin,"VERTICES BEGIN");
          obj[nobj].data.triangle.p1.X = ParseFloat(lin,"VERTICES Point 1 X");
          obj[nobj].data.triangle.p1.Y = ParseFloat(lin,"VERTICES Point 1 Y");
          obj[nobj].data.triangle.p1.Z = ParseFloat(lin,"VERTICES Point 1 Z");
          obj[nobj].data.triangle.p2.X = ParseFloat(lin,"VERTICES Point 2 X");
          obj[nobj].data.triangle.p2.Y = ParseFloat(lin,"VERTICES Point 2 Y");
          obj[nobj].data.triangle.p2.Z = ParseFloat(lin,"VERTICES Point 2 Z");
          obj[nobj].data.triangle.p3.X = ParseFloat(lin,"VERTICES Point 3 X");
          obj[nobj].data.triangle.p3.Y = ParseFloat(lin,"VERTICES Point 3 Y");
          obj[nobj].data.triangle.p3.Z = ParseFloat(lin,"VERTICES Point 3 Z");
          ParseSup(lin,"VERTICES END");
          vertex=0;
         }
       else
       if( strcmp(buff,"AMBIENT") == 0 )
         {
          ParseAmbient(lin);
          ambient=0;
         }
       else
       if(strcmp(buff,"DIFFUSE") == 0 )
         {
          ParseDiffuse(lin); diffuse=0;
         }
       else
       if(strcmp(buff,"SPECULAR") == 0 )
         {
          ParseSpecular(lin); specular = 0;
         }
       else
       if(strcmp(buff,"REFLECTION") == 0 )
         {
          ParseReflection(lin); refl=0;
         }
       else
       if(strcmp(buff,"REFRACTION") == 0 )
         {
          ParseRefraction(lin); refr=0;
         }
       else
       if(strcmp(buff,"WIDTH") == 0 )
         {
          ParseWidth(lin); width=0;
         }
       else
       if(strcmp(buff,"INDEX") == 0 )
         {
          ParseIndex(lin); index=0;
         }
       else
       if(strcmp(buff,"REFLEC_DIFFUSE") == 0 )
         {
          ParseReflDiffuse(lin); refld=0;
         }
       else
       if(strcmp(buff,"REFRAC_DIFFUSE") == 0 )
         {
          ParseRefrDiffuse(lin);
          refrd=0;
         }
       else
        if( strcmp(buff,"MAPPING") == 0 )
         {
          if( fscanf(lin,"%24s",buff) != 1 ) buff[0]=0;
          if( strcmp(buff,"NONE") == 0 )
             obj[nobj].Mapping = 0;
          else
           {
            obj[nobj].Mapping = 1;
            obj[nobj].map.Type = PLANAR;
            MakeMapping(&obj[nobj],buff);
           }
          map=0;
         }
       else
         {
          fprintf(stderr,"Triangle Parse Error\n");
          exit(0);
         }
      }

 ParseSup(lin,"END TRIANGLE");
 nobj++;
}
 

void ParseEye(FILE *lin)
{
 char buff[25]; 
 char pos,dup,dir;
 
 pos=dup=dir=1;
 
 ParseInf(lin,"EYE BEGIN");

 while( pos||dup||dir)
      {
       if( fscanf(lin,"%24s",buff) != 1 ) buff[0]=0;
 
       if( strcmp(buff,"POS") == 0 )
         {
          ParseInf(lin,"EYE POS");
          eye.X = ParseFloat(lin,"EYE POS X");
          eye.Y = ParseFloat(lin,"EYE POS Y");
          eye.Z = ParseFloat(lin,"EYE POS Z"); 
          ParseSup(lin,"EYE POS");
          pos=0;
         }
       else
       if(strcmp(buff,"DIR") == 0 )
         {
          ParseInf(lin,"EYE DIR");
          eye_dir.X = ParseFloat(lin,"EYE DIR X");
          eye_dir.Y = ParseFloat(lin,"EYE DIR Y");
          eye_dir.Z = ParseFloat(lin,"EYE DIR Z");
          ParseSup(lin,"EYE DIR");
          dir=0;
         }
       else
       if( strcmp(buff,"UP") == 0 )
         {
          ParseInf(lin,"EYE UP");
          up.X = ParseFloat(lin,"EYE UP X");
          up.Y = ParseFloat(lin,"EYE UP Y");
          up.Z = ParseFloat(lin,"EYE UP Z");
          ParseSup(lin,"EYE UP");
          dup=0;
         }
       else
         {
          fprintf(stderr,"EYE Parse Error\n");
          exit(0);
         }
      }

 ParseSup(lin,"END EYE");
} 
       
void ParseScene(FILE *lin)
{
 char buff[25];
 
 ParseInf(lin,"SCENE BEGIN");
 if( fscanf(lin,"%24s",buff) != 1 ) buff[0]=0;
 if( strcmp(buff,"FOV") == 0 )
   {
    fov = ParseFloat(lin,"SCENE BEGIN"); 
    fov = tan( fov * M_PI / 180 ) / sqrt(2.0); 
   }
 else
   {
    fprintf(stderr,"Parse Scene Error\n");
    exit(0);
   }

 ParseSup(lin,"SCENE END");
}
 
void ParseLight(FILE *lin)
{
 char buff[25];
 char ori,ang;
 
 ori=ang=1;

 ParseInf(lin,"LIGHT BEGIN");
 while( ori||ang )
      {
       if( fscanf(lin,"%24s",buff) != 1 ) buff[0]=0;       
       if( strcmp(buff,"ORIGIN") == 0 )
         {
          ParseInf(lin,"LIGHT ORIGIN"); 
          light[0].org.X = ParseFloat(lin,"LIGHT ORIGIN X");
          light[0].org.Y = ParseFloat(lin,"LIGHT ORIGIN Y");
          light[0].org.Z = ParseFloat(lin,"LIGHT ORIGIN Z");
          ParseSup(lin,"LIGHT ORIGIN");
          ori=0;
         }
       else
       if(strcmp(buff,"ANGLE") == 0 )
         {
          light[0].angle = ParseFloat(lin,"LIGHT ANGLE");
          light[0].angle *= M_PI/180.0;  
          ang=0;
         }  
       else
         {
          fprintf(stderr,"Parse Light Error\n");
          exit(0);
         }
      }
 
 ParseSup(lin,"LIGHT END");
}
     
void ParseFile(char *name)
{
 char eye,scene,light,obj,buff[25]; 
 FILE *in;

 if( (in=fopen(name,"r")) == 0  )
   {
    fprintf(stderr,"No Input File\n");
    exit(0);
    }

 eye=scene=light=obj=1;

 while( (feof(in) == 0) )
 {
  if( fscanf(in,"%24s",buff) != 1 )
     {
      fprintf(stderr,"Parse End\n");
      if( obj )
        {
         fprintf(stderr,"No Object To Render\n");
         exit(0);
        }
      else
      if( scene )
        {
         fprintf(stderr,"No Scene\n");
         exit(0);
        }
      else
      if( light )
        {
         fprintf(stderr,"No Light\n");
         exit(0);
        }
      else
      if( eye )
        {
         fprintf(stderr,"No Eye\n");
         exit(0);
        }
         

      return;
     } 

  fprintf(stderr,"Parse Obj=%d .... ",nobj);
  fflush(stderr);
 
  if( strcmp(buff,"SPHERE") == 0)
    {
     ParseSphere(in); fprintf(stderr,"Another Sphere! \n"); obj=0;
    }
  else
  if( strcmp(buff,"PLANE") == 0)
    {
     ParsePlane(in);  fprintf(stderr,"Another Plane! \n"); obj=0;
    }
  else
  if( strcmp(buff,"TRIANGLE") == 0)
    {
     ParseTriangle(in);  fprintf(stderr,"Another Triangle! \n"); obj=0;
    }
  else
  if( strcmp(buff,"EYE") == 0 )
    {
     ParseEye(in);  fprintf(stderr,"Eye!\n"); eye=0;
    }
  else
  if( strcmp(buff,"LIGHT") == 0 )
    {
     ParseLight(in); fprintf(stderr,"Light!\n"); light=0;
    }
  else
  if( strcmp(buff,"SCENE") == 0 )
    {
     ParseScene(in); fprintf(stderr,"Scene!\n"); scene=0;
    }
  else
    {
     fprintf(stderr,"Unknow Object: %s\n",buff);
     if( nobj != 0 )
        fprintf(stderr,"Try To Find Another...\n");         
     else
        exit(0);
    }
 }

 fclose(in);
 BuildBVH();   /* tree for the parsed scene (no-op for tiny scenes) */
}
  
