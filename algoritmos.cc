#include "algoritmos.hh"
#include <stdio.h>
#include <algorithm>
#include <iostream>

//Variable global para el peso
int peso=1;

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
  std::cout << "Pasos para solucionar: " << (int) n->distancia << std::endl;
  std::list<Nodo*> camino;
  while(n!= NULL){
    /*for (int i = 0; i < 16; i++) {
      std::cout << (int) n->estado[i] << "  ";
      }
      std::cout << std::endl << std::endl;*/

    camino.push_front(n);
    n= n->predecesor;
  }
  return camino;
}

int heuristica(Nodo* n){
  return  heuristicaManhattan(n->estado);
}

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

int heuristicaManhattan(int8 estado[16]){
  int sum = 0;
  for (int i = 0; i < 16; i++) {
    sum= sum + matrizH[i][estado[i]];
  };	
  return sum;
}

bool is_goal(Nodo* n){
  bool r = true;
  int matrizR[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
	
  for (int i = 0; i<16; i++) {
    if (matrizR[i]!=n->estado[i]){
      r = false;
      i =16;
    } 
  };
	
  return r;	
};

std::list<int8*> obtenerMovimientos(Nodo* n){
  // IMPLEMENTAR
  return std::list<int8*>();
}

std::pair<std::list<Nodo*>,int> DFS_acotado(Nodo* n,int t){
  
  if((n->distancia)+ peso*(n->heuristica) > t ){
    std::list<Nodo*> empty;
    return std::pair<std::list<Nodo*>,int>
      (empty,(n->distancia)+peso*(n->heuristica));
  }
  if(is_goal(n)){
    return std::pair<std::list<Nodo*>,int>((extract_solution(n)),n->distancia);
  }
  int nueva_t= 1000;

  // Sucesores del estado n
  std::list<int8*> movimientos= obtenerMovimientos(n);

  for(std::list<int8*>::iterator it= movimientos.begin(); 
      it!=movimientos.end(); it++){
    int8 *movimiento=(*it);
    Nodo *n2= make_node(n,movimiento);
    std::pair<std::list<Nodo*>,int> par= DFS_acotado(n2,t);
    if (par.first.size()!=0) return par;
    nueva_t= std::min(nueva_t,par.second);
  }
  return std::pair<std::list<Nodo*>,int>(std::list<Nodo*>(),nueva_t);
  
}


std::list<Nodo*> ida_manhattan(int8 estado[16]){
  Nodo* n= make_root(estado);
  int t= heuristica(n)*peso;
  std::list<Nodo*> plan;
  std::pair<std::list<Nodo*>,int> par;

  while(plan.size()<=0 and t<1000){
    par= DFS_acotado(n,t);
    t= par.second;
    plan= par.first;
  }
  return plan; 
}
/*
int main(int argc,char *argv[]){
  return 0;
  }*/
