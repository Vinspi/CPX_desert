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
<<<<<<< HEAD
			free_liste(x);
			//if(x != NULL) free(x);
			x = add_to_liste(x, imin);		
			xs->taille = 1;
		}
		/*if(g->degre[i] == min){
			x = add_to_liste(x, imin);		
=======
			//free_liste(x);
			free(x);
			x = add_to_liste(x, imin);
			xs->taille = 1;
		}
		if(g->degre[i] == min){
			x = add_to_liste(x, imin);
>>>>>>> 9f63778cb00fb1e6cba921210505281efcef92cc
			xs->taille = (xs->taille) + 1;
		}*/
	}
	
	xs->lx = x;
}

//Remplie la liste xs avec l'ensemble des sommets de degré minimum, et retourne le degré minimum, sauf si tout les sommets restants ont le même degré.
int sommet_degre_min_safe(Graph_m *g, sous_graphe_max *xs){

	liste x = NULL;
	int min = n_max, imin = -1, allsame = 1;
	for(int i = 0; i < g->n; i++){
		if(g->degre[i] > -1 && g->degre[i] <= min){
			imin = i;
			min = g->degre[i];
		}
	}
	
	
	for(int i = 0; i < g->n; i++){
		if(g->degre[i] == min){
			//printf("Ajout de %d\n",i);
			x = add_to_liste(x, i);		
			xs->taille = (xs->taille) + 1;
		}
		else if(g->degre[i] > -1 && g->degre[i] != min){
			allsame = 0;
		} 

	}

	xs->lx = x;
	if (allsame == 1) return -1;
	return min;

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


	/*Copie du graph g
	Graph_m *g2 = malloc(sizeof(Graph_m));
	g2->n = g->n;
	for(int i = 0; i < g->n; i++){
		for(int j = 0; j < g->n; j++){		
			g2->a[i][j] = g->a[i][j];
		}
		g2->degre[i] = g->degre[i];
		//if(sgm->taille < 2) printf("g->degre[%d] = %d\n",i,g->degre[i]);
	}

	/*if(verif_maximal(g) == 1){
		return sgm;
<<<<<<< HEAD
	}/*	
	
=======
	}

>>>>>>> 9f63778cb00fb1e6cba921210505281efcef92cc

	/*Choisir le nouveau sommet i*/
	sous_graphe_max *lns = malloc(sizeof(sous_graphe_max));
	sommet_degre_min_safe(g,lns);

	if(lns->lx == NULL){
		/*printf("degres :\n");
		for(int i = 0; i < g->n; i++){
			printf("%d : %d\n",i,g->degre[i]);
		}*/
		return sgm;
	}
	
	if (lns->taille == 1){
		sgm->taille++;
		sgm->lx = add_to_liste(sgm->lx,lns->lx->st);

		/*Mettre à jour les degrés et l'adjacence*/


		g->degre[lns->lx->st] = -1;

		for(int j=0;j<g->n;j++){
			if( g->a[sgm->lx->st][j] == 1 && g->degre[j] > -1){
				g->degre[j] = -1;
				
				for(int z = 0; z < g->n; z++){
					if(g->a[j][z] == 1){
						(g->degre[z])--;
					}
				}
			}
		}
<<<<<<< HEAD
		
=======



>>>>>>> 9f63778cb00fb1e6cba921210505281efcef92cc

		/*Appel récursif*/
		return calcul_maximum_complet_rec(g,sgm);

	}
	else {		//DIVERGENCE

		//Test avec heuristique du premier arrivé.
		sgm->taille++;
		sgm->lx = add_to_liste(sgm->lx,lns->lx->st);

		/*Mettre à jour les degrés et l'adjacence*/




		g->degre[lns->lx->st] = -1;
<<<<<<< HEAD
=======

>>>>>>> 9f63778cb00fb1e6cba921210505281efcef92cc
		for(int j=0;j<g->n;j++){
			if(g->a[sgm->lx->st][j] == 1 && g->degre[j] > -1){
				g->degre[j] = -1;
				for(int z = 0; z < g->n; z++){
					if(g->a[j][z] == 1){
						(g->degre[z])--;
						
					}
				}
			}
<<<<<<< HEAD
		}	
		
=======
		}


>>>>>>> 9f63778cb00fb1e6cba921210505281efcef92cc


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
		//printf("g->degre[%d] = %d\n",i,g->degre[i]);
	}

	sous_graphe_max *sgm = malloc(sizeof(sous_graphe_max));
	return calcul_maximum_complet_rec(g2,sgm);
}



