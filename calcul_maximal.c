#include "util.h"



//Fonction vérifiant si l'on a construit un maximal en observant la table d'adjacence : si tout les sommets sont marqués, on a créé un mmaximal.
//Renvoie 1 dans ce cas, 0 sinon.
int verif_maximal(Graph *g, int* tab){

	for(int i = 0; i < g->n; i++){
		if(tab[i] == 1) return 0;
	}
	return 1;

}


//Fonction construisant et renvoyant un sous-ensemble X tel que X est un sous-graphe désert maximal de G.
liste calcul_maximal(Graph *g){

    //Création de la table d'adjacence et d'un tampon.
    int tableau_adjacence[n_max], i = 0;
    for(int i = 0; i < g->n; i++);
      tableau_adjacence[i] = 1;


	  liste X = NULL;
	  liste tmp_parcour = X;

    //Tant que l'on a pas créé de maximal, on continue :
	  while(verif_maximal(g, all) != 1){
		  i = 0;

		  //Choisir le nouveau sommet : on choisira le premier sommet non-marqué que l'on trouve en parcourant la table d'adjacence.
		  while(i < g->n && tableau_adjacence[i] == 0){
			     i++;
		  }

      //On l'ajoute à la liste.
		  X = add_to_liste(X, i);
		  tmp_parcour = X;

      //Et on marque son adjacence.
      while (tmp_parcour != NULL) {

        tableau_adjacence[tmp_parcour->st] = 0;
        for(int j=0;j<g->n;j++){
				      if(g->a[tmp_parcour->st][j])
                tableau_adjacence[j] = 0;
			  }
        tmp_parcour = tmp_parcour -> suiv;
		  }



	   }


	   return X;
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


    //Résultat :
    liste X = calcul_maximal(graph);
    printf("Le sous-graphe maximal généré est :\n");
    printf_liste(X);


    return 0;


}
