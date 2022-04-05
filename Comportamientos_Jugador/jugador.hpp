#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"

using namespace std;

class ComportamientoJugador : public Comportamiento {

public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size) {
        // Constructor de la clase
        // Dar el valor inicial a las variables de estado

        fil = col = 99;
        brujula = 0;
        ultimaAccion = actIDLE;
        girar_derecha = false;
        bien_situado = false;

        // Inicializamos la matriz auxiliar al máximo de los mapas por dos
        // evitando así desbordamientos
        vector<unsigned char> aux(mapaResultado.size() * 2, '?');
        for (unsigned int i = 0; i < mapaResultado.size() * 2; i++) {
            mapa_no_posicionado.push_back(aux);
        }
        // Inicializamos en medio del mapa interno, mirando al norte
        fil_interna = col_interna = mapaResultado.size();
        brujula_interna = 0;
        bien = false;

        casilla_bikini = casilla_posicionamiento = casilla_recarga = casilla_zapatilla = false;
        recarga = bikini = posicionamiento = zapatilla = 0;
        restaurar_orientacion = 0;
        comprobado = interesante = false;
        tengo_bikini = tengo_zapatillas = false;

        vector<int> aux2(mapaResultado.size(), 0);
        for (int i = 0; i < mapaResultado.size(); i++) {
            matriz_grid.push_back(aux2);
        }

        pair<int, int> aux3(0, 0);
        for (int i = 0; i < 4; i++)
            carga_del_grid.push_back(aux3);

        region = 0;
        tendencia = false;
        explorado = false;
        acabadeevaluar = false;
        obstaculo = true;
        contador_inactividad = 0;
        posicion_estatica.first = posicion_estatica.second = 0;
        aparecer = true;
        siguiendo_muro = false;
        for (int i = 0; i < 4; i++)
            reset_regiones.push_back(0);
        acaba_de_recargar = false;
        muro = false;
        noexplorar = false;
    }

    ComportamientoJugador(const ComportamientoJugador &comport) : Comportamiento(comport) {}

    ~ComportamientoJugador() {}

    Action think(Sensores sensores);

    int interact(Action accion, int valor);

    void ActualizarMapaResultado(Sensores sensores);

    void ActualizarMapa_No_Posicionado(Sensores sensores);

    void CombinarMapas();

    void ComprobarVision(Sensores sensores);

    void BuscarInteres(int posicion);

    vector <vector<unsigned char>>
    ResetearMapa_No_Posicionado(int &brujula_interna, int &fil_interna, int &col_interna);

    bool PegadoAPared(Sensores sensores);

    void Dividircargas();

    int ElegirRegion();

    void ResetearRegion();

    void Girar(int region);

    void CalcularTendendia();

    void Dividircargas_Interna();

    void SeguirMuro(Sensores sensores);

    Action MovimientoNoUbicado(Sensores sensores);

    Action MovimientoUbicado(Sensores sensores);

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

    bool tengo_zapatillas;
    bool tengo_bikini;

    // Matriz interna auxiliar
    vector <vector<unsigned char>> mapa_no_posicionado;
    // Posicion interna auxiliar
    int fil_interna, col_interna, brujula_interna;

    // Función para rotar 90 grados a la derecha una matriz
    void rotar_matriz_90_grados(vector <vector<unsigned char>> &M);

    // Booleanas auxiliares por si vemos algo interesante en el rango de vision
    bool casilla_bikini;
    int bikini;
    bool casilla_zapatilla;
    int zapatilla;
    bool casilla_recarga;
    int recarga;
    bool casilla_posicionamiento;
    int posicionamiento;
    int restaurar_orientacion;
    bool comprobado;
    bool interesante;
    vector <Action> acciones;

    // Variables para movimiento aleatorio
    vector <vector<int>> matriz_grid;

    void cargar_grid();
    int comprobacion();
    int comprobacion2();

    vector <pair<int, int>> carga_del_grid;
    int region;
    vector <Action> actionconprob;
    bool tendencia;
    bool explorado;
    bool acabadeevaluar;
    bool obstaculo;
    pair<int, int> posicion_estatica;
    int contador_inactividad;
    bool aparecer;
    bool siguiendo_muro;
    vector<int> reset_regiones;
    bool acaba_de_recargar;
    bool muro;
    bool noexplorar;
};

#endif
