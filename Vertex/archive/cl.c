#include <stdlib.h>
#include <alloc.h>
#include <stru_pro.h>
#include <vector.h>
/**************************************************************************/
/* INIT_OBJET : initialise … '0' tous les champs d'une structure objet    */
/**************************************************************************/

void init_objet(OBJET *obj)
{
	obj->name[0]='\0';
	obj->nbpoints=0;
	obj->ptrpoints=NULL;
	obj->nbfaces=0;
	obj->ptrfaces=NULL;
	obj->alpha=obj->beta=obj->phi=0;
	obj->cx=obj->cy=obj->cz=0;
	obj->s=0;
}


/**************************************************************************/
/* ERASE : Libere la memoire occupee par les composants d'un objet        */
/**************************************************************************/

void erase(OBJET *obj)
{
WORD i;

/* Libere les tableaux de liens de chaque faces */
for (i=0;i<obj->nbfaces;i++)
   if (obj->ptrfaces[i].liens!=NULL)
      farfree(obj->ptrfaces[i].liens);

/* Libere le tableau des faces */
if (obj->ptrfaces!=NULL)
   farfree(obj->ptrfaces);

/* Libere le tableau des points 3D */
if (obj->ptrpoints!=NULL)
   farfree(obj->ptrpoints);

/* Initialise tous les champs */
init_objet(obj);
}


/**************************************************************************/
/* CLEAN 1 FACE : supprime les liens nuls (lien d'un point a lui meme)    */
/*  d'une face d'un objet                                                 */
/**************************************************************************/

void clean1face(OBJET *obj,int numface)
{

int i,j,k,l,newnbliens;
VECT pointa,pointb;

newnbliens=obj->ptrfaces[numface].nbpoints;

l=0;
i=0;

while (i<newnbliens-1-l)
	{
	pointa=obj->ptrpoints[obj->ptrfaces[numface].liens[i-l]];
	j=i+1;
	while(j<newnbliens-l)
		{
		pointb=obj->ptrpoints[obj->ptrfaces[numface].liens[j]];
		/* compare les liens 2 … 2 */
		if (sont_identique(&pointa,&pointb))
			{
			/* si lien identique a l'actuel */
			for (k=j-l;k<newnbliens-l;k++)
				/* on l'enl‚ve */
				obj->ptrfaces[numface].liens[k]=obj->ptrfaces[numface].liens[k+1];
			l++; /* nb de liens deja enlev‚ */
			}
		j++;
		}
	i++;
	}
/* r‚ajuste la taille m‚moire occup‚e */
farrealloc(obj->ptrfaces[numface].liens,(newnbliens-l)*sizeof(int));
/* mise … jour du nombre de liens dans la face */
obj->ptrfaces[numface].nbpoints=newnbliens-l;

}


/**************************************************************************/
/* CLEANFACES : supprime les liens nuls pour toutes les faces             */
/**************************************************************************/

void cleanfaces(OBJET *obj)
{
int i;
for (i=0;i<obj->nbfaces;i++)
	clean1face(obj,i);
}


/**************************************************************************/
/* CLEANOBJET : elimine les redondances de points de l'objet et remet a   */
/*  jour toutes les tables de liens                                       */
/**************************************************************************/

