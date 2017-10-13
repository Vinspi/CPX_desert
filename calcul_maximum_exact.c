//#include "util.h"

//Teste donc je mets tout ici parce que je sais pas faire un makefile.
//___________________________________________________________
#include <stdio.h>
#include <stdlib.h>
#define n_max 1000

typedef int sommet;

typedef struct {
	int a[n_max][n_max];
	int n;
	int degre[n_max];
} Graph;

typedef struct maillon{
	sommet st;
	struct maillon *suiv;
} maillon;

typedef maillon *liste;


//Fontion initGraphe : initialise les champs de la structure representant le graphe en mémoire
//[Param IN]	Graph *g : pointeur vers la structure reprensentant le graphe en mémoire

void initGraph(Graph *g, int n){
	for(int i=0; i<n_max; i++){
		for(int j=0; j<n_max; j++){
			g->a[i][j] = 0;
		}
		g->degre[i] = 0;
	}
	g->n = n;
}


//Fonction printGraph : écrit la représentation matricielle du graphe dans un fichier
//[Param IN]	char *filename : le nom du fichier dans lequel écrire
//[Param IN]	Graph *g : pointeur vers la structure reprensentant le graphe en mémoire


void printGraph(char* filename, Graph *g){
	FILE *out = NULL;
	out = fopen(filename, "w+");

	if(out == NULL)
		exit(15);

	fprintf(out, "        |");

	for(int i=0; i<g->n; i++)
		fprintf(out, "s%d |", i);

	fprintf(out, "\n");

	for(int t=0; t<g->n+2; t++){
			fprintf(out, "----");
	}

	for(int i=0; i<g->n; i++){
		fprintf(out, "\n s%d     | ", i);
		for(int j=0; j<g->n; j++){
			fprintf(out, "%d | ", g->a[i][j]);
		}
		fprintf(out, "\n");
		for(int t=0; t<g->n+2; t++){
			fprintf(out, "----");
		}
	}

	fclose(out);
	printf("\n[Ecriture fichier terminée : %s]\n", filename);
}


//Fontion loadSource : construit le graphe (representation matricielle) à partir du fichier
//[Param IN]	char *filename : le nom du fichier contenant la liste des aretes
//[Param IN]	Graph *g : pointeur vers la structure reprensentant le graphe en mémoire

void loadSource(char* filename, Graph *g){

	FILE *source = NULL;
	source = fopen(filename, "r");

	if(source == NULL)
		exit(10);

	char nbSt_str[10], nbAr_str[10], from_str[10], to_str[10];
	int nbSt, nbAr, from, to;

	fscanf(source, "%s %s", nbSt_str, nbAr_str);

	nbSt = atoi(nbSt_str);
	nbAr = atoi(nbAr_str);

	initGraph(g, nbSt);

    printf("Nombre de sommets : %d\n", nbSt);
	printf("Nombre d'aretes : %d\n", nbAr);

	while(fscanf(source, "%s %s", from_str, to_str) != EOF){
		from = atoi(from_str);
		to = atoi(to_str);

		if(g->a[from][to] != 1 && g->a[to][from] != 1){
			g->degre[from] = g->degre[from] + 1;
			g->degre[to] = g->degre[to] + 1;

		}
		if(g->a[from][to] != 1){
			g->a[from][to] = 1;
		}

		if(g->a[to][from] != 1){
			g->a[to][from] = 1;
		}




	}

	fclose(source);
}

void printf_liste(liste x){

	liste tmp = x; 	int i = 0;
	printf("[ ");
	while(tmp != NULL){
		printf("%d ",tmp->st);
		tmp = tmp->suiv;
		i++;
	}
	printf("], taille %d.\n",i);
}

//Ajoute un sommet a à la liste x.
liste add_to_liste(liste x, sommet a){
	liste tmp = malloc(sizeof(liste));
	tmp->st = a;
	tmp->suiv = x;
	return tmp;
}

