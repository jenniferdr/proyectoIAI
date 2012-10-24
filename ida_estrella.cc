#include "algoritmos.hh"
#include <stdio.h>
#include <algorithm>
#include <iostream>
#include <cstring>
#include <cstdlib>

//Variable global para el peso
int peso;
long long int num_nodos;
const int movimientos[]= {4,-1,-4,1};
Nodo nodos[1000];

std::pair<Nodo*,int> DFS_acotado(int nodo,int t,int (*h)(int8 estado[16])){
  //std::cout << (int) n->distancia << std::endl;
  Nodo* n = &nodos[nodo];

  int c = n->distancia + peso*(*h)(n->estado);
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
   
    //	  if (num_nodos == 500000000) { std::exit(0); }
    //std::cout << num_nodos << "\n";

    nodos[nodo+1].distancia = nodos[nodo].distancia + 1;
    nodos[nodo+1].predecesor = n;

    std::pair<Nodo*,int> par= DFS_acotado(nodo+1, t,h);
    if (par.first!=NULL) return par;
    nueva_t= std::min(nueva_t,par.second);
  }
  return std::pair<Nodo*,int>(NULL,nueva_t);
}


std::list<Nodo*> ida_estrella(int8 estado[16], int (*h)(int8 inicial[16])){
  // Por eficiencia todos los nodos estaran en nodos[]
  
  // Atributos del nodo raiz
  for (int i = 0; i < 16; i++) {
    nodos[0].estado[i] = estado[i];
  }
  nodos[0].distancia = 0;
  nodos[0].predecesor = NULL;

  // (*h) es la funcion heuristica
  int t = (*h)((&nodos[0])->estado)*peso;
  Nodo* solucion;
  std::pair<Nodo*,int> par;

  while (solucion==NULL and t<1000){
    num_nodos=0;
    par = DFS_acotado(0,t,h);
    solucion= par.first;
    t= par.second;
    std::cout << " " << t;
  }
  std::cout <<" "<< num_nodos;
  return extract_solution(solucion);
}

// función wrapper convenientes. Proveen la implementación de IDA* con heurísticas
// particulares.
std::list<Nodo*> ida_static555(int8 inicial[16]) {
  // 3 Bases de datos de patrones disjuntas
  return ida_estrella(inicial, h_static555);
}

std::list<Nodo*> ida_manhattan(int8 inicial[16]) {
  // Distancia manhattan
  return ida_estrella(inicial, heuristicaManhattan);
}

std::list<Nodo*> ida_static55_M(int8 inicial[16]) {
  // 2 Bases de datos de patrones y el resto de las fichas con distancia manhattan
  return ida_estrella(inicial, h_static55_M);
}

std::list<Nodo*> ida_static5_M(int8 inicial[16]) {
  // 1 base de datos de patrones y el resto distancia manhattan
  return ida_estrella(inicial, h_static5_M);
}

std::list<Nodo*> ida_conflictos(int8 inicial[16]) {
  // Distancia manhattan con conflictos lineales en filas
  return ida_estrella(inicial, h_conflictos);
}
