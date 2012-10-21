#include "algoritmos.hh"

#include <cstring>
#include <queue>
#include <unordered_set>
#include <set>
#include <map>
#include <iostream>

bool set_todos[16] = { true, true, true, true,
		       true, true, true, true,
		       true, true, true, true,
		       true, true, true, true };

struct NodoA {
  int8 estado[16];
  char posicion_cero;
  unsigned char g;
  unsigned char f;
  NodoA* padre;

  /*  bool operator<(const NodoA* b) {
    return this->f < b->f;
    }*/
};

#include <functional>
struct CompNodo : public std::binary_function<NodoA*, NodoA*, bool> {
    bool operator()(const NodoA* lhs, const NodoA* rhs) const {
      return lhs->f == rhs->f ? lhs->g >= rhs->g : lhs->f >= rhs->f;
    }
};

long long int compactar_full(int8 estado[16]) {
  long long int res = 0;
  for (int i = 0; i < 16; i++) {
    res = (estado[i] | res) << 4;
  }
  res = res >> 4;
  return res;
}


extern bool puedeMoverse(int dist, int posicion_cero);

Nodo* a_static555(int8 inicial[16], int peso) {

  using namespace std;

  priority_queue<NodoA*, vector<NodoA*>, CompNodo> cola;
  map<long long int, NodoA*> abiertos;
  unordered_set<long long int> cerrados;

  int nodos_expandidos = 0;

  NodoA* nodo_i = new NodoA();
  memcpy(nodo_i->estado, inicial, 16*sizeof(int8));
  nodo_i->g = 0;
  nodo_i->f = peso * h_static555(nodo_i->estado);
  for (int i = 0; i < 16; i++) {
    if (inicial[i] == 0) {
      nodo_i->posicion_cero = i;
      break;
    }
  }
  nodo_i->padre = NULL;

  cola.push(nodo_i);
  abiertos[compactar_full(inicial)] = nodo_i;

  int movs[4] = {-4,-1,1,4};

  while (!cola.empty()) {

    NodoA* nodo = cola.top();
    nodos_expandidos++;

    if ( h_static555(nodo->estado) == 0 ) {
      // Consegui la solucion
      std::cout << "Res: " << (int) nodo->g << std::endl;
      std::cout << "Nodos: " << nodos_expandidos << std::endl;
      while (nodo != NULL) {
	for (int i = 0; i < 16; i++) {
	  std::cout << (int) (nodo->estado[i]) << "  ";
	}
	std::cout << std::endl << std::endl;
	nodo = nodo->padre;
      }
      return NULL;
    }

    cola.pop();
    abiertos.erase( compactar_full(nodo->estado) );
    cerrados.insert( compactar_full(nodo->estado) );

    for (int i = 0; i < 4; i++) {

      if (puedeMoverse(movs[i], nodo->posicion_cero)) {
	int8 temp[16];
	memcpy(&temp, &(nodo->estado), 16*sizeof(int8));

	char ficha = temp[nodo->posicion_cero + movs[i]];
	temp[nodo->posicion_cero] = ficha;
	temp[nodo->posicion_cero + movs[i]] = 0;

	long long int indice = compactar_full(temp);

	if ( cerrados.count(indice) > 0 ) continue;

	unsigned char g_tentativo = nodo->g + 1;

	NodoA* hijo;

	if ( abiertos.count(indice) > 0) {
	  hijo = abiertos[indice];
	  if ( g_tentativo <= hijo->g ) {
	    hijo->padre = nodo;
	    hijo->g = g_tentativo;
	    hijo->f = g_tentativo + peso * h_static555(temp);
	  }

	} else {
	  hijo = new NodoA();
	  memcpy(hijo->estado, &temp, 16*sizeof(int8));
	  hijo->posicion_cero = nodo->posicion_cero + movs[i];
	  hijo->g = g_tentativo;
	  hijo->f = g_tentativo + peso * h_static555(temp);
	  hijo->padre = nodo;

	  abiertos[indice] = hijo;
	  cola.push(hijo);
	}
      }
    }
  }

  return NULL;
}
