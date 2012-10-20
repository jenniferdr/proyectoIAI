#include <cstring>
#include <queue>
#include <unordered_map>

#include "algoritmos.hh"

#include <iostream>

typedef std::unordered_map<long long int, unsigned char> BDPat;

BDPat bd1, bd2, bd3;

/*
 * Particiones del 15 puzzle. Cada número es un set.
 * 0 1 1 2 
 * 1 1 2 2
 * 1 3 2 2
 * 3 3 3 3
 */
bool set1[16] = {false, true, true, false, true, true, false, false, true, false, false, false, false, false, false, false};      
bool set2[16] = {false, false, false, true, false, false, true, true, false, false, true, true, false, false, false, false};
bool set3[16] = {false, false, false, false, false, false, false, false, false, true, false, false, true, true, true, true};

int8 estado_inicial1[16] = {0, 1, 2, 7, 3, 4, 7, 7, 5, 7, 7, 7, 7, 7, 7, 7};

int8 estado_inicial2[16] = {0, 1, 1, 3, 1, 1, 6, 7, 1, 1, 10, 11, 1, 1, 1, 1};

int8 estado_inicial3[16] = {0, 1, 1, 1, 1, 1, 1, 1, 1, 9, 1, 1, 12, 13, 14, 15};

/*
 * Modifica 3 maps globales de estados a enteros
 */



struct NodoPat {
  int8 estado[16];
  unsigned char movimientos;
  char posicion_cero;
};

int movs[4] = {-4,-1,1,4};

long long int compactar(char estado[16]) {
  long long int res = 0;
  for (int i = 0; i < 16; i++) {
    res += (long long int) estado[i];
    res = res << 4;
  }
  return res;
}

bool puedeMoverse(int dist, int posicion_cero) {
  if (posicion_cero + dist > 15 or posicion_cero + dist < 0) return false;

  switch (posicion_cero) {
  case 4:
  case 8:
  case 12:
    return dist != -1;
    break;
  case 3:
  case 7:
  case 11:
    return dist != 1;
  }

  return true;
}

void llenarBDPat(bool set[16], int8 inicial[16], BDPat* bd) {
  /* meter el puzzle inicial en la tabla de hash con costo 0 */
  NodoPat* nodo_inicial = new NodoPat();
  memcpy(&(nodo_inicial->estado), inicial, 16*sizeof(int8));

  std::cout << "Estado inicial: ";
  for (int i=0; i< 16; i++) {
      std::cout << (int) nodo_inicial->estado[i] << " ";
  }
  std::cout << std::endl;

  nodo_inicial->movimientos = 0;
  nodo_inicial->posicion_cero = 0;
  (*bd)[compactar(inicial)] = 0;

  /* meter puzzle inicial en la cola */
  std::queue<NodoPat*> cola;
  cola.push(nodo_inicial);

  int cola_size = 0;

  while (!cola.empty()) {
    NodoPat* nodo = cola.front();
    cola.pop();

    cola_size = cola_size > cola.size() ? cola_size : cola.size();

    /*    std::cout << "Expandiendo: ";
    for (int i=0; i< 16; i++) {
      std::cout << (int) nodo->estado[i] << " ";
    }
    std::cout << std::endl;*/

    for (int i = 0; i < 4; i++) {

      if (puedeMoverse(movs[i], nodo->posicion_cero)) {

	NodoPat* hijo = new NodoPat();
	memcpy(hijo, nodo, sizeof(NodoPat));

	char ficha = hijo->estado[hijo->posicion_cero + movs[i]];
	hijo->estado[hijo->posicion_cero] = ficha;
	hijo->estado[hijo->posicion_cero + movs[i]] = 0;
	hijo->posicion_cero += movs[i];
	hijo->movimientos += set[ficha] ? 1 : 0;

	long long int indice = compactar(hijo->estado);
	if ( bd->count(indice) == 0) {

	  (*bd)[indice] = hijo->movimientos;
	  cola.push(hijo);

	  /*	  std::cout << "Nuevo!: "; // << std::endl;
	  for (int i=0; i< 16; i++) {
	    std::cout << (int) hijo->estado[i] << " ";
	  }
	  std::cout << std::endl; */

	} else {
	  delete hijo;
	}
      }

    }

    delete nodo;
  }
}

void generarBD() {
  bd1.reserve(5765760);
  bd2.reserve(5765760);
  bd3.reserve(5765760);

  std::cout << bd1.max_load_factor() << std::endl;
  std::cout << bd1.size() << std::endl;
  std::cout << bd1.max_size() << std::endl;



  llenarBDPat(set1, estado_inicial1, &bd1);

  std::cout << bd1.max_load_factor() << std::endl;
  std::cout << bd1.size() << std::endl;
  std::cout << bd1.max_size() << std::endl;

  /*  llenarBDPat(set2, estado_inicial2, &bd2);
      llenarBDPat(set3, estado_inicial3, &bd3);*/
}

int main() {
  generarBD();

  /*  std::cout << "BD1" << std::endl;

  for (std::pair<long long int, unsigned char> p : bd1) {
    std::cout << "Clave: " << p.first << ", Valor: " << (int) p.second << std::endl;
  }

  std::cout << "BD2" << std::endl;

  for (std::pair<long long int, unsigned char> p : bd2) {
    std::cout << "Clave: " << p.first << ", Valor: " << p.second << std::endl;
  }

  std::cout << "BD3" << std::endl;

  for (std::pair<long long int, unsigned char> p : bd3) {
    std::cout << "Clave: " << p.first << ", Valor: " << p.second << std::endl;
    }*/

}
