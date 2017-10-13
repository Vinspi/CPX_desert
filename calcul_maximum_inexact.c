#include "util.h"





//Fonction vérifiant si l'on a construit un maximal en observant la table de degré : si tout les sommets ont un degré non-marqué négatif, càd qu'ils sont marqués, alors on a créé un mmaximal.
//Renvoie 1 dans ce cas, 0 sinon.
int verif_maximal2(Graph *g){


	for(int i = 0; i < g->n; i++){
		if(g->degre[i] > -1) return 0;
	}
	return 1;

}


//Fonction retournant le premier sommet disponible dont le degré non-marqué est minimum.
int sommet_degre_min(Graph *g){

	liste x = NULL;
	int min = n_max, imin = -1;
	for(int i = 0; i < g->n; i++){
		if(g->degre[i] > -1 && g->degre[i] < min){
			imin = i;
			min = g->degre[i];
		}
	}

	return imin;

}


//Fonction construisant et renvoyant un sous-ensemble X tel que X est une approximation de(s) sous-graphe(s) désert(s) maximum de G.
liste calcul_maximum_inexacte(Graph *g){


    int i;
	  liste X = NULL;

    //Tant que l'on a pas créé de maximal, on continue :
	  while(verif_maximal2(g) == 0){


		  //On choisit le nouveau sommet : ici, on choisira le premier sommet non-marqué de degré minimum que l'on trouvera en parcourant la table des degrés.
      i = sommet_degre_min(g);

      //On l'ajoute ensuite à la liste.
		  X = add_to_liste(X, i);


      //Et on marque son adjacence.
      g->degre[i] = -1;
      	for(int j=0;j<g->n;j++){
          if(g->a[i][j] == 1 && g->degre[j] > -1){
            g->degre[j] = -1;
            for(int z = 0; z < g->n; z++)
              if(g->a[j][z] == 1)
                (g->degre[z])--;
          }
        }
	   }

	   return X;
}




int main(int argc, char *argv[]){

    if(argc != 2){
      printf("USAGE : ./calcul_maximal_inexact fichier_contenant_le_graphe\n");
      exit(-1);
    }


    //Création du graphe.
    Graph *graph = malloc(sizeof(Graph));
    char* fgraph = argv[1];
    loadSource(fgraph, graph);


    //Résultat :
    liste X = calcul_maximum_inexacte(graph);
    printf("Le sous-graphe maximum approché généré est :\n");
    printf_liste(X);


    return 0;


}
