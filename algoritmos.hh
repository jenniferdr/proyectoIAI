#ifndef ALGORITMOS_IA
#define ALGORITMOS_IA

#include <list>

typedef char int8;

struct Nodo{
  int8 estado[16];
  int8 distancia;
  int8 heuristica;
  Nodo* predecesor;
  char posicion_cero;
};

// A*
Nodo* a_manhattan(int8 estado[16], int peso);
Nodo* a_static5_M(int8 estado[16], int peso);
Nodo* a_static55_M(int8 estado[16], int peso);
Nodo* a_static555(int8 estado[16], int peso);
Nodo* a_conflictos(int8 estado[16], int peso);

// IDA*
std::list<Nodo*> ida_manhattan(int8 estado[16]);


// Heurísticas
int8 heuristica(Nodo* n);
int h_static555(int8 estado[16]);
int h_static55_M(int8 estado[16]);
int h_static5_M(int8 estado[16]);
void generarBD();

// Util
Nodo* make_root(int8 estado[16]);
Nodo* make_node(Nodo* padre,int8 estado[16]);
std::list<Nodo*> extract_solution(Nodo* n);
int heuristicaManhattan(int8 estado[16]);
bool is_goal(Nodo* n);
bool puedeMoverse(int dist, int posicion_cero);

void generarTabla();
int h_conflictos(int8 estado[16]);

#endif
