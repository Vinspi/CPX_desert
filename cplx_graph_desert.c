#include <stdio.h>
#include <stdlib.h>
#include <limits.h>


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

typedef struct {
	int taille;
	liste lx;
} sous_graphe_max;

liste sous_graphe_maximal(Graph_m *g, liste sous_graphe_desert);
liste sous_graphe_maximal_incomplete(Graph_m *g, liste sous_graphe_desert);
sous_graphe_max* sous_graphe_maximal_complet_rec(Graph_m *g, sous_graphe_max *sous_graphe_desert);
sous_graphe_max* sous_graphe_maximal_complet(Graph_m *g, sous_graphe_max *sous_graphe_desert);
sous_graphe_max* liste_sommet_degre_min(Graph_m *g, sous_graphe_max *xs);
Graph_m* clone_graph(Graph_m *src, Graph_m *dest);

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



        /*printf("( %d,", from);
        printf(" %d )\n", to);*/
    }

	fclose(source);
}

int main(void){

	Graph_m *graph = malloc(sizeof(Graph_m));

	liste sous_graphe_desert = NULL, sous_graphe_desert_maximal = NULL, sous_graphe_desert_maximal_maximal = NULL, tmp;

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

	loadSource("./graph/Benchs/anna", graph);

	//printGraph("./matri_graph", graph);



	sous_graphe_desert = sous_graphe_maximal(graph, sous_graphe_desert);
	
	printf_liste(sous_graphe_desert);
	printf("sous graphe desert ? %d\n", verification_graphe_desert(graph, sous_graphe_desert));

	printf("sous graphe maximal ? %d\n", verification_maximalite(graph, sous_graphe_desert));
	sous_graphe_desert_maximal = sous_graphe_maximal_incomplete(graph, sous_graphe_desert_maximal);

	printf_liste(sous_graphe_desert_maximal);

	printf("sous graphe desert ? %d\n", verification_graphe_desert(graph, sous_graphe_desert_maximal));

	printf("sous graphe maximal ? %d\n", verification_maximalite(graph, sous_graphe_desert_maximal));

	//sous_graphe_desert_maximal_maximal = sous_graphe_maximal_complet(graph, sous_graphe_desert_maximal_maximal);

	//printf_liste(sous_graphe_desert_maximal_maximal);

	//printf("sous graphe desert ? %d\n", verification_graphe_desert(graph, sous_graphe_desert_maximal_maximal));

	//printf("sous graphe maximal ? %d\n", verification_maximalite(graph, sous_graphe_desert_maximal_maximal));



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

/*
	cette fonction va renvoyer un sommet que l'on pourra ajouter à notre
	sous-graphe désert en conservant ses propriétés et -1 si x est maximal
*/

int sommet_a_ajouter(Graph_m *g, liste x){

	int tableau_adjacence[n_max];
	liste tmp_parcour = x;

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
		 un sommet au sous-graphe sans perdre sa propriété de sous-graphe desert.
		 on renvoie donc le sommet que l'on peut ajouter.
	*/

	for(int i=0;i<g->n;i++)
	  	if(tableau_adjacence[i])
				return i;

	return -1;
}

/*
	fonction permettant de trouver une liste de sommets formant
	un sous graphe désert maximal de g
*/

liste sous_graphe_maximal(Graph_m *g, liste sous_graphe_desert){

	liste tmp;
	int sommet_potentiel;

	/* initialisation de la liste de sommet avec le 1er sommet */
	sous_graphe_desert = malloc(sizeof(liste));
	sous_graphe_desert->st = 0;
	sous_graphe_desert->suiv = NULL;

	do{
		sommet_potentiel = sommet_a_ajouter(g,sous_graphe_desert);
		if(sommet_potentiel >= 0){
			tmp = malloc(sizeof(liste));
			tmp->st=sommet_potentiel;
			tmp->suiv=sous_graphe_desert;
			sous_graphe_desert=tmp;
		}
	}while(sommet_potentiel > 0);

	return sous_graphe_desert;
}

/* fonction utilitaire permettant d'afficher une liste */

void printf_liste(liste x){

    liste tmp = x;
		int acc = 0;
    printf("[ ");
    while(tmp != NULL){
        printf("%d ",tmp->st);
        tmp = tmp->suiv;
				acc++;
    }
    printf("]\n");
		printf("taille : %d\n",acc);
}
/*
	fonction utilitaire de copie de tableau
*/

int copie_tab(int source[], int dest[], int n){
	for(int i=0;i<n;i++){
		dest[i] = source[i];
	}
}

/*
	cette fonction choisit l'indice du sommet ayant un degres minimum
	en ignorant les valeurs négatives
*/

int sommet_deg_minimum(int degre[], int n){
	int min=INT_MAX, indice_min = 0;
	for(int i=0;i<n;i++)
		if(degre[i] > -1 && min > degre[i]){
			min = degre[i];
			indice_min = i;
		}
	//printf("min : %d\n", min);
	if(min == INT_MAX)
		return -1;
	return indice_min;
}

/*
	cette fonction va renvoyer un sommet que l'on pourra ajouter à notre
	sous-graphe désert en conservant ses propriétés, ce sommet sera de
	degrès minimum et -1 si x est maximal
*/

int sommet_a_ajouter_degres_minimum(Graph_m *g, int dernier_sommet_ajoute, int degres[]){

	int sommet_deg_min;



	/* on enleve tous ses sommets adjacents */
	for(int i=0;i<g->n;i++){
		if(g->a[dernier_sommet_ajoute][i]){
			degres[i] = -1;
			for(int j=0;j<g->n;j++)
				if(g->a[i][j] && degres[i] > -1){
					degres[j] = degres[j] - 1;
				}
		}
	}


	/* on recherche une valeur positive, si on en trouve une cela signifie que
		 le sous graphe n'est pas maximal puisque nous pouvons ajouter au moins
		 un sommet au sous-graphe sans perdre sa propriété de sous-graphe desert.
		 on renvoie donc le sommet que l'on peut ajouter.
	*/
	sommet_deg_min = sommet_deg_minimum(degres, g->n);


	return sommet_deg_min;

}

/*
	fonction trouvant une liste correspondant a un sous-graphe desert maximal
	de longeur approchant celle du sous-graphe maximum de g grâce à une heuristique
*/

liste sous_graphe_maximal_incomplete(Graph_m *g, liste sous_graphe_desert){

	liste tmp;
	int degres[g->n];
	/* on réalise une copie de degres contenu dns le graphe */
	copie_tab(g->degre, degres, g->n);


	/* initialisation de la liste de sommet avec le sommet de degres minimum*/
	int sommet_potentiel = sommet_deg_minimum(degres, g->n);

	degres[sommet_potentiel] = -1;

	sous_graphe_desert = malloc(sizeof(liste));
	sous_graphe_desert->st = sommet_potentiel;
	sous_graphe_desert->suiv = NULL;

	do{
		sommet_potentiel = sommet_a_ajouter_degres_minimum(g,sous_graphe_desert->st,degres);

		printf("on choisit %d (degres %d)\n", sommet_potentiel,degres[sommet_potentiel]);
		for(int i=0;i<g->n;i++)
			printf("%d ", degres[i]);
		printf("\n\n");

		if(sommet_potentiel >= 0){
			tmp = malloc(sizeof(liste));
			tmp->st=sommet_potentiel;
			tmp->suiv=sous_graphe_desert;
			sous_graphe_desert=tmp;
			degres[sommet_potentiel] = -1;
		}
	} while(sommet_potentiel >= 0);

	return sous_graphe_desert;
}

void retire_adjacent(Graph_m *g, int sommet){

	for(int i=0;i<g->n;i++){
		if(g->a[sommet][i])
			g->degre[i] = -1;
		for(int j=0;j<g->n;j++)
			if(g->a[i][j])
				g->degre[j] = g->degre[j]-1;
	}

}

/*
	fonction utilitaire permettant de cloner un objet sous_graphe_max
*/

sous_graphe_max* clone_sous_graphe_max(sous_graphe_max *src, sous_graphe_max *dest){

	dest = malloc(sizeof(sous_graphe_max));
	dest->taille = src->taille;



	liste tmp = src->lx, tmp2, lx=NULL;

	while (tmp != NULL) {

		tmp2 = malloc(sizeof(liste));

		tmp2->st = tmp->st;

		tmp2->suiv = lx;

		lx = tmp2;

		tmp = tmp -> suiv;

	}
	dest->lx = lx;

	return dest;

}

/*
	fonction utilitaire permettant de cloner un objet graphe
*/

Graph_m* clone_graph(Graph_m *src, Graph_m *dest){

	dest = malloc(sizeof(Graph_m));

	dest->n = src->n;

	for(int i=0;i<src->n;i++){
		dest->degre[i] = src->degre[i];
		for(int j=0;j<src->n;j++)
			dest->a[i][j] = src->a[i][j];
	}

	return dest;
}

sous_graphe_max* liste_sommet_degre_min(Graph_m *g, sous_graphe_max *xs){

	int taille = 0;
	liste x = NULL, tmp;
	xs = malloc(sizeof(sous_graphe_max));

	int min = n_max, imin;

	for(int i = 0; i < g->n; i++){
		if(g->degre[i] > -1 && g->degre[i] < min){
			imin = i;
			min = g->degre[i];
		}
	}

	for(int i = 0; i < g->n; i++){
		if(min == g->degre[i]){
			tmp = malloc(sizeof(liste));
			tmp->st = i;
			tmp->suiv = x;
			x = tmp;
			taille++;
		}
	}

	xs->lx = x;
	xs->taille = taille;

	return xs;
}

sous_graphe_max* sous_graphe_maximal_complet_rec(Graph_m *g, sous_graphe_max *sous_graphe_desert){

	sous_graphe_max *SGM, *resultat, *tmp, *clone_liste;
	Graph_m *copie_g;
	liste tmp2;



	resultat = malloc(sizeof(sous_graphe_max));
	resultat -> lx = NULL;
	resultat -> taille = 0;

	SGM = liste_sommet_degre_min(g,SGM);

	if(SGM->lx == NULL)
		return sous_graphe_desert;

	printf_liste(SGM->lx);

	liste l = SGM->lx;
	while (l != NULL) {
		copie_g = clone_graph(g,copie_g);
		copie_g->degre[l->st] = -1;
		clone_liste = clone_sous_graphe_max(sous_graphe_desert,clone_liste);

		tmp2 = malloc(sizeof(liste));
		tmp2->st = l->st;
		tmp2->suiv = clone_liste->lx;
		clone_liste->lx = tmp2;


		printf("on choisit : %d\n", l->st);

		retire_adjacent(copie_g, l->st);
		for(int i=0;i<g->n;i++)
			printf("%d \n", g->degre[i]);



		printf("recursion ! \n");
		tmp = sous_graphe_maximal_complet_rec(copie_g, clone_liste);

		if(tmp->taille > resultat -> taille)
			resultat = tmp;
		l = l->suiv;
	}

	return resultat;

}

sous_graphe_max* sous_graphe_maximal_complet(Graph_m *g, sous_graphe_max *sous_graphe_desert){

	/* init de la fonction */
	int max = 0;
	liste l;
	sous_graphe_max *SGM, *tmp, *resultat;

	for(int i=0;i<g->n;i++)
		printf("%d \n", g->degre[i]);
	printf("recursion ! \n");

	resultat = malloc(sizeof(sous_graphe_max));
	resultat -> lx = NULL;
	resultat -> taille = 0;
	Graph_m *copie_g;
	SGM = liste_sommet_degre_min(g,SGM);

	printf_liste(SGM->lx);

	l = SGM->lx;



	while (l != NULL) {
		copie_g = clone_graph(g,copie_g);

		printf("on choisit : %d\n", l->st);

		copie_g->degre[l->st] = -1;

		sous_graphe_desert = malloc(sizeof(sous_graphe_max));
		sous_graphe_desert->taille = 1;
		sous_graphe_desert->lx = malloc(sizeof(liste));
		sous_graphe_desert->lx->st = l->st;
		sous_graphe_desert->lx->suiv = NULL;

		retire_adjacent(copie_g, l->st);

		printf("recursion ! \n");
		tmp = sous_graphe_maximal_complet_rec(copie_g,sous_graphe_desert);
		if(tmp->taille > resultat -> taille)
			resultat = tmp;
		l=l->suiv;
	}

	return resultat;

}
