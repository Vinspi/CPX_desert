#include "util.h"




//	Fonction vérifiant si la liste X, qui implémente un sous-ensemble de sommets, est un sous-graphe désert maximal dans le graphe G.
//	Retourne 1 si X est un sous-graphe désert maximal, 0 sinon.
int verification_maximalite(Graph *g, liste X){

	int tableau_adjacence[n_max], max = 1;
	liste tmp_parcour = X;

	//Si X n'est pas un sous graphe desert de G, il ne peut être maximal.
	if(!verification_graphe_desert(g,X))
		return 0;

	for(int i=0;i<g->n;i++)
			tableau_adjacence[i] = 1;

  //On parcourt X.
	while (tmp_parcour != NULL) {

  	//On marque le sommet choisi.
		tableau_adjacence[tmp_parcour->st] = 0;

  	//On marque ensuite tout ses adjacents.
		for(int i=0;i<g->n;i++){
			if(g->a[tmp_parcour->st][i])
				tableau_adjacence[i] = 0;
		}
		tmp_parcour = tmp_parcour -> suiv;
	}

	//  On parcourt notre tableau d'adjacence.
  //  Si l'on trouve un sommet non-marqué, càd pour lequel la valeur dans le tableau d'adjacence vaut 1, alors il existe un sommet que l'on peut rajouter à X : X n'est pas maximal
	for(int i=0;i<g->n;i++)
	  	if(tableau_adjacence[i]){
			     printf("tab[%d] = %d\n",i, tableau_adjacence[i]);
			   max = 0;
		  }
	return max;
}





int main(int argc, char *argv[]){

    if(argc != 1){
      printf("USAGE : ./verif_desert fichier_contenant_le_graphe\n");
      exit(-1);
    }
    printf("argv[1] = %s\n",argv[1]);
    //Création du graphe.
    Graph *graph = malloc(sizeof(Graph));
    char* fgraph = argv[1];
    loadSource(fgraph, graph);


    //Exemple de création d'un sous-ensemble de sommets à tester :
    print("Liste créée\n");
    liste sous_graphe_desert = NULL;
    sous_graphe_desert = add_to_liste(sous_graphe_desert,0);
    sous_graphe_desert = add_to_liste(sous_graphe_desert,2);
    sous_graphe_desert = add_to_liste(sous_graphe_desert,4);


    //Résultat
    if(verification_maximalite(graph,sous_graphe_desert)){
      printf("Le sous-graphe est maximal !\n");
    }
    else {
      printf("Le sous-graphe n'est pas maximal.\n");
    }

    return 0;


}
