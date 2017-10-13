#include "util.h"


//Fonction vérifiant si la liste x, qui implémente un sous-ensemble de sommets, est un sous-graphe désert dans le graphe G.
//Retourne 1 si x est un sous-graphe désert, 0 sinon.
int verification_graphe_desert(Graph *g, liste x){

	liste tmp_parcour = x, tmp_parcour2;
	int resultat = 1;

	while(tmp_parcour != NULL && resultat == 1){
		tmp_parcour2 = tmp_parcour -> suiv;
		while (tmp_parcour2 != NULL && resultat == 1) {
			if(g->a[tmp_parcour->st][tmp_parcour2->st])
				resultat = 0;
			tmp_parcour2 = tmp_parcour2 -> suiv;
		}
		tmp_parcour = tmp_parcour -> suiv;
	}

	return resultat;
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


    //Exemple de création d'un sous-ensemble de sommets à tester :
    liste sous_graphe_desert = NULL;
    sous_graphe_desert = add_to_liste(sous_graphe_desert,0);
    sous_graphe_desert = add_to_liste(sous_graphe_desert,2);
    sous_graphe_desert = add_to_liste(sous_graphe_desert,5);


    //Résultat
    if(verification_graphe_desert(graph,sous_graphe_desert)){
      printf("Le sous-graphe X est désert !\n");
    }
    else {
      printf("Le sous-graphe X n'est pas désert.\n");
    }

    return 0;


}