//___________________________________________________________________________________________



sous_graphe_max* vcalcul_maximum_complet_rec(Graph_m *g, sous_graphe_max *sgm){
	

	/*Copie du graph g
	Graph_m *g2 = malloc(sizeof(Graph_m));
	g2->n = g->n;
	for(int i = 0; i < g->n; i++){
		for(int j = 0; j < g->n; j++){		
			g2->a[i][j] = g->a[i][j];
		}
		g2->degre[i] = g->degre[i];
		//if(sgm->taille < 2) printf("g->degre[%d] = %d\n",i,g->degre[i]);
	}

	/*if(verif_maximal(g) == 1){
		return sgm;
	}/*	
	

	/*Choisir le nouveau sommet i*/
	sous_graphe_max *lns = malloc(sizeof(sous_graphe_max));
	int d = sommet_degre_min_safe(g,lns);

	if(lns->lx == NULL){
		/*printf("degres :\n");
		for(int i = 0; i < g->n; i++){
			printf("%d : %d\n",i,g->degre[i]);
		}*/
		return sgm;
	}
	
	//Lorsqu'il n'y a pas de divergence.
	if (lns->taille == 1){
		
		sgm->taille++;
		sgm->lx = add_to_liste(sgm->lx,lns->lx->st);

		/*Mettre à jour les degrés et l'adjacence*/
		
		
		g->degre[lns->lx->st] = -1;
			
		for(int j=0;j<g->n;j++){
			if( g->a[sgm->lx->st][j] == 1 && g->degre[j] > -1){
				g->degre[j] = -1;
				
				for(int z = 0; z < g->n; z++){
					if(g->a[j][z] == 1){
						(g->degre[z])--;
					}
				}
			}
		}
		

		/*Appel récursif*/
		return vcalcul_maximum_complet_rec(g,sgm);
		
	} 
	else {		//DIVERGENCE

		printf("DIVERGENCE\n");
		if(d == 0){	//Si le degré minimum est égal à 0, on peut prendre tout les sommets ayant le degré minimum.
			printf("Divergence EVITEYYYYY CAR 0\n");
			sgm->taille += lns->taille;
			liste fri;
			//On ajoute chaque sommet de degré minimum à la liste et on modifie les degrés.
			while(lns->lx != NULL){
				sgm->lx = add_to_liste(sgm->lx,lns->lx->st);
				g->degre[lns->lx->st] = -1;
				for(int j=0;j<g->n;j++){
					if( g->a[lns->lx->st][j] == 1 && g->degre[j] > -1){
						g->degre[j] = -1;
						for(int z = 0; z < g->n; z++){
							if(g->a[j][z] == 1){
								(g->degre[z])--;
							}
						}
					}
				}

				fri = lns->lx;
				lns->lx=lns->lx->suiv;
				free(fri);
			}
			
			return vcalcul_maximum_complet_rec(g,sgm);
		}
		

		else if(d == -1){	//Si tout les sommets ont le même degré (càd le degré minimum) à un moment donnée, le choix du sommet à cet instant n'aura aucune influence sur la taille du sous-graphe final. On choisira donc le premier par défaut.
			printf("Divergence EVITEYYYYY CAR ALLSAME\n");
			sgm->taille++;
			sgm->lx = add_to_liste(sgm->lx,lns->lx->st);

			/*Mettre à jour les degrés et l'adjacence*/
		
		
			g->degre[lns->lx->st] = -1;
			
			for(int j=0;j<g->n;j++){
				if( g->a[lns->lx->st][j] == 1 && g->degre[j] > -1){
					g->degre[j] = -1;
					
					for(int z = 0; z < g->n; z++){
						if(g->a[j][z] == 1){
							(g->degre[z])--;
						}
					}
				}
			}
			//free_liste(lns);

			/*Appel récursif*/
			return vcalcul_maximum_complet_rec(g,sgm);

		}
		else {
			sous_graphe_max *div,*dmax;
			int max = sgm->taille;
			//On parcourt tout les sommets partageant le degré minimum.
			while(lns->lx != NULL){
				//printf("ESSAI\n");
				//Copie du graphe G dans un nouveau graphe : besoin d'une instance par divergence (sinon structure partagée).
				Graph_m *g2 = malloc(sizeof(Graph_m));
				g2->n = g->n;
				for(int i = 0; i < g->n; i++){
					for(int j = 0; j < g->n; j++){		
						g2->a[i][j] = g->a[i][j];
					}
					g2->degre[i] = g->degre[i];
				}
				//printf("Apres copie du graphe ?\n");

				//Copie de sgm.
				sous_graphe_max* tmp = malloc(sizeof(Graph_m));
				liste ltmp = sgm->lx;
				//printf_liste(sgm->lx);
				while(ltmp != NULL){
				
					tmp->lx = add_to_liste(tmp->lx,ltmp->st);
					//printf("Sommet %d ajouté\n",ltmp->st);
					ltmp = ltmp -> suiv;
				
				}
				tmp->taille = sgm->taille;
				//printf("Apres copie du sgm ?\n");

				tmp->taille++;
				tmp->lx = add_to_liste(tmp->lx,lns->lx->st);
				//Modification de G en utilisant le sommet "potentiel" courant.
				g2->degre[tmp->lx->st] = -1;
				//printf("Ici ?\n");
				for(int j=0;j<g2->n;j++){
					//printf("ESSAI FOR ? %d\n",tmp->taille);
					if(g2->a[tmp->lx->st][j] == 1){
						//printf("BLBLBL\n");
						if(g2->degre[j] > -1){
							//printf("La ?\n");					
							g2->degre[j] = -1;
							for(int z = 0; z < g2->n; z++){
								if(g2->a[j][z] == 1){
									(g2->degre[z])--;
								}
							}
						}
					}
				}		
				//printf("Avant la rec ?\n");
				//Choix ou omission du sommet.		
				div = vcalcul_maximum_complet_rec(g2,tmp);
				if (div->taille > max) dmax = div;
				//else free(tmp);
				lns->lx = lns->lx->suiv;

			}
			return dmax;
		}
		
	}
}





