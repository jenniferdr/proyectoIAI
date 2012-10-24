#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "algoritmos.hh"
#include<fstream>
#include<sstream>


int main(int argc, char *argv[]){

  if(argc!=5){
    std::cout << 
      "Error. Sintaxis: <algoritmo> <heuristica> <peso(W)> <archivo_iniciales>";
    std::cout << std::endl << "heuristica: manhattan, Mconflictos, static555 ";
    std::cout << std::endl << "algoritmo: a_estrella ida_estrella" << std::endl;
    return 1;
  }
  std::string algoritmo= std::string(argv[1]);
  if(algoritmo!= "a_estrella" && algoritmo!= "ida_estrella"){
    std::cout << "Error: algoritmo no valido";
    return 1;
  }
  std::string heuristica= std::string(argv[2]);
  if(heuristica!= "manhattan" && heuristica!= "Mconflictos"
     && heuristica!= "static555"){
    std::cout << "Error: heuristica no valida";
    return 1;
  }
  if(atoi(argv[3])<=0){
    std::cout << "Error: el peso debe ser mayor o igual a 1";
    return 1;
  }
  peso= atoi(argv[3]);
  
  std::ifstream archivo(argv[4]);
  
  if(!archivo){
    std::cout << "Ha fallado la lectura" << std::endl;
    return -1;
  }
  generarBD();
  generarTabla();
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

    std::list<Nodo*> listaResuelto;
    Nodo *nodoResuelto;

    if(algoritmo=="a_estrella"){
      if(heuristica=="manhattan") nodoResuelto= a_manhattan(estado,peso);
      if(heuristica=="Mconflictos") nodoResuelto= a_conflictos(estado,peso);
      if(heuristica=="static555") nodoResuelto= a_static555(estado,peso);
    } 
    if(algoritmo=="ida_estrella"){
      if(heuristica=="manhattan") listaResuelto= ida_manhattan(estado);
      if(heuristica=="Mconflictos") listaResuelto= ida_conflictos(estado);
      if(heuristica=="static555") listaResuelto= ida_static555(estado);
      std::cout << " " << (int) resuelto.back()->distancia;
    }

    std::cout << "\n --- \n" << std::endl;
    /*    Nodo* res = a_static555(estado, 1);
	  extract_solution(res);*/
  }
 
  return 0;
}
