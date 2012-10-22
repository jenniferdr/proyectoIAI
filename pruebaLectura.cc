#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "algoritmos.hh"
#include<fstream>
#include<sstream>


int main(){
  // FALTA hacer las validaciones necesarias 
  // Y leer de la linea de comandos el nombre del archivo y peso

  std::ifstream archivo("prueba.txt");
  
  if(!archivo){
    std::cout << "Ha fallado la lectura" << std::endl;
    return -1;
  }
  char linea[40];

  while(archivo.getline(linea,40)){
    std::cout << linea << " : ";

    char estado[16];
    
    std::istringstream stream(linea,std::istringstream::in);
    for(int i=0; i<16;i++){
      char tile[3];
      stream >> tile;
      estado[i]= atoi(tile);
    }

    std::list<Nodo*> resuelto= ida_manhattan(estado);
    /*
    for(int i=0; i<16 ;i++){
      int a = (int)(resuelto.front())->estado[i];
      std::cout << a << " ";
      }*/
    std::cout << "\n --- \n";
    
    
  }
 
  return 0;
}
