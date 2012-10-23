#include "algoritmos.hh"
#include <stdio.h>
#include <algorithm>
#include <iostream>
#include <cstring>

//Variable global para el peso
int peso=1;
int num_nodos;
const int movimientos[]= {-1,1,4,-4};

std::pair<Nodo*,int> DFS_acotado(Nodo* n,int t){
  //std::cout << (int) n->distancia << std::endl;
  if((n->distancia)+ peso*(heuristica(n)) > t ){
    return std::pair<Nodo*,int>(NULL,(n->distancia)+peso*(heuristica(n)));
  }
  if(is_goal(n)){
    return std::pair<Nodo*,int>(n,n->distancia);
  }
  int nueva_t= 1000;

  int posCero;
  //buscar posicion del cero
  for(int i=0; i<16 ; i++){
    if(n->estado[i]==0){
      posCero=i;
      break;
    }
  }
  
  for(int i=0; i<4 ;i++){

    if(!puedeMoverse(movimientos[i],posCero)) continue;

    int8 nuevoEstado[16];
    // Generar el nuevo estado
    memcpy(nuevoEstado,n->estado,sizeof(int8)*16);
    nuevoEstado[posCero]= nuevoEstado[posCero+movimientos[i]];
    nuevoEstado[posCero+movimientos[i]]=0;

    num_nodos++;
    //std::cout << num_nodos << "\n";

    Nodo *n2= make_node(n,nuevoEstado);
    std::pair<Nodo*,int> par= DFS_acotado(n2,t);
    if(par.first!=NULL) return par;
    delete(n2);
    nueva_t= std::min(nueva_t,par.second);
  }
    
  return std::pair<Nodo*,int>(NULL,nueva_t);
  
}


std::list<Nodo*> ida_manhattan(int8 estado[16]){
  Nodo* n= make_root(estado);
  int t= heuristica(n)*peso;
  Nodo* solucion;
  std::pair<Nodo*,int> par;
  num_nodos=0;
  
  while(solucion==NULL and t<1000){
    par= DFS_acotado(n,t);
    solucion= par.first;
    t= par.second;
    std::cout << " " << t;
  }
  std::cout <<" "<< num_nodos;
  return extract_solution(solucion);
}
