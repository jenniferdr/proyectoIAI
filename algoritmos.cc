#include "algoritmos.hh"
#include <stdio.h>
#include <algorithm>
#include <iostream>
#include <cstring>


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
  return camino;
}

int8 heuristica(Nodo* n){
  return (int8) heuristicaManhattan(n->estado);
}

int8 matrizH[16][16] =
  { {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
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
  return matrizH[estado[0]][0] + matrizH[estado[1]][1]
    + matrizH[estado[2]][2] + matrizH[estado[3]][3]
    + matrizH[estado[4]][4] + matrizH[estado[5]][5]
    + matrizH[estado[6]][6] + matrizH[estado[7]][7]
    + matrizH[estado[8]][8] + matrizH[estado[9]][9]
    + matrizH[estado[10]][10] + matrizH[estado[11]][11]
    + matrizH[estado[12]][12] + matrizH[estado[13]][13]
    + matrizH[estado[14]][14] + matrizH[estado[15]][15];
}

int8 matrizR[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

bool is_goal(Nodo* n){
  bool r = true;
  for (int i = 0; i<16 && r; i++) {
    r = r && matrizR[i]==n->estado[i];
  }
  return r;
};
