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

typedef struct {
	int taille;
	liste lx;
} sous_graphe_max;

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
		g->degre[to] = g->degre[to] +1;

		/*printf("( %d,", from);
		printf(" %d )\n", to);*/
	}

	fclose(source);
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

	int tableau_adjacence[n_max], max = 1;
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
	  	if(tableau_adjacence[i]){
			printf("tab[%d] = %d\n",i, tableau_adjacence[i]);
			max = 0;
		}
	return max;
}


//Affichage liste x.
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

int verif_all(Graph_m *g, int* tab){

	for(int i = 0; i < g->n; i++){
		if(tab[i] == 1) return 1;
	}
	return 0;

}

void generer_tableau_all(Graph_m *g, int* tab){
	for(int i = 0; i < g->n; i++){
		tab[i] = 1;
	}

}



liste calcul_maximal(Graph_m *g){


	int all[n_max], i = 0;
	generer_tableau_all(g, all);	
	

	liste x = NULL;
	/*On prend 0 par défaut.
	x = add_to_liste(x, 0);*/
	liste tmp_parcour = x;
	while(verif_all(g, all) != 0){	
		i = 0;
		
		/*Choisir le nouveau sommet i*/
		while(i < g->n && all[i] == 0){
			i++;
		}
		
		x = add_to_liste(x, i);
		
		
		
		tmp_parcour = x;
		/*Retirer l'adjacence*/
		while (tmp_parcour != NULL) {
		/* on retire le sommet courant des adjacents */
			all[tmp_parcour->st] = 0;
		/* on retire ensuite tous ses sommets adjacents */
			for(int j=0;j<g->n;j++){
				if(g->a[tmp_parcour->st][j])
					all[j] = 0;
			}
			tmp_parcour = tmp_parcour -> suiv;
		}
		


	}
	return x;
}





//__________________________________________________________________
//__________________________________________________________________
//__________________________________________________________________


void free_liste(liste x){

	while(x != NULL){
		liste tmp = x;
		x = x -> suiv;
		free(tmp);
	}

}

void sommet_degre_min(Graph_m *g, sous_graphe_max *xs){

	
	liste x = NULL;
	int min = n_max, imin = -1;
	for(int i = 0; i < g->n; i++){
		
		if(g->degre[i] > -1 && g->degre[i] < min){
			imin = i;
			min = g->degre[i];
			free_liste(x);
			//if(x != NULL) free(x);
			x = add_to_liste(x, imin);		
			xs->taille = 1;
		}
		/*if(g->degre[i] == min){
			x = add_to_liste(x, imin);		
			xs->taille = (xs->taille) + 1;
		}*/
	}
	
	xs->lx = x;
}

void sommet_degre_min_safe(Graph_m *g, sous_graphe_max *xs){

	liste x = NULL;
	int min = n_max, imin = -1;
	for(int i = 0; i < g->n; i++){
		if(g->degre[i] > -1 && g->degre[i] < min){
			imin = i;
			min = g->degre[i];
		}
	}
	
	printf("min = %d, de degre %d\n",imin,min);
	for(int i = 0; i < g->n; i++){
		if(g->degre[i] == min){
			x = add_to_liste(x, i);		
			xs->taille = (xs->taille) + 1;
		}

	}
	
	xs->lx = x;

}

//Retourne 1 si maximal, 0 sinon.
int verif_maximal(Graph_m *g){

	
	for(int i = 0; i < g->n; i++){
		//if(tab[i] == 1) return 1;		
		if(g->degre[i] > -1) return 0;
	}
	return 1;

}
sous_graphe_max* calcul_maximum_complet_rec(Graph_m *g, sous_graphe_max *sgm){
	


	/*if(verif_maximal(g) == 1){
		return sgm;
	}/*	
	

	/*Choisir le nouveau sommet i*/
	sous_graphe_max *lns = malloc(sizeof(sous_graphe_max));
	sommet_degre_min_safe(g,lns);

	if(lns->lx == NULL){
		printf("degres :\n");
		for(int i = 0; i < g->n; i++){
			printf("%d : %d\n",i,g->degre[i]);
		}
		return sgm;
	}
	
	if (lns->taille == 1){
		printf("min = %d\n",lns->lx->st);
		sgm->taille++;
		sgm->lx = add_to_liste(sgm->lx,lns->lx->st);

		/*Mettre à jour les degrés et l'adjacence*/
		
		
		g->degre[lns->lx->st] = -1;
			
		for(int j=0;j<g->n;j++){
			if(g->a[sgm->lx->st][j] == 1){
				g->degre[j] = -1;
				for(int z = 0; z < g->n; z++){
					if(g->a[j][z] == 1){
						(g->degre[z])--;
					}
				}
			}
		}
		

		/*Appel récursif*/
		return calcul_maximum_complet_rec(g,sgm);
		
	} 
	else {		//DIVERGENCE

		//Test avec heuristique du premier arrivé.
		sgm->taille++;
		sgm->lx = add_to_liste(sgm->lx,lns->lx->st);

		/*Mettre à jour les degrés et l'adjacence*/
		
		
		
			
		g->degre[lns->lx->st] = -1;
			
		for(int j=0;j<g->n;j++){
			if(g->a[sgm->lx->st][j] == 1){
				g->degre[j] = -1;
				for(int z = 0; z < g->n; z++){
					if(g->a[j][z] == 1){
						(g->degre[z])--;
					}
				}
			}
		}	
		


		/*Appel récursif*/
		return calcul_maximum_complet_rec(g,sgm);

	}
}
sous_graphe_max* calcul_maximum_complet(Graph_m *g){
	
	Graph_m *g2 = malloc(sizeof(Graph_m));
	g2->n = g->n;
	for(int i = 0; i < g->n; i++){
		for(int j = 0; j < g->n; j++){		
			g2->a[i][j] = g->a[i][j];
		}
		g2->degre[i] = g->degre[i];
	}
	
	sous_graphe_max *sgm = malloc(sizeof(sous_graphe_max));
	return calcul_maximum_complet_rec(g2,sgm);
}



//___________________________________________________________________________________________
//__________________________________________________________________
//__________________________________________________________________
//__________________________________________________________________
//__________________________________________________________________

int main(void){

	Graph_m *graph = malloc(sizeof(Graph_m));

	liste sous_graphe_desert = NULL, tmp;
	
	/*tmp = malloc(sizeof(liste));
	tmp->st = 0;
	tmp->suiv = sous_graphe_desert;
	sous_graphe_desert = tmp;*/

	/*sous_graphe_desert = add_to_liste(sous_graphe_desert, 0);
	sous_graphe_desert = add_to_liste(sous_graphe_desert, 4);
	sous_graphe_desert = add_to_liste(sous_graphe_desert, 2);

	/*tmp = malloc(sizeof(liste));
	tmp->st = 4;
	tmp->suiv = sous_graphe_desert;
	sous_graphe_desert = tmp;

	tmp = malloc(sizeof(liste));
	tmp->st = 2;
	tmp->suiv = sous_graphe_desert;
	sous_graphe_desert = tmp;*/

	loadSource("./anna", graph);

	//printGraph("./matri_graph", graph);

	/*printf("sous graphe desert ? %d\n", verification_graphe_desert(graph, sous_graphe_desert));

	printf("sous graphe maximal ? %d\n", verification_maximalite(graph, sous_graphe_desert));
	printf_liste(sous_graphe_desert);*/
	
	sous_graphe_max *sgm = calcul_maximum_complet(graph);
	printf_liste(sgm->lx);


	printf("sous graphe maximal ? %d\n", verification_maximalite(graph, sgm->lx));
	
	

	return 0;
}

