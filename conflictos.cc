#include <cstring>
#include "algoritmos.hh"
#include <iostream>

int conflictos[16][16][16][16];

void generarTabla() {

  memset(conflictos, 0, sizeof(int)*16*16*16*16);

  for (int f1 = 1; f1 <= 3; f1++) {
    for (int f2 = f1+1; f2 <= 3; f2++) {

      for (int p1 = 0; p1 <= 3; p1++) {
	for (int p2 = 0; p2 <=3; p2++) {

	  if (p1==p2) continue;

	  if (p1>=p2) {
	    conflictos[f1][f2][p1][p2] = 2;
	    conflictos[f2][f1][p2][p1] = 2;
	  }
	}
      }
    }
  }

  for (int f1 = 4; f1 <= 7; f1++) {
    for (int f2 = f1+1; f2 <= 7; f2++) {

      for (int p1 = 4; p1 <= 7; p1++) {
	for (int p2 = 4; p2 <=7; p2++) {

	  if (p1==p2) continue;

	  if (p1>=p2) {
	    conflictos[f1][f2][p1][p2] = 2;
	    conflictos[f2][f1][p2][p1] = 2;
	  }
	}
      }
    }
  }


  for (int f1 = 8; f1 <= 11; f1++) {
    for (int f2 = f1+1; f2 <= 11; f2++) {

      for (int p1 = 8; p1 <= 11; p1++) {
	for (int p2 = 8; p2 <=11; p2++) {

	  if (p1==p2) continue;

	  if (p1>=p2) {
	    conflictos[f1][f2][p1][p2] = 2;
	    conflictos[f2][f1][p2][p1] = 2;
	  }
	}
      }
    }
  }

  for (int f1 = 12; f1 <= 15; f1++) {
    for (int f2 = f1+1; f2 <= 15; f2++) {

      for (int p1 = 12; p1 <= 15; p1++) {
	for (int p2 = 12; p2 <=15; p2++) {

	  if (p1==p2) continue;

	  if (p1>=p2) {
	    conflictos[f1][f2][p1][p2] = 2;
	    conflictos[f2][f1][p2][p1] = 2;
	  }
	}
      }
    }
  }
}

int h_conflictos(int8 estado[16]) {
  int res = heuristicaManhattan(estado);
  for (int i = 0; i < 4; i++) {
    for (int j = i; j < 4; j++) {
      res += conflictos[estado[i]][estado[j]][i][j];
    }
  }
  for (int i = 4; i < 8; i++) {
    for (int j = i; j < 8; j++) {
      res += conflictos[estado[i]][estado[j]][i][j];
    }
  }
  for (int i = 8; i < 12; i++) {
    for (int j = i; j < 12; j++) {
      res += conflictos[estado[i]][estado[j]][i][j];
    }
  }
  for (int i = 8; i < 16; i++) {
    for (int j = i; j < 16; j++) {
      res += conflictos[estado[i]][estado[j]][i][j];
    }
  }

  return res;
}
