#include <cstring>
#include <iostream>
#include <queue>

#include "algoritmos.hh"

/*
 * Particiones del 15 puzzle. Cada número es un set.
 * 0 1 1 2 
 * 1 1 2 2
 * 1 3 2 2
 * 3 3 3 3
 */

bool set1[16] = 
  {false, true, true, false,
   true, true, false, false,
   true, false, false, false,
   false, false, false, false};

bool set2[16] = 
  {false, false, false, true,
   false, false, true, true,
   false, false, true, true,
   false, false, false, false};

bool set3[16] = 
  {false, false, false, false,
   false, false, false, false,
   false, true, false, false,
   true, true, true, true};

int8 estado_inicial1[16] = 
  { 0,  1,  2, -1,
    4,  5, -1, -1,
    8, -1, -1, -1,
   -1, -1, -1, -1};

int8 estado_inicial2[16] = 
  { 0, -1, -1,  3,
   -1, -1,  6,  7,
   -1, -1, 10, 11,
   -1, -1, -1, -1};

int8 estado_inicial3[16] = 
  { 0, -1, -1, -1,
   -1, -1, -1, -1,
   -1,  9, -1, -1,
   12, 13, 14, 15};

/*
 * Bases de datos de patrones
 * Son arreglos de 6 dimensiones. Cada dimensión es la posición de una
 * ficha distinguible (o el blanco) en el tablero.
 * Van haber muchas casillas sin utilizar. Eso mientras no se me ocurre
 * una mejor manera.
 */

unsigned char bd1[16*16*16*16*16*16];
unsigned char bd2[16*16*16*16*16*16];
unsigned char bd3[16*16*16*16*16*16];

/*
 * Empaca las posiciones de las fichas distinguibles en un entero
 * para usarlas de índice en la BD.
 */
long long int compactar(char estado[16], bool set[16]) {

  /*  for (int i = 0; i< 16; i++) {
    std::cout << (int) estado[i]<<"  ";
    }*/
  int res = 0;
  int p[6];
  int indice = 0;

  for (int i = 0; i < 16; i++) {
    if (i != 0 and !set[i]) continue;

    for (int j = 0; j <16; j++) {
      if (estado[j] == i) {
	p[indice] = j;
	indice++;
      }
    }
  }

  for (int i = 0; i< 6; i++) {
    //std::cout << "shifting " << (int) p[i] << std::endl;
    res = (p[i] | res) << 4;
  }
  res = res >> 4;

  /*  std::cout << " : " << res << std::endl;*/
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

struct NodoPat {
  int8 estado[16];
  unsigned char movimientos;
  char posicion_cero;
};

int movs[4] = {-4,-1,1,4};

void llenarBDPat(bool set[16], int8 inicial[16], unsigned char bd[]) {

  NodoPat* nodo_inicial = new NodoPat();
  memcpy(&(nodo_inicial->estado), inicial, 16*sizeof(int8));
  nodo_inicial->movimientos = 0;
  nodo_inicial->posicion_cero = 0;

  bd[compactar(inicial, set)] = 0;

  std::cout << "inicial: " << compactar(inicial, set) << std::endl;

  std::queue<NodoPat*> cola;
  cola.push(nodo_inicial);

  int nodos_expandidos = 0;

  while (!cola.empty()) {
    NodoPat* nodo = cola.front();
    cola.pop();

    nodos_expandidos++;

    for (int i = 0; i < 4; i++) {

      if (puedeMoverse(movs[i], nodo->posicion_cero)) {

	int8 temp[16];
	memcpy(&temp, &(nodo->estado), 16*sizeof(int8));

	char ficha = temp[nodo->posicion_cero + movs[i]];
	temp[nodo->posicion_cero] = ficha;
	temp[nodo->posicion_cero + movs[i]] = 0;
	//	std::cout << "bla: " << (int) bd[compactar(temp, set)] << std::endl;
	if ( bd[compactar(temp, set)] == 255 ) {
	  NodoPat* hijo = new NodoPat();
	  memcpy(&(hijo->estado), &temp, 16*sizeof(int8));
	  hijo->posicion_cero = nodo->posicion_cero + movs[i];
	  hijo->movimientos = nodo->movimientos + (ficha != -1 ? 1 : 0);

	  bd[compactar(temp, set)] = hijo->movimientos;
	  cola.push(hijo);
	}
      }

    }

    delete nodo;
  }
}

void generarBD() {
  for (int i = 0; i < (16*16*16*16*16*16); i++) {
    bd1[i] = 255;
    bd2[i] = 255;
    bd3[i] = 255;
  }

  llenarBDPat(set1, estado_inicial1, bd1);
  llenarBDPat(set2, estado_inicial2, bd2);
  llenarBDPat(set3, estado_inicial3, bd3);
}

int h_static555(int8 estado[16]) {
  return bd1[compactar(estado, set1)] + bd2[compactar(estado, set2)] + bd3[compactar(estado, set3)];
}
