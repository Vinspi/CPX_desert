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

int main(void){

	Graph_m *graph = malloc(sizeof(Graph_m));

	liste sous_graphe_desert = NULL, tmp;

	tmp = malloc(sizeof(liste));
	tmp->st = 0;
	tmp->suiv = sous_graphe_desert;
	sous_graphe_desert = tmp;

	tmp = malloc(sizeof(liste));
	tmp->st = 4;
	tmp->suiv = sous_graphe_desert;
	sous_graphe_desert = tmp;

	tmp = malloc(sizeof(liste));
	tmp->st = 2;
	tmp->suiv = sous_graphe_desert;
	sous_graphe_desert = tmp;

	loadSource("./source_graph", graph);

	//printGraph("./matri_graph", graph);

	printf("sous graphe desert ? %d\n", verification_graphe_desert(graph, sous_graphe_desert));

	printf("sous graphe maximal ? %d\n", verification_maximalite(graph, sous_graphe_desert));
	return 0;
}

/*
	 permet de verifier si la liste x est un sous graphe désert de g
	 la fonction renvoie 1 si c'est le cas  sinon.
*/

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

/*
	permet de verifier la maximalité d'un sous graphe désert de g
	la fonction renvoie 1 si le ous graphe passé en parametre est maximal et 0 sinon.
*/

int verification_maximalite(Graph_m *g, liste x){

	int tableau_adjacence[n_max];
	liste tmp_parcour = x;

	/* si ce n'est pas un sous graphe desert de g il ne peut etre maximal */
	if(!verification_graphe_desert(g,x))
		return 0;

		for(int i=0;i<g->n;i++)
			tableau_adjacence[i] = 1;

	while (tmp_parcour != NULL) {
		/* on retire le sommet courant des adjacents */
		tableau_adjacence[tmp_parcour->st] = 0;
		/* on retire ensuite tous ses sommets adjacents */
		for(int i=0;i<g->n;i++){
			if(g->a[tmp_parcour->st][i])
				tableau_adjacence[i] = 0;
		}
		tmp_parcour = tmp_parcour -> suiv;
	}

	/* on recherche une valeur positive, si on en trouve une cela signifie que
		 le sous graphe n'est pas maximal puisque nous pouvons ajouter au moins
		 un sommet au sous-graphe sans perdre sa propriété de sous-graphe desert
	*/

	for(int i=0;i<g->n;i++)
	  	if(tableau_adjacence[i])
				return 0;

	return 1;
}
