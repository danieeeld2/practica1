#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"
using namespace std;

class ComportamientoJugador : public Comportamiento{

  public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size){
      // Constructor de la clase
      // Dar el valor inicial a las variables de estado

      fil = col = 99;
      brujula = 0;
      ultimaAccion = actIDLE;
      girar_derecha = false;
      bien_situado = false;

      // Inicializamos la matriz auxiliar al máximo de los mapas por dos
      // evitando así desbordamientos
      vector<unsigned char> aux(mapaResultado.size()*2, '?');
      for (unsigned int i = 0; i < mapaResultado.size()*2; i++){
        mapa_no_posicionado.push_back(aux);
      }
      // Inicializamos en medio del mapa interno, mirando al norte
      fil_interna = col_interna = mapaResultado.size();
      brujula_interna = 0;
      bien = false;
    }

    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);
    int interact(Action accion, int valor);
    void ActualizarMapaResultado(Sensores sensores);
    void ActualizarMapa_No_Posicionado(Sensores sensores);
    void ResetearMapa_No_Posicionado();
    void CombinarMapas();


  private:
  
  // Declarar aquí las variables de estado

  // Donde estoy y donde voy a dar el siguiente paso
  int fil, col, brujula;
  // Actualización de información y última acción realizada
  Action ultimaAccion;

  bool girar_derecha;
  bool bien_situado;

  // Pasa a estar situado bien por primera vez
  bool bien;

  // Matriz interna auxiliar
  vector<vector<unsigned char>> mapa_no_posicionado;
  // Posicion interna auxiliar
  int fil_interna, col_interna, brujula_interna;

  // Función para rotar 90 grados a la derecha una matriz
  void rotar_matriz_90_grados(vector<vector<unsigned char>> &M);


};

#endif
