#ifndef ALGORITMOS_IA
#define ALGORITMOS_IA

typedef char int8;

struct Nodo {

  int8[16] estado;
  int distancia;
  int heuristica;
  Nodo* predecesor;

};

Nodo* a_manhattan(int peso);
Nodo* ida_manhattan(int peso);

#endif
