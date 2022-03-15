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
    }

    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);
    int interact(Action accion, int valor);

  private:
  
  // Declarar aquí las variables de estado

  // Donde estoy y donde voy a dar el siguiente paso
  int fil, col, brujula;
  // Actualización de información y última acción realizada
  Action ultimaAccion;

  bool girar_derecha;
  bool bien_situado;


};

#endif
