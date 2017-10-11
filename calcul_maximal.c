//#include "util.h"

//Trucs à enlever quand quelqu'un fera un makefile. Qui marche.
//___________________________________________________________________________


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









//___________________________________________________________________________


//Fonction vérifiant si l'on a construit un maximal en observant la table d'adjacence : si tout les sommets sont marqués, on a créé un mmaximal.
//Renvoie 1 dans ce cas, 0 sinon.
int verif_maximal(Graph *g, int* tab){

	for(int i = 0; i < g->n; i++){
		if(tab[i] == 0) return 0;
	}
	return 1;

}


//Fonction construisant et renvoyant un sous-ensemble X tel que X est un sous-graphe désert maximal de G.
liste calcul_maximal(Graph *g){

    //Création de la table d'adjacence et d'un tampon.
    int tableau_adjacence[g->n], i = 0;
    for(int i = 0; i < g->n; i++){
      tableau_adjacence[i] = 0;
		}

	  liste X = NULL;

    //Tant que l'on a pas créé de maximal, on continue :
	  while(verif_maximal(g, tableau_adjacence) != 1){
		  i = 0;

		  //On choisit le nouveau sommet : on choisira le premier sommet non-marqué que l'on trouve en parcourant la table d'adjacence.
		  while(i < g->n-1 && tableau_adjacence[i] == 1){
			     i++;
		  }


      //On l'ajoute ensuite à la liste.
		  X = add_to_liste(X, i);

      //Et on marque son adjacence.
      tableau_adjacence[i] = 1;
      for(int j=0;j<g->n;j++){
				    if(g->a[i][j]){
            	tableau_adjacence[j] = 1;

						}
		  }


	   }


	   return X;
}





int main(int argc, char *argv[]){

    if(argc != 2){
      printf("USAGE : ./verif_desert fichier_contenant_le_graphe\n");
      exit(-1);
    }
    
    //Création du graphe.
    Graph *graph = malloc(sizeof(Graph));
    char* fgraph = argv[1];
    loadSource(fgraph, graph);


    //Résultat :
    liste X = calcul_maximal(graph);
    printf("Le sous-graphe maximal généré est :\n");
    printf_liste(X);


    return 0;


}
