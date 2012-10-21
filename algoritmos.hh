#ifndef ALGORITMOS_IA
#define ALGORITMOS_IA

#include <list>

typedef char int8;

struct Nodo{

  int8 estado[16];
  int distancia;
  int heuristica;
  Nodo* predecesor;
};


Nodo* a_manhattan(int peso);
Nodo* ida_manhattan(int8 estado[16],int peso);
Nodo* make_root(int8 estado[16]);
Nodo* make_node(Nodo* padre,int8 estado[16]);
std::list<Nodo*> extract_solution(Nodo* n);

#endif
