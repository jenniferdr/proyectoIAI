#include <cstring>
#include <iostream>
#include <fstream>
#include <queue>

#include "algoritmos.hh"

/**
 * Funciones para generar la base de datos de patrones disjunta
 * y calcular la heurística basada en ella.
 * Se particionó el puzzle en 3 conjuntos de 5 fichas cada una:
 *
 *    0 1 1 2
 *    1 1 2 2
 *    1 3 2 2
 *    3 3 3 3
 *
 * Cada base de datos se representa como un arreglo de 6 dimensiones con
 *  16 posiciones cada una. Para obtener la heurística dado un patrón,
 * se indexa utilizando la posición de cada ficha relvante del patrón.
 * Este esquema no es el más eficiente posible, pero las 3 bases de datos
 * juntas ocupan 50 Mb, lo cual nos pareció un tamaño razonable.
 *
 * Las bases de datos se generan comenzando con el estado final del puzzle
 * y luego utilizando BFS se desarma hasta obtener todas las configuraciones
 * posibles. Cuando se consigue una configuración nueva, se guarda en la base
 * de datos la distancia a la raíz, contando solo los pasos que involucren
 * mover una ficha del conjunto relevante a esa base de datos.
 *
 * La heurística parece funcionar en a mayoría de las pruebas, sin embargo,
 * existen algunos casos en los que sobreestima la distancia al goal. No
 * se pudo determinar por qué, así que por ello no confiamos demasiado en
 * nuestra implementación de esta heurística.
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
 */

unsigned char bd1[16*16*16*16*16*16];
unsigned char bd2[16*16*16*16*16*16];
unsigned char bd3[16*16*16*16*16*16];

/*
 * Empaca las posiciones de las fichas distinguibles en un entero
 * para usarlas de índice en la BD.
 */
int compactar(char estado[16], bool set[16]) {

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
    res = (p[i] | res) << 4;
  }
  res = res >> 4;

  return res;
}

// Determina si dada una posición del blanco en el puzzle, y un offset,
// es posible mover el blanco a la posición del blanco sumado ese offset.
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

/*
 * Utilizando BFS llena la base de datos dada desarmando el estado inicial dado.
 */
void llenarBDPat(bool set[16], int8 inicial[16], unsigned char bd[]) {

  NodoPat* nodo_inicial = new NodoPat();
  memcpy(&(nodo_inicial->estado), inicial, 16*sizeof(int8));
  nodo_inicial->movimientos = 0;
  nodo_inicial->posicion_cero = 0;

  bd[compactar(inicial, set)] = 0;

  std::queue<NodoPat*> cola;
  cola.push(nodo_inicial);

  int nodos_expandidos = 0;

  int movs[4] = {-4,-1,1,4};

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
	if ( bd[compactar(temp, set)] == 255 ) {
	  NodoPat* hijo = new NodoPat();
	  memcpy(&(hijo->estado), &temp, 16*sizeof(int8));
	  hijo->posicion_cero = nodo->posicion_cero + movs[i];
	  hijo->movimientos = nodo->movimientos + (ficha > 0 ? 1 : 0);
	  bd[compactar(temp, set)] = hijo->movimientos;
	  cola.push(hijo);
	} else {
	  unsigned char val1 = bd[compactar(temp, set)];
	  unsigned char val2 = nodo->movimientos + (ficha > 0 ? 1 : 0);
	  bd[compactar(temp, set)] = val1 < val2 ? val1 : val2;
	}
      }
    }
    delete nodo;
  }
}

void generarBD() {
  // Llena la base de datos inicialmente con 'infinito'
  for (int i = 0; i < (16*16*16*16*16*16); i++) {
    bd1[i] = 255;
    bd2[i] = 255;
    bd3[i] = 255;
  }

  // Si un archivo con nombre 'patrones.bd' ya existe, se carga de allí en vez
  // de generarla. Sino, se genera y se escribe en disco.
  std::ifstream entrada("patrones.bd", std::ios::in | std::ios::binary);
  if (!entrada) {
    std::cout << "No existe la base de datos de patrones, se generará" << std::endl;
    llenarBDPat(set1, estado_inicial1, bd1);
    llenarBDPat(set2, estado_inicial2, bd2);
    llenarBDPat(set3, estado_inicial3, bd3);

    std::ofstream salida;
    salida.open("patrones.bd", std::ios::out | std::ios::binary);
    salida.write((char*) bd1, 16*16*16*16*16*16*sizeof(unsigned char));
    salida.write((char*) bd2, 16*16*16*16*16*16*sizeof(unsigned char));
    salida.write((char*) bd3, 16*16*16*16*16*16*sizeof(unsigned char));
    salida.close();

  } else {
    entrada.read((char*) bd1, 16*16*16*16*16*16*sizeof(unsigned char));
    entrada.read((char*) bd2, 16*16*16*16*16*16*sizeof(unsigned char));
    entrada.read((char*) bd3, 16*16*16*16*16*16*sizeof(unsigned char));
    entrada.close();
  }
}

// Matriz para heurística Manhattan.
extern int matrizH[16][16];

/*
 * La heurística toma el estado actual, y para cada conjunto de fichas, usa su
 * posición actual como índice en la base de datos para buscar el patrón
 * correspondiente. Luego se suman los 3 valores obtenidos.
 */
int h_static555(int8 estado[16]) {
  return bd1[compactar(estado, set1)] + bd2[compactar(estado, set2)]
    + bd3[compactar(estado, set3)];
}

int h_static55_M(int8 estado[16]) {
  // Busca el estado actual en dos bases de datos
  int res = bd1[compactar(estado, set1)] + bd2[compactar(estado, set2)];
  // A eso le suma la distancia manhattan del resto de las fichas
  for (int i = 0; i < 16; i++) {
    if (!set3[i]) continue;
    for (int j = 0; j < 16; j++) {
      if (estado[j] == i) {
	res += matrizH[i][j];
      }
    }
  }
  return res;
}

int h_static5_M(int8 estado[16]) {
  // Busca el estado actual en la primera base de datos y suma la distancia
  // manhattan del resto de las fichas.
  int res = bd1[compactar(estado, set1)];
  for (int i = 0; i < 16; i++) {
    if (set1[i]) continue;
    for (int j = 0; j < 16; j++) {
      if (estado[j] == i) {
	res += matrizH[i][j];
      }
    }
  }
  return res;
}
