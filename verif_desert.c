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


//Fonction vérifiant si la liste x, qui implémente un sous-ensemble de sommets, est un sous-graphe désert dans le graphe G.
//Retourne 1 si x est un sous-graphe désert, 0 sinon.
int verification_graphe_desert(Graph *g, liste x){

	liste tmp_parcour = x, tmp_parcour2;
	int resultat = 1;

	while(tmp_parcour != NULL && resultat == 1){
		tmp_parcour2 = tmp_parcour -> suiv;
		while (tmp_parcour2 != NULL && resultat == 1) {
			if(g->a[tmp_parcour->st][tmp_parcour2->st])
				resultat = 0;
			tmp_parcour2 = tmp_parcour2 -> suiv;
		}
		tmp_parcour = tmp_parcour -> suiv;
	}

	return resultat;
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


    //Exemple de création d'un sous-ensemble de sommets à tester :
    liste sous_graphe_desert = NULL;
    sous_graphe_desert = add_to_liste(sous_graphe_desert,0);
    sous_graphe_desert = add_to_liste(sous_graphe_desert,2);
    sous_graphe_desert = add_to_liste(sous_graphe_desert,5);


    //Résultat
    if(verification_graphe_desert(graph,sous_graphe_desert)){
      printf("Le sous-graphe X est désert !\n");
    }
    else {
      printf("Le sous-graphe X n'est pas désert.\n");
    }

    return 0;


}
