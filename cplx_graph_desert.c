#include <stdio.h>
#include <stdlib.h>


/*	Liste des codes d'erreur

	10 : L'ouverture du fichier source a échoué
	15 : L'ouverture du fichier de sortie à échoué

*/

#define n_max 1000

typedef int sommet;

typedef struct {
	int a[n_max][n_max];
	int n;
	int degre[n_max];
} Graph_m;

typedef struct maillon{
	sommet st;
	struct maillon *suiv;
} maillon;

typedef maillon *liste;



/*	Fontion initGraphe : initialise les champs de la structure representant le graphe en mémoire
	[Param IN]	Graph_m *g : pointeur vers la structure reprensentant le graphe en mémoire */

void initGraph(Graph_m *g, int n){
	for(int i=0; i<n_max; i++){
		for(int j=0; j<n_max; j++){
			g->a[i][j] = 0;
		}
		g->degre[i] = 0;
	}
	g->n = n;
}


/* 	Fonction printGraph : écrit la représentation matricielle du graphe dans un fichier
	[Param IN]	char *filename : le nom du fichier dans lequel écrire
	[Param IN]	Graph_m *g : pointeur vers la structure reprensentant le graphe en mémoire */

void printGraph(char* filename, Graph_m *g){
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


/*	Fontion loadSource : construit le graphe (representation matricielle) à partir du fichier
	[Param IN]	char *filename : le nom du fichier contenant la liste des aretes
	[Param IN]	Graph_m *g : pointeur vers la structure reprensentant le graphe en mémoire */

void loadSource(char* filename, Graph_m *g){

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

		g->a[from][to] = 1;
		g->a[to][from] = 1;
		g->degre[from] = g->degre[from] + 1;

		printf("( %d,", from);
		printf(" %d )\n", to);
	}

	fclose(source);
}

//Affichage liste x.
void printf_liste(liste x){

	liste tmp = x;
	printf("[ ");
	while(tmp != NULL){
		printf("%d ",tmp->st);
		tmp = tmp->suiv;
	}
	printf("]\n");
}

//Ajoute un sommet a à la liste x.
liste add_to_liste(liste x, sommet a){
	liste tmp = malloc(sizeof(liste));
	tmp->st = a;
	tmp->suiv = x;
	return tmp;
}







int verification_graphe_desert(Graph_m *g, liste x){


	liste tmp_parcour = x, tmp_parcour2;
	int resultat = 1;

	while(tmp_parcour != NULL){
		tmp_parcour2 = tmp_parcour -> suiv;
		while (tmp_parcour2 != NULL) {
			if(g->a[tmp_parcour->st][tmp_parcour2->st])
				resultat = 0;
			tmp_parcour2 = tmp_parcour2 -> suiv;
		}
		tmp_parcour = tmp_parcour -> suiv;
	}

	return resultat;
}



//Approche par tableau.
void generer_tableau_all(Graph_m *g, int* tab){
	for(int i = 0; i < g->n; i++){
		tab[i] = 1;
	}

}


void retirer_adjacence(Graph_m *g, liste x, int* tab){
	liste tmp = x;
	while(tmp != NULL){
		tab[tmp->st] = 0;					//Retrait des sommets de la liste.
		for(int i = 0; i < g->n; i++){
			if(g->a[tmp->st][i] == 1) tab[i] = 0;		//Retrait des sommets adjacents aux sommets de la liste
		}
		tmp = tmp->suiv;
	}
}

//Retourne 0 si la l'adjancence est nulle, 1 sinon.
int verif_all(Graph_m *g, int* tab){

	for(int i = 0; i < g->n; i++){
		if(tab[i] == 1) return 1;
	}
	return 0;

}

//Retourne 0 si maximal, 1 sinon.
int verification_maximalite(Graph_m *g, liste x){
	int all[n_max]; 
	generer_tableau_all(g, all);
	
	retirer_adjacence(g, x, all);
	return verif_all(g, all);

}




int main(void){

	Graph_m *graph = malloc(sizeof(Graph_m));
 
	loadSource("./source_graph", graph);
	
	//printGraph("./matri_graph", graph);
	
	liste x;
	x = add_to_liste(x, 0);
	x = add_to_liste(x, 2);
	x = add_to_liste(x, 4);	
	//x = add_to_liste(x, 3);
	printf_liste(x);

	if(verification_graphe_desert(graph, x) == 1) printf("x est un sous-graphe desert de g\n");
	else printf("x n'est pas un sous-graphe desert de g\n");

	printf("%d\n",verification_maximalite(graph, x));

	return 0;
}

