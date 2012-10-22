#include <cstring>
#include <functional>
#include <iostream>
#include <unordered_map>
#include <queue>
#include <unordered_set>

#include "algoritmos.hh"

struct CompNodo : public std::binary_function<Nodo*, Nodo*, bool> {
    bool operator()(const Nodo* lhs, const Nodo* rhs) const {
      return lhs->heuristica == rhs->heuristica ?
	lhs->distancia >= rhs->distancia
	: lhs->heuristica >= rhs->heuristica;
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

Nodo* a_estrella(int8 inicial[16], int peso, int (*h)(int8 inicial[16])) {

  using namespace std;

  priority_queue<Nodo*, vector<Nodo*>, CompNodo> cola;
  unordered_map<long long int, Nodo*> abiertos;
  unordered_set<long long int> cerrados;

  int nodos_expandidos = 0;

  Nodo* nodo_i = new Nodo();
  memcpy(nodo_i->estado, inicial, 16*sizeof(int8));
  nodo_i->distancia = 0;
  nodo_i->heuristica = peso * (*h)(nodo_i->estado);
  for (int i = 0; i < 16; i++) {
    if (inicial[i] == 0) {
      nodo_i->posicion_cero = i;
      break;
    }
  }
  nodo_i->predecesor = NULL;

  cola.push(nodo_i);
  abiertos[compactar_full(inicial)] = nodo_i;

  int movs[4] = {-4,-1,1,4};

  while (!cola.empty()) {

    Nodo* nodo = cola.top();

    cola.pop();
    abiertos.erase( compactar_full(nodo->estado) );
    cerrados.insert( compactar_full(nodo->estado) );

    if (nodo->predecesor == NULL && nodo->distancia != 0) {
      // Encontré un nodo borrado de la cola, hacer delete e ignorar
      delete nodo;
      continue;
    }

    nodos_expandidos++;

    if ( is_goal(nodo) ) {
      std::cout << "Nodos expandidos: " << nodos_expandidos << std::endl;
      return nodo;
    }

    for (int i = 0; i < 4; i++) {

      if (puedeMoverse(movs[i], nodo->posicion_cero)) {
	int8 temp[16];
	memcpy(&temp, &(nodo->estado), 16*sizeof(int8));

	char ficha = temp[nodo->posicion_cero + movs[i]];
	temp[nodo->posicion_cero] = ficha;
	temp[nodo->posicion_cero + movs[i]] = 0;

	long long int indice = compactar_full(temp);

	if ( cerrados.count(indice) > 0 ) continue;

	unsigned char g_tentativo = nodo->distancia + 1;

	Nodo* hijo;

	if ( abiertos.count(indice) > 0) {
	  hijo = abiertos[indice];
	  if ( g_tentativo <= hijo->distancia ) {
	    // Para no tener que revisar toda la cola para eliminar el
	    // hijo que fue insertado y que hay que reemplazar,
	    // Lo marcamos como 'borrado' y cuando los consigamos,
	    // lo ignoramos.
	    hijo->predecesor = NULL;

	    Nodo* nhijo = new Nodo();
	    memcpy(nhijo->estado, &temp, 16*sizeof(int8));
	    nhijo->posicion_cero = nodo->posicion_cero + movs[i];
	    nhijo->distancia = g_tentativo;
	    nhijo->heuristica = g_tentativo + peso * (*h)(temp);
	    nhijo->predecesor = nodo;
	    abiertos[indice] = nhijo;
	    cola.push(nhijo);
	  }

	} else {
	  hijo = new Nodo();
	  memcpy(hijo->estado, &temp, 16*sizeof(int8));
	  hijo->posicion_cero = nodo->posicion_cero + movs[i];
	  hijo->distancia = g_tentativo;
	  hijo->heuristica = g_tentativo + peso * (*h)(temp);
	  hijo->predecesor = nodo;

	  abiertos[indice] = hijo;
	  cola.push(hijo);
	}
      }

    }
  }

  return NULL;
}

Nodo* a_static555(int8 inicial[16], int peso) {
  return a_estrella(inicial, peso, h_static555);
}

Nodo* a_manhattan(int8 inicial[16], int peso) {
  return a_estrella(inicial, peso, heuristicaManhattan);
}

Nodo* a_static55_M(int8 inicial[16], int peso) {
  return a_estrella(inicial, peso, h_static55_M);
}

Nodo* a_static5_M(int8 inicial[16], int peso) {
  return a_estrella(inicial, peso, h_static5_M);
}

/*

  using namespace std;

  priority_queue<Nodo*, vector<Nodo*>, CompNodo> cola;
  map<long long int, Nodo*> abiertos;
  unordered_set<long long int> cerrados;

  int nodos_expandidos = 0;

  Nodo* nodo_i = new Nodo();
  memcpy(nodo_i->estado, inicial, 16*sizeof(int8));
  nodo_i->distancia = 0;
  nodo_i->heuristica = peso * h_static555(nodo_i->estado);
  for (int i = 0; i < 16; i++) {
    if (inicial[i] == 0) {
      nodo_i->posicion_cero = i;
      break;
    }
  }
  nodo_i->predecesor = NULL;

  cola.push(nodo_i);
  abiertos[compactar_full(inicial)] = nodo_i;

  int movs[4] = {-4,-1,1,4};

  while (!cola.empty()) {

    Nodo* nodo = cola.top();
    nodos_expandidos++;

    if ( h_static555(nodo->estado) == 0 ) {
      // Consegui la solucion
      std::cout << "Res: " << (int) nodo->distancia << std::endl;
      std::cout << "Nodos: " << nodos_expandidos << std::endl;
      while (nodo != NULL) {
	for (int i = 0; i < 16; i++) {
	  std::cout << (int) (nodo->estado[i]) << "  ";
	}
	std::cout << std::endl << std::endl;
	nodo = nodo->predecesor;
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

	unsigned char g_tentativo = nodo->distancia + 1;

	Nodo* hijo;

	if ( abiertos.count(indice) > 0) {
	  hijo = abiertos[indice];
	  if ( g_tentativo <= hijo->distancia ) {
	    hijo->predecesor = nodo;
	    hijo->distancia = g_tentativo;
	    hijo->heuristica = g_tentativo + peso * h_static555(temp);
	  }

	} else {
	  hijo = new Nodo();
	  memcpy(hijo->estado, &temp, 16*sizeof(int8));
	  hijo->posicion_cero = nodo->posicion_cero + movs[i];
	  hijo->distancia = g_tentativo;
	  hijo->heuristica = g_tentativo + peso * h_static555(temp);
	  hijo->predecesor = nodo;

	  abiertos[indice] = hijo;
	  cola.push(hijo);
	}
      }
    }
  }

  return NULL;
}
*/
