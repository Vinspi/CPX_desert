#include "util.h"


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