void free_liste(liste x){

	while(x != NULL){
		liste tmp = x;
		x = x -> suiv;
		free(tmp);
	}

}



//_____________________________________________________________

typedef struct {
	int taille;
	liste lx;
} sous_graphe_max;




//Fonction remplissant la liste XS avec l'ensemble des sommets de degré non-marqué minimum, et retourne le degré non-marqué minimum, sauf si tout les sommets restants ont le même degré, auquel cas retourne -1.
int sommets_degre_min(Graph *g, int degre[], sous_graphe_max *xs){

	liste x = NULL;
	int min = n_max, imin = -1, allsame = 1;
	for(int i = 0; i < g->n; i++){
		if(degre[i] > -1 && degre[i] <= min){
			imin = i;
			min = degre[i];
		}
	}


	for(int i = 0; i < g->n; i++){
		if(degre[i] == min){
			//printf("Ajout de %d\n",i);
			x = add_to_liste(x, i);
			xs->taille = (xs->taille) + 1;
		}
		else if(degre[i] > -1 && degre[i] != min){
			allsame = 0;
		}

	}

	xs->lx = x;
	if (allsame == 1) return -1;
	return min;

}



sous_graphe_max* calcul_maximum_exact_rec(Graph *g, int degre[], sous_graphe_max *sgm){


	/*Choisir le nouveau sommet i*/
	sous_graphe_max *lns = malloc(sizeof(sous_graphe_max));
	int d = sommets_degre_min(g,degre,lns);


  //S'il n'y a plus de sommets à ajouter, c'est que sgm est un sous-graphe maximal. On renvoie donc sgm, le sous-graphe maximum.
	if(lns->lx == NULL){
		return sgm;
	}

	//S'il n'y a qu'un seul sommet de degré non-marqué minimum, alors il n'y a pas divergence : il devient inutile de faire des copies des structures.
  //On ajoute simplement le sommet au sous-ensemble de sommets, en effectuant les opérations nécessaires, à savoir incrémentation de la taille, et marquage de l'adjacence.
	if (lns->taille == 1){

		sgm->taille++;
		sgm->lx = add_to_liste(sgm->lx,lns->lx->st);
		/*Mettre à jour les degrés et l'adjacence*/
		degre[lns->lx->st] = -1;
		for(int j=0;j<g->n;j++){
      if(g->a[sgm->lx->st][j] == 1 && degre[j] > -1){
				degre[j] = -1;
				for(int z = 0; z < g->n; z++){
					if(g->a[j][z] == 1){
						(degre[z])--;
					}
				}
			}
		}
		return calcul_maximum_exact_rec(g,degre,sgm);

	}

  //S'il y a plusieurs sommets de degré non-marqué minimum, il y a divergence : on doit explorer toutes les possibilités possibles, et donc tester tout les sommets un à un.
  //... Sauf si l'on tombe sur un cas particulier.
	else {

    //Si le degré minimum est égal à 0, on peut prendre tout les sommets ayant le degré minimum. (Voir Rapport Partie V.5. Optimisation)
		if(d == 0){


      //On ajoute simplement chaque sommet de degré non-marqué minimum à la liste en modifiant les degrés.
			sgm->taille += lns->taille;

			while(lns->lx != NULL){
				sgm->lx = add_to_liste(sgm->lx,lns->lx->st);
				degre[lns->lx->st] = -1;
				for(int j=0;j<g->n;j++){
					if(g->a[lns->lx->st][j] == 1 && degre[j] > -1){
						degre[j] = -1;
						for(int z = 0; z < g->n; z++){
							if(g->a[j][z] == 1){
								(degre[z])--;
							}
						}
					}
				}
				lns->lx=lns->lx->suiv;

			}
			return calcul_maximum_exact_rec(g,degre,sgm);
		}

    //Si tout les sommets ont le même degré (càd le degré non-marqué minimum) à un moment donné, le choix du sommet à cet instant n'aura aucune influence sur la taille du sous-graphe final. On choisira donc le premier par défaut. (Voir Rapport Partie V.5. Optimisation)
		else if(d == -1){
			sgm->taille++;
			sgm->lx = add_to_liste(sgm->lx,lns->lx->st);

			/*Mettre à jour les degrés et l'adjacence*/
			degre[lns->lx->st] = -1;

			for(int j=0;j<g->n;j++){
				if(g->a[lns->lx->st][j] == 1 && degre[j] > -1){
					degre[j] = -1;

					for(int z = 0; z < g->n; z++){
						if(g->a[j][z] == 1){
							(degre[z])--;
						}
					}
				}
			}

			return calcul_maximum_exact_rec(g,degre,sgm);

		}

    //Sinon, cas général : on essaye toutes les possiblités, en duplicant nos structures afin de faire des calculs en parrallèle.
		else {

      //On crée ensuite un tampon et un max. On comparera par la suite la taille du tampon, contenant le sous-graphe maximum "potentiel" courant, et celle du maximum courant.
			sous_graphe_max *sgmtmp,*dmax;
			int max = sgm->taille;

			//On parcourt tout les sommets partageant le degré minimum.
			while(lns->lx != NULL){

				//Copie du tableau de degré dans un nouveau tableau : besoin d'une instance par possiblité pour la divergence (sinon structures partagées).
        int copie_degre[n_max];
        for(int i = 0; i < g->n;i++){
            copie_degre[i] = degre[i];

        }


				//Copie du sous-ensemble de sommets courant : ici aussi, on crée une instance par possibilité pour la divergence (sinon structures partagées).
				sous_graphe_max* tmp = malloc(sizeof(sous_graphe_max));
				liste ltmp = sgm->lx;
				while(ltmp != NULL){
					tmp->lx = add_to_liste(tmp->lx,ltmp->st);
					ltmp = ltmp -> suiv;
				}
				tmp->taille = sgm->taille;


        //On ajoute ensuite le sommet "potentiel" courant dans le sous-ensemble des sommets de degré non-marqué minimum, en effectuant les opérations habituelles.
				tmp->taille++;
				tmp->lx = add_to_liste(tmp->lx,lns->lx->st);
				copie_degre[lns->lx->st] = -1;

				for(int j=0;j<g->n;j++){
					if(g->a[tmp->lx->st][j] == 1 && copie_degre[j] > -1){
							copie_degre[j] = -1;
							for(int z = 0; z < g->n; z++){
								if(g->a[j][z] == 1){
									(copie_degre[z])--;
								}
							}
					}
				}

				//Choix ou omission du sous-graphe maximum "potentiel".
				sgmtmp = calcul_maximum_exact_rec(g,copie_degre,tmp);
				if (sgmtmp->taille > max){
          dmax = sgmtmp;
          max = dmax->taille;
        }
				lns->lx = lns->lx->suiv;

			}
			//Enfin, on retourne le plus grand sous-graphe maximum "potentiel" généré lors de la divergence.
			return dmax;
		}

	}
}


//Fonction principale.
sous_graphe_max* calcul_maximum_exact(Graph *g){


    int copie_degre[n_max];
    for(int i = 0; i < g->n;i++){
        copie_degre[i] = g->degre[i];
    }


  	sous_graphe_max *sgm = malloc(sizeof(sous_graphe_max));

  	return calcul_maximum_exact_rec(g,copie_degre,sgm);
}



int main(int argc, char *argv[]){

    if(argc != 2){
      printf("USAGE : ./calcul_maximum_exact fichier_contenant_le_graphe\n");
      exit(-1);
    }



    //Création du graphe.
    Graph *graph = malloc(sizeof(Graph));
    char* fgraph = argv[1];
    loadSource(fgraph, graph);



    //Résultat :
    liste X = calcul_maximum_exact(graph)->lx;
    printf("Le sous-graphe maximum exact généré est :\n");
    printf_liste(X);


    return 0;


}
