#include "algoritmos.hh"
#include <stdio.h>

Nodo* make_root(int8 estado[16]){
  Nodo* n= new Nodo();

  for(int i=0; i<16;i++){
    n->estado[i]= estado[i];
  }
  n->distancia= 0;
  n->predecesor= NULL;

  return n;  
}

Nodo* make_node(Nodo* padre,int8 estado[16]){
  Nodo* n= new Nodo();
  
  for(int i=0; i<16;i++){
    n->estado[i]= estado[i];
  }
  n->distancia= padre->distancia +1;
  n->predecesor= padre;

  return n;  
}

std::list<Nodo*> extract_solution(Nodo* n){
  std::list<Nodo*> camino;
  while(n!= NULL){
    camino.push_front(n);
    n= n->predecesor;
  } 

}
int heuristicaManhattan(int8 estado[16]){
	int sum;
	int matrizH[16][16] = 
	{ {0, 1, 2, 3, 1, 2, 3, 4, 2, 3, 4, 5, 3, 4, 5, 6},
	  {1, 0, 1, 2, 2, 1, 2, 3, 3, 2, 3, 4, 4, 3, 4, 5},
	  {2, 1, 0, 1, 3, 2, 1, 2, 4, 3, 2, 3, 5, 4, 3, 4},
	  {3, 2, 1, 0, 4, 3, 2, 1, 5, 4, 3, 2, 6, 5, 4, 3},
	  {1, 2, 3, 4, 0, 1, 2, 3, 1, 2, 3, 4, 2, 3, 4, 5},
	  {2, 1, 2, 3, 1, 0, 1, 2, 2, 1, 2, 3, 3, 2, 3, 4},
	  {3, 2, 1, 2, 2, 1, 0, 1, 3, 2, 1, 2, 4, 3, 2, 3},
	  {4, 3, 2, 1, 3, 2, 1, 0, 4, 3, 2, 1, 5, 4, 3, 2},
	  {2, 3, 4, 5, 1, 2, 3, 4, 0, 1, 2, 3, 1, 2, 3, 4},
	  {3, 2, 3, 4, 2, 1, 2, 3, 1, 0, 1, 2, 2, 1, 2, 3},
	  {4, 3, 2, 3, 3, 2, 1, 2, 2, 1, 0, 1, 3, 2, 1, 2},
	  {5, 4, 3, 2, 4, 3, 2, 1, 3, 2, 1, 0, 4, 3, 2, 1},
	  {3, 4, 5, 6, 2, 3, 4, 5, 1, 2, 3, 4, 0, 1, 2, 3},
	  {4, 3, 4, 5, 3, 2, 3, 4, 2, 1, 2, 3, 1, 0, 1, 2},
	  {5, 4, 3, 4, 4, 3, 2, 3, 3, 2, 1, 2, 2, 1, 0, 1},
	  {6, 5, 4, 3, 5, 4, 3, 2, 4, 3, 2, 1, 3, 2, 1, 0}
	  };
	  
	for (int i = 0; i < 16; i++) {
			sum= matrizH[i][estado[i]];
		};
	
	
		return sum;
	}

bool is_goal(Nodo* n){
	bool r = true;
	int matrizR[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
	
	for (int i = 0; i < 16; i++) {
			if (matrizR[i]!=n->estado[i]){
				r = false;
				i =16;
			} 
		};
	
	return r;
	
	};
	
int heuristica(Nodo* n){
	int r;
	r = heuristicaManhattan(n->estado);
	return r;
}
	
	
int main(){
  // main dummy para poder compilar 
  return 0;
}