//Fonction principale.
sous_graphe_max* vcalcul_maximum_complet(Graph_m *g){
	
	Graph_m *g2 = malloc(sizeof(Graph_m));
	g2->n = g->n;
	for(int i = 0; i < g->n; i++){
		for(int j = 0; j < g->n; j++){		
			g2->a[i][j] = g->a[i][j];
		}
		g2->degre[i] = g->degre[i];
		//printf("g->degre[%d] = %d\n",i,g->degre[i]);
	}
	
	sous_graphe_max *sgm = malloc(sizeof(sous_graphe_max));
	return vcalcul_maximum_complet_rec(g2,sgm);
}









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

	loadSource("./Benchs/anna", graph);

	//printGraph("./matri_graph", graph);

	/*printf("sous graphe desert ? %d\n", verification_graphe_desert(graph, sous_graphe_desert));

	printf("sous graphe maximal ? %d\n", verification_maximalite(graph, sous_graphe_desert));
	printf_liste(sous_graphe_desert);*/
<<<<<<< HEAD
	
	/*sous_graphe_max *sgm = calcul_maximum_complet(graph);
=======

	sous_graphe_max *sgm = calcul_maximum_complet(graph);
>>>>>>> 9f63778cb00fb1e6cba921210505281efcef92cc
	printf_liste(sgm->lx);

	printf("sous graphe desert ? %d\n", verification_graphe_desert(graph, sgm->lx));
	printf("sous graphe maximal ? %d\n", verification_maximalite(graph, sgm->lx));*/
	
	sous_graphe_max *vsgm = vcalcul_maximum_complet(graph);
	printf_liste(vsgm->lx);

<<<<<<< HEAD
	printf("sous graphe desert ? %d\n", verification_graphe_desert(graph, vsgm->lx));
	printf("sous graphe maximal ? %d\n", verification_maximalite(graph, vsgm->lx));
	
	
=======
	printf("sous graphe maximal ? %d\n", verification_maximalite(graph, sgm->lx));


>>>>>>> 9f63778cb00fb1e6cba921210505281efcef92cc

	return 0;
}
