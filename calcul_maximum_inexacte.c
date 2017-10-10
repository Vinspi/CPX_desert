#include "util.h"



//Insérer Inexacte.


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
    liste X = calcul_maximum_inexacte(graph);
    printf("Le sous-graphe maximum approché généré est :\n");
    printf_liste(X);


    return 0;


}
