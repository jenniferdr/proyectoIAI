#include <cstring>
#include <functional>
#include <iostream>
#include <unordered_map>
#include <queue>
#include <unordered_set>

#include "algoritmos.hh"

// Provee la función comparadora de nodos
// Es necesaria para que la priority_queue de la librería estándar ordene
// los nodos de acuerdo a su valor f en vez de su apuntador.
struct CompNodo : public std::binary_function<Nodo*, Nodo*, bool> {
    bool operator()(const Nodo* lhs, const Nodo* rhs) const {
      return lhs->heuristica == rhs->heuristica ?
	lhs->distancia >= rhs->distancia
	: lhs->heuristica >= rhs->heuristica;
    }
};

// Representa una configuración de un 15-puzzle como un entero de 64 bits.
long long int compactar_full(int8 estado[16]) {
  long long int res = 0;
  for (int i = 0; i < 16; i++) {
    res = (estado[i] | res) << 4;
  }
  res = res >> 4;
  return res;
}

// Definida en basepatrones.cc
// Dada una posición del blanco en el puzzle y un offset (-1, 1, 4 o -4),
// determina si ese offset representa un movimiento válido del blanco.
extern bool puedeMoverse(int dist, int posicion_cero);

/**
 * Resuelve el problema del 15-puzzle utilizando el algoritmo WA*.
 * Los parámetros son el estado inicial del puzzle a resolver, el peso
 * que se le dará a la heurística, y la función heurística.
 *
 * Esta implementación funciona bien para casos en los que la cantidad de nodos
 * es baja (alrededor de 5 millones). Usualmente el algoritmo explota
 * en tamaño de memoria, consumiendo todos los recursos del sistema y sin
 * alcanzar una solución.
 *
 * Consideramos que para mejorar esta implantación es necesario reducir
 * el tamaño de la representación de los nodos, implantar nuestras propias
 * estructuras para la cola de prioridades y conjuntos (de manera de que se
 * adapten mejor a este problema en específico), y posiblemente manejar
 * manualmente la memoria dinámica.
 */
Nodo* a_estrella(int8 inicial[16], int peso, int (*h)(int8 inicial[16])) {

  using namespace std;

  priority_queue<Nodo*, vector<Nodo*>, CompNodo> cola;
  unordered_map<long long int, Nodo*> abiertos;
  unordered_set<long long int> cerrados;

  int nodos_expandidos = 0;

  // Inicializar nodo del estado inicial
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

    // Cuando se expande un sucesor que ya está en los abiertos con un costo
    // mayor, como el API no permite borrarlo de la cola, se marca como borrado.
    // Para marcarlo se coloca su predecesor como NULL y su distancia de la raíz
    // es mayor a cero (así lo podemos distinguir de la raíz)
    // Aquí lo detectamos y liberamos la memoria asociada.
    if (nodo->predecesor == NULL && nodo->distancia != 0) {
      delete nodo;
      continue;
    }

    nodos_expandidos++;

    if ( is_goal(nodo) ) {
      std::cout << "Nodos expandidos: " << nodos_expandidos << std::endl;
      std::cout << "Pasos para la solución: " << (int) nodo->distancia << std::endl;
      return nodo;
    }

    // Expansión de sucesores
    // El arreglo movs[] guarda los offsets desde la casilla actual del blanco
    // hacia donde podría moverse. Para cada uno determinamos si es un movimiento
    // válido y expandimos el nodo de manera correspondiente.
    for (int i = 0; i < 4; i++) {

      if (puedeMoverse(movs[i], nodo->posicion_cero)) {
	int8 temp[16];
	memcpy(&temp, &(nodo->estado), 16*sizeof(int8));

	char ficha = temp[nodo->posicion_cero + movs[i]];
	temp[nodo->posicion_cero] = ficha;
	temp[nodo->posicion_cero + movs[i]] = 0;

	long long int indice = compactar_full(temp);

	// Si es un nodo que ya estaba cerrado, saltarlo
	if ( cerrados.count(indice) > 0 ) continue;

	unsigned char g_tentativo = nodo->distancia + 1;

	Nodo* hijo;

	if ( abiertos.count(indice) > 0) {
	  hijo = abiertos[indice];
	  if ( g_tentativo <= hijo->distancia ) {
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


// función wrapper convenientes. Proveen la implantación de A* con heurísticas
// particulares.
Nodo* a_static555(int8 inicial[16], int peso) {
  // 3 Bases de datos de patrones disjuntas
  return a_estrella(inicial, peso, h_static555);
}

Nodo* a_manhattan(int8 inicial[16], int peso) {
  // Distancia manhattan
  return a_estrella(inicial, peso, heuristicaManhattan);
}

Nodo* a_static55_M(int8 inicial[16], int peso) {
  // 2 Bases de datos de patrones y el resto de las fichas con distancia manhattan
  return a_estrella(inicial, peso, h_static55_M);
}

Nodo* a_static5_M(int8 inicial[16], int peso) {
  // 1 base de datos de patrones y el resto distancia manhattan
  return a_estrella(inicial, peso, h_static5_M);
}

Nodo* a_conflictos(int8 inicial[16], int peso) {
  // Distancia manhattan con conflictos lineales en filas
  return a_estrella(inicial, peso, h_conflictos);
}
