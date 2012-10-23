#include "algoritmos.hh"
#include <stdio.h>
#include <algorithm>
#include <iostream>
#include <cstring>
#include <cstdlib>

//Variable global para el peso
int peso=1;
long long int num_nodos;
const int movimientos[]= {4,-1,-4,1};
Nodo nodos[1000];

std::pair<Nodo*,int> DFS_acotado(int nodo,int t){
  //std::cout << (int) n->distancia << std::endl;
  Nodo* n = &nodos[nodo];

  int c = n->distancia + peso*heuristica(n);
  if ( c > t ){
    return std::pair<Nodo*,int>(NULL,c);
  }

  if (is_goal(n)) {
    return std::pair<Nodo*,int>(n,n->distancia);
  }

  int nueva_t = 1000;

  int posCero;
  //buscar posicion del cero
  for(int i = 0; i < 16; i++){
    if(n->estado[i]==0){
      posCero=i;
      break;
    }
  }
  
  for(int i=0; i<4 ;i++){

    if(!puedeMoverse(movimientos[i],posCero)) continue;

    int8* nuevoEstado = nodos[nodo+1].estado;
 
    // Generar el nuevo estado
    memcpy(nuevoEstado,n->estado,sizeof(int8)*16);
    nuevoEstado[posCero]= nuevoEstado[posCero+movimientos[i]];
    nuevoEstado[posCero+movimientos[i]]=0;

    num_nodos++;
    if (num_nodos % 10000000 == 0) { std::cout << "Expandi " << num_nodos << std::endl; }

    //	  if (num_nodos == 500000000) { std::exit(0); }
    //std::cout << num_nodos << "\n";

    nodos[nodo+1].distancia = nodos[nodo].distancia + 1;
    nodos[nodo+1].predecesor = n;

    std::pair<Nodo*,int> par= DFS_acotado(nodo+1, t);
    if (par.first!=NULL) return par;
    nueva_t= std::min(nueva_t,par.second);
  }
    
  return std::pair<Nodo*,int>(NULL,nueva_t);
  
}


std::list<Nodo*> ida_manhattan(int8 estado[16]){

  for (int i = 0; i < 16; i++) {
    nodos[0].estado[i] = estado[i];
  }
  nodos[0].distancia = 0;
  nodos[0].predecesor = NULL;

  int t = heuristica(&nodos[0])*peso;
  Nodo* solucion;
  std::pair<Nodo*,int> par;


  while (solucion==NULL and t<1000){
  num_nodos=0;
    par = DFS_acotado(0,t);
    solucion= par.first;
    t= par.second;
    std::cout << " " << t << std::endl;
  }
  std::cout <<" "<< num_nodos;
  return extract_solution(solucion);
}
