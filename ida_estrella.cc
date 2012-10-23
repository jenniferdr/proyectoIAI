#include "algoritmos.hh"
#include <stdio.h>
#include <algorithm>
#include <iostream>
#include <cstring>

//Variable global para el peso
int peso=1;
int num_nodos;

std::pair<std::list<Nodo*>,int> DFS_acotado(Nodo* n,int t){
  //std::cout << (int) n->distancia << std::endl;
  if((n->distancia)+ peso*(heuristica(n)) > t ){
    std::list<Nodo*> empty;
    return std::pair<std::list<Nodo*>,int>
      (empty,(n->distancia)+peso*(heuristica(n)));
  }
  if(is_goal(n)){
    return std::pair<std::list<Nodo*>,int>((extract_solution(n)),n->distancia);
  }
  int nueva_t= 1000;

  // Sucesores del estado n
  std::list<int8> listMov;
  int posCero;

  //buscar posicion del cero
  for(int i=0; i<16 ; i++){
    if(n->estado[i]==0){
      posCero=i;
      break;
    }
  }

  if(puedeMoverse(1,posCero)) listMov.push_front(1);
  if(puedeMoverse(-1,posCero)) listMov.push_front(-1);
  if(puedeMoverse(4,posCero)) listMov.push_front(4);
  if(puedeMoverse(-4,posCero)) listMov.push_front(-4);
  
  for(std::list<int8>::iterator it= listMov.begin(); 
      it!=listMov.end(); it++){
    num_nodos++;
    //std::cout << num_nodos << "\n";
    int8 movimiento=(*it);
    int8 nuevoEstado[16];
    // Generar el nuevo estado
    memcpy(nuevoEstado,n->estado,sizeof(int8)*16);
    nuevoEstado[posCero]= nuevoEstado[posCero+movimiento];
    nuevoEstado[posCero+movimiento]=0;

    Nodo *n2= make_node(n,nuevoEstado);
    
    std::pair<std::list<Nodo*>,int> par= DFS_acotado(n2,t);
    if(par.first.size()!=0) return par;
    delete(n2);
    nueva_t= std::min(nueva_t,par.second);
  }
    
  return std::pair<std::list<Nodo*>,int>(std::list<Nodo*>(),nueva_t);
  
}


std::list<Nodo*> ida_manhattan(int8 estado[16]){
  Nodo* n= make_root(estado);
  int t= heuristica(n)*peso;
  std::list<Nodo*> plan;
  std::pair<std::list<Nodo*>,int> par;
  num_nodos=0;
  
  while(plan.size()<=0 and t<1000){
    par= DFS_acotado(n,t);
    t= par.second;
    std::cout << " " << t << std::endl;
    plan= par.first;
    
  }
  std::cout <<" "<< num_nodos;
  return plan;
}
