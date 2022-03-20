#include "../Comportamientos_Jugador/jugador.hpp"
#include <iostream>
using namespace std;



Action ComportamientoJugador::think(Sensores sensores){

	Action accion = actIDLE;

	cout << "Posicion: fila " << sensores.posF << " columna " << sensores.posC << " ";
	switch(sensores.sentido){
		case 0: cout << "Norte" << endl; break;
		case 1: cout << "Este" << endl; break;
		case 2: cout << "Sur " << endl; break;
		case 3: cout << "Oeste" << endl; break;
	}
	cout << "Terreno: ";
	for (int i=0; i<sensores.terreno.size(); i++)
		cout << sensores.terreno[i];
	cout << endl;

	cout << "Superficie: ";
	for (int i=0; i<sensores.superficie.size(); i++)
		cout << sensores.superficie[i];
	cout << endl;

	cout << "Colisión: " << sensores.colision << endl;
	cout << "Reset: " << sensores.reset << endl;
	cout << "Vida: " << sensores.vida << endl;
	cout << endl;


	// Determinar el efecto de la ultima accion enviada
	switch(ultimaAccion){
		case actFORWARD:
			/*Depende de la orientación y por convención supondremos
			que el norte está en los valores bajos de fil (sur en los altos)
			y que oeste esta en los valores bajos de col (este en los altos)*/
			switch(brujula){
				case 0: //Norte
					fil--;break;
				case 1: // Este
					col++;break;
				case 2: // Sur
					fil++;break;
				case 3: // Oeste
					col--;break;
			}
			break;
		case actTURN_L:
			brujula = (brujula+3)%4;
			girar_derecha = (rand()%2==0);
			break;
		case actTURN_R:
			brujula = (brujula+1)%4;
			girar_derecha = (rand()%2==0);
			break;
	}

	// Si morimos, nos desorientamos
	if(sensores.reset){
		bien_situado = false;
	}

	if(sensores.terreno[0] == 'G' and !bien_situado){
		fil = sensores.posF;
		col = sensores.posC;
		brujula = sensores.sentido;
		bien_situado = true;
	}

	if(bien_situado){
		
		if(brujula==0){	// Norte
			mapaResultado[fil][col] = sensores.terreno[0];
			for(int i=-1;i<=1;i++)
				mapaResultado[fil-1][col+i] = sensores.terreno[i+2];
			for(int i=-2;i<=2;i++)
				mapaResultado[fil-2][col+i] = sensores.terreno[i+6];
			for(int i=-3;i<=3;i++)
				mapaResultado[fil-3][col+i] = sensores.terreno[i+12];
		}
		if(brujula==2){	// Sur
			mapaResultado[fil][col] = sensores.terreno[0];
			for(int i=-1;i<=1;i++)
				mapaResultado[fil+1][col+i] = sensores.terreno[-i+2];
			for(int i=-2;i<=2;i++)
				mapaResultado[fil+2][col+i] = sensores.terreno[-i+6];
			for(int i=-3;i<=3;i++)
				mapaResultado[fil+3][col+i] = sensores.terreno[-i+12];
		}
		if(brujula==1){ // Este
			mapaResultado[fil][col] = sensores.terreno[0];
			for(int i=-1;i<=1;i++)
				mapaResultado[fil+i][col+1] = sensores.terreno[i+2];
			for(int i=-2;i<=2;i++)
				mapaResultado[fil+i][col+2] = sensores.terreno[i+6];
			for(int i=-3;i<=3;i++)
				mapaResultado[fil+i][col+3] = sensores.terreno[i+12];
		}
		if(brujula==3){ // Oeste
			mapaResultado[fil][col] = sensores.terreno[0];
			for(int i=-1;i<=1;i++)
				mapaResultado[fil+i][col-1] = sensores.terreno[-i+2];
			for(int i=-2;i<=2;i++)
				mapaResultado[fil+i][col-2] = sensores.terreno[-i+6];
			for(int i=-3;i<=3;i++)
				mapaResultado[fil+i][col-3] = sensores.terreno[-i+12];
		}
	}

	// Accion

	if((sensores.terreno[2]=='T' or sensores.terreno[2]=='S' or sensores.terreno[2]=='G') 
	    and (sensores.superficie[2]=='_')){
		accion = actFORWARD;
	}
	else if(!girar_derecha){
		accion = actTURN_L;
	}
	else{
		accion = actTURN_R;
	}
	

	ultimaAccion = accion;
	return accion;
}

int ComportamientoJugador::interact(Action accion, int valor){
  return false;
}
