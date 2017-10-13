#include "util.h"




//	Fonction vérifiant si la liste X, qui implémente un sous-ensemble de sommets, est un sous-graphe désert maximal dans le graphe G.
//	Retourne 1 si X est un sous-graphe désert maximal, 0 sinon.
int verification_maximalite(Graph *g, liste X){

	int tableau_adjacence[n_max], max = 1;
	liste tmp_parcour = X;

	//Si X n'est pas un sous graphe desert de G, il ne peut être maximal.
	/*if(!verification_graphe_desert(g,X))
		return 0;
*/
	for(int i=0;i<g->n;i++)
			tableau_adjacence[i] = 1;

  //On parcourt X.
	while (tmp_parcour != NULL) {

  	//On marque les sommets choisis avec un entier négatif.
		tableau_adjacence[tmp_parcour->st] = -1;

  	//On marque ensuite tout ses adjacents avec 0.
		for(int i=0;i<g->n;i++){
			if(g->a[tmp_parcour->st][i]){

					//Sauf si l'un des adjacents est déjà dans l'ensemble de sommets, auquel cas X n'est pas désert.
					if(tableau_adjacence[i] == -1){
						printf("X non désert, %d et %d adjacents et tous deux dans X.\n",tmp_parcour->st,i);
						return 0;
					}
					else tableau_adjacence[i] = 0;
			}
		}
		tmp_parcour = tmp_parcour -> suiv;
	}

	//  On parcourt notre tableau d'adjacence.
  //  Si l'on trouve un sommet non-marqué, càd pour lequel la valeur dans le tableau d'adjacence vaut 1, alors il existe un sommet que l'on peut rajouter à X : X n'est pas maximal
	for(int i=0;i<g->n;i++)
	  	if(tableau_adjacence[i] > 0){
			  	printf("tab[%d] = %d\n",i, tableau_adjacence[i]);
			   	max = 0;
		  }
	return max;
}





int main(int argc, char *argv[]){

    if(argc != 2){
      printf("USAGE : ./verif_maximal fichier_contenant_le_graphe\n");
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
		sous_graphe_desert = add_to_liste(sous_graphe_desert,7);

    //Résultat
    if(verification_maximalite(graph,sous_graphe_desert)){
      printf("Le sous-graphe X est maximal !\n");
    }
    else {
      printf("Le sous-graphe X n'est pas maximal.\n");
    }

    return 0;


}
