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

int main(){
  // main dummy para poder compilar 
  return 0;
}
