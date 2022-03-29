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
					fil--;
					fil_interna--;
					break;
				case 1: // Este
					col++;
					col_interna++;
					break;
				case 2: // Sur
					fil++;
					fil_interna++;
					break;
				case 3: // Oeste
					col--;
					col_interna--;
					break;
			}
			break;
		case actTURN_L:
			brujula = (brujula+3)%4;
			brujula_interna = (brujula_interna+3)%4;
			girar_derecha = (rand()%2==0);
			break;
		case actTURN_R:
			brujula = (brujula+1)%4;
			brujula_interna = (brujula_interna+1)%4;
			girar_derecha = (rand()%2==0);
			break;
	}

	// Si morimos, nos desorientamos
	if(sensores.reset){
		bien_situado = false;
		ResetearMapa_No_Posicionado();
	}

	if(sensores.terreno[0] == 'G' and !bien_situado){
		fil = sensores.posF;
		col = sensores.posC;
		brujula = sensores.sentido;
		bien_situado = true;
	}

	if(bien_situado){
		if(!bien){
			bien = true;
			CombinarMapas();
		}	
		ActualizarMapaResultado(sensores);
	}else{	
		ActualizarMapa_No_Posicionado(sensores);
		bien = false;
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

void ComportamientoJugador::ActualizarMapaResultado(Sensores sensores){
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

void ComportamientoJugador::ActualizarMapa_No_Posicionado(Sensores sensores){
	if(brujula_interna==0){	// Norte
		mapa_no_posicionado[fil_interna][col_interna] = sensores.terreno[0];
		for(int i=-1;i<=1;i++)
			mapa_no_posicionado[fil_interna-1][col_interna+i] = sensores.terreno[i+2];
		for(int i=-2;i<=2;i++)
			mapa_no_posicionado[fil_interna-2][col_interna+i] = sensores.terreno[i+6];
		for(int i=-3;i<=3;i++)
			mapa_no_posicionado[fil_interna-3][col_interna+i] = sensores.terreno[i+12];
	}
	if(brujula_interna==2){	// Sur
		mapa_no_posicionado[fil_interna][col_interna] = sensores.terreno[0];
		for(int i=-1;i<=1;i++)
			mapa_no_posicionado[fil_interna+1][col_interna+i] = sensores.terreno[-i+2];
		for(int i=-2;i<=2;i++)
			mapa_no_posicionado[fil_interna+2][col_interna+i] = sensores.terreno[-i+6];
		for(int i=-3;i<=3;i++)
			mapa_no_posicionado[fil_interna+3][col_interna+i] = sensores.terreno[-i+12];
	}
	if(brujula_interna==1){ // Este
		mapa_no_posicionado[fil_interna][col_interna] = sensores.terreno[0];
		for(int i=-1;i<=1;i++)
			mapa_no_posicionado[fil_interna+i][col_interna+1] = sensores.terreno[i+2];
		for(int i=-2;i<=2;i++)
			mapa_no_posicionado[fil_interna+i][col_interna+2] = sensores.terreno[i+6];
		for(int i=-3;i<=3;i++)
			mapa_no_posicionado[fil_interna+i][col_interna+3] = sensores.terreno[i+12];
	}
	if(brujula_interna==3){ // Oeste
		mapa_no_posicionado[fil_interna][col_interna] = sensores.terreno[0];
		for(int i=-1;i<=1;i++)
			mapa_no_posicionado[fil_interna+i][col_interna-1] = sensores.terreno[-i+2];
		for(int i=-2;i<=2;i++)
			mapa_no_posicionado[fil_interna+i][col_interna-2] = sensores.terreno[-i+6];
		for(int i=-3;i<=3;i++)
			mapa_no_posicionado[fil_interna+i][col_interna-3] = sensores.terreno[-i+12];
	}
}

void ComportamientoJugador::ResetearMapa_No_Posicionado(){
	for(int i=0;i<mapaResultado.size()*2;i++)
		mapa_no_posicionado[i].clear();
	vector<unsigned char> aux(mapaResultado.size()*2, '?');
    for (unsigned int i = 0; i < mapaResultado.size()*2; i++){
        mapa_no_posicionado.push_back(aux);
    }
	fil_interna = col_interna = 100;
    brujula_interna = 0;
}

void ComportamientoJugador::rotar_matriz_90_grados(vector<vector<unsigned char>> &M){
	vector<vector<unsigned char>>aux(M.size(),vector<unsigned char>(M.size(),'?'));
	int t;
	for(int i=0;i<M.size();i++){
		t=0;
		for(int j=M.size()-1;j>=0;--j){
			aux[i][t]=M[j][i];
			t++;
		}
	}
	M=aux;
}

void ComportamientoJugador::CombinarMapas(){
	int desfase = (brujula-brujula_interna+4)%4;
	for(int i=0;i<desfase;i++){
		rotar_matriz_90_grados(mapa_no_posicionado);
		int aux = fil_interna;
		fil_interna = col_interna;
		col_interna = mapa_no_posicionado[0].size()-aux-1;
	}
	for(int i=0;i<mapaResultado.size();i++){
		for(int j=0; j<mapaResultado[0].size();j++){
			mapaResultado[i][j] = mapa_no_posicionado[fil_interna-fil+i][col_interna-col+j];
		}
	}
}