void cleanobjet(OBJET *obj)
{

int i,j,k,l,m,n,newnbpoints;
VECT pointa,pointb;
long huge *tabdeja;

newnbpoints=obj->nbpoints;

tabdeja=(long huge*)farmalloc(newnbpoints*sizeof(long));

l=0;

for (i=0;i<newnbpoints;i++)
	tabdeja[i]=-1;


/* marquage des points identiques */
for (i=0;i<obj->nbpoints-1;i++)
	{
	/* si le point n'est pas d‚j… marqu‚s */
	if (tabdeja[i]==-1)
		{
		pointa=obj->ptrpoints[i];
		for (j=i+1;j<obj->nbpoints;j++)
			{
			pointb=obj->ptrpoints[j];
			/* si les points sont identiques, */
			if (sont_identique(&pointa,&pointb))
			/* on les marque du meme indice */
				tabdeja[j]=i;
			}
		}
	}


/* on r‚affecte les tables des liens avec les indices trouv‚s */
for (i=0;i<obj->nbpoints;i++)
	{
	if (tabdeja[i]!=-1)	/* si points  identique en plusieurs exemplaire */
		{
		for (m=0;m<obj->nbfaces;m++)
				for (n=0;n<obj->ptrfaces[m].nbpoints;n++)
					{
					if (obj->ptrfaces[m].liens[n]==i)
						obj->ptrfaces[m].liens[n]=tabdeja[i];
						/* on ne leur affecte plus qu'une seule r‚f‚rence */
					}
		}
	}

/* comme on va supprimer des points on remet … jour les tables de liens */
for (i=0;i<obj->nbpoints;i++)
	{
	if (tabdeja[i]!=-1)
		{
		for (m=0;m<obj->nbfaces;m++)
			for (n=0;n<obj->ptrfaces[m].nbpoints;n++)
				{
				/* si le lien n'est plus valide */
				if (obj->ptrfaces[m].liens[n]>i-l)
					obj->ptrfaces[m].liens[n]--; /* mise … jour */
				}
		l++; /* nombre de point deja enlev‚s (virtuellement) */
		}
	}

l=0;
i=0;

/* on supprime physiquement les points redondants dans la liste */
for (i=0;i<obj->nbpoints;i++)
	{
	if (tabdeja[i]!=-1) /* si points redondants */
		{
		k=i-l;
		for (k=i-l;k<newnbpoints-1;k++)
				/*suppression en d‚calant la liste */
				obj->ptrpoints[k]=obj->ptrpoints[k+1];
		l++; /* nombre de points d‚j… supprim‚s */
		newnbpoints--;	/* nouveau nombre de points */
		}
	}

obj->nbpoints=newnbpoints;


/* r‚ajustement de la m‚moire occup‚e */
farrealloc(obj->ptrpoints,obj->nbpoints*sizeof(VECT));

farfree(tabdeja);
}




int face_adjacente_non_marquee(FACE *ptrfaces,int nbfaces,int indice,int arete)
{
 int i,j=0;
 int nbsommets;
 int a1or=ptrfaces[indice].liens[arete];
 int a1ex=ptrfaces[indice].liens[(arete+1)%ptrfaces[indice].nbpoints];
 int a2or;
 int a2ex;

 for (j=0;j<nbfaces;j++)							/* Pour chaque face */
      if(j!=indice && ptrfaces[j].s!=0) /* Face diff‚rente et non marqu‚e */
	{
	 nbsommets=ptrfaces[j].nbpoints;

	 for(i=0;i<nbsommets;i++)		/* Pour chaque sommet */
	    {
	     a2or=ptrfaces[j].liens[i];
	     a2ex=ptrfaces[j].liens[(i+1)%nbsommets];
	     if(a2or==a1or && a2ex==a1ex) return -j;
		if (a2or==a1ex && a2ex==a1or) return j;
	    }
	}
 return indice;
}


void unifier_normale(OBJET *obj,int indiceface)
{
 int	nb_aretes=obj->ptrfaces[indiceface].nbpoints;
 int indice;


 if (obj->ptrfaces[indiceface].s==0)
    return;		/* Si face d‚j… visit‚e -> fin */
 else
   obj->ptrfaces[indiceface].s=0;		/* Face -> visit‚e */

 while (nb_aretes--)	/* Visiter chaque face adjacente*/
      if ((indice=face_adjacente_non_marquee((FACE*)obj->ptrfaces
           ,obj->nbfaces,indiceface,nb_aretes))!=indiceface)
	{
         if(indice<0)
            inverse_vecteur_normal(obj,indice*=-1);
         unifier_normale(obj,indice);
	}

}



void unifier_normales_objet(OBJET *obj)
{

	int i;
	for (i=0;i<obj->nbfaces;i++)
		obj->ptrfaces[i].s=1; 
	unifier_normale(obj,0);	/* Uniformise toutes les faces par */
											/* rapport … l'indice de la face en paramŠtre */
}

