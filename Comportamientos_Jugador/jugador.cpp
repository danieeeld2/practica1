#include "../Comportamientos_Jugador/jugador.hpp"
#include <iostream>

using namespace std;

Action ComportamientoJugador::think(Sensores sensores) {

    Action accion = actFORWARD;

    cout << "Posicion: fila " << sensores.posF << " columna " << sensores.posC << " ";
    switch (sensores.sentido) {
        case 0:
            cout << "Norte" << endl;
            break;
        case 1:
            cout << "Este" << endl;
            break;
        case 2:
            cout << "Sur " << endl;
            break;
        case 3:
            cout << "Oeste" << endl;
            break;
    }
    cout << "Terreno: ";
    for (int i = 0; i < sensores.terreno.size(); i++)
        cout << sensores.terreno[i];
    cout << endl;

    cout << "Superficie: ";
    for (int i = 0; i < sensores.superficie.size(); i++)
        cout << sensores.superficie[i];
    cout << endl;

    cout << "Colisión: " << sensores.colision << endl;
    cout << "Reset: " << sensores.reset << endl;
    cout << "Vida: " << sensores.vida << endl;
    cout << endl;

    // Determinar el efecto de la ultima accion enviada
    if(!sensores.colision){
        switch (ultimaAccion) {
            case actFORWARD:
                /*Depende de la orientación y por convención supondremos
                que el norte está en los valores bajos de fil (sur en los altos)
                y que oeste esta en los valores bajos de col (este en los altos)*/
                switch (brujula) {
                    case 0: // Norte
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
                for (int i = 0; i < 4; i++)
                    reset_regiones[i] = 0;
                break;
            case actTURN_L:
                brujula = (brujula + 3) % 4;
                brujula_interna = (brujula_interna + 3) % 4;
                girar_derecha = (rand() % 2 == 0);
                break;
            case actTURN_R:
                brujula = (brujula + 1) % 4;
                brujula_interna = (brujula_interna + 1) % 4;
                girar_derecha = (rand() % 2 == 0);
                break;
        }
    }

    if(sensores.terreno[0] != 'B' and sensores.terreno[0] != 'A' and sensores.terreno[0] != 'G'
    and sensores.terreno[0] != 'X' and sensores.terreno[0] != 'D' and sensores.terreno[0] != 'K'){
        acabadeaparecer = false;
    }
    

    // Si morimos, nos desorientamos
    if (sensores.reset) {
        bien_situado = false;
        acciones.clear();
        interesante = false;
        mapa_no_posicionado.clear();
        mapa_no_posicionado = ResetearMapa_No_Posicionado(brujula_interna, fil_interna, col_interna);
        casilla_bikini = casilla_posicionamiento = casilla_recarga = casilla_zapatilla = false;
        recarga = bikini = posicionamiento = zapatilla = 0;
        siguiendo_muro = false;
        tendencia = false;
        obstaculo = false;
        muro = false;
    }

    if ((sensores.terreno[0] == 'G' or sensores.nivel == 0) and !bien_situado) {
        fil = sensores.posF;
        col = sensores.posC;
        brujula = sensores.sentido;
        bien_situado = true;
    }

    if (bien_situado) {
        if (posicion_estatica.first == fil and posicion_estatica.second == col) {
            contador_inactividad++;
        } else {
            contador_inactividad = 0;
        }
    } else {
        if (posicion_estatica.first == fil_interna and posicion_estatica.second == col_interna) {
            contador_inactividad++;
        } else {
            contador_inactividad = 0;
        }
    }

    if (bien_situado) {
        if (!bien) {
            bien = true;
            CombinarMapas();
            actionconprob.clear();
            tendencia = false;
        }
        if(!sensores.colision)
            ActualizarMapaResultado(sensores);
    } else {
        if(!sensores.colision)
            ActualizarMapa_No_Posicionado(sensores);
        bien = false;
    }

    // Accion
    if (acciones.size() == 0) {
        interesante = false;
        ComprobarVision(sensores);
    }
    if (interesante) {
        siguiendo_muro = false;
        if (casilla_posicionamiento and !bien_situado) {
            if (acciones.size() == 0)
                BuscarInteres(posicionamiento);
        } else if (casilla_zapatilla and !tengo_zapatillas) {
            if (acciones.size() == 0)
                BuscarInteres(zapatilla);
        } else if (casilla_bikini and !tengo_bikini) {
            if (acciones.size() == 0)
                BuscarInteres(bikini);
        } else if (casilla_recarga and sensores.bateria < 2500 and !acaba_de_recargar) {
            if (acciones.size() == 0)
                BuscarInteres(recarga);
            acaba_de_recargar = true;
        }
        actionconprob.clear();
    }
    if (acciones.size() != 0) {
        accion = acciones[0];
        acciones.erase(acciones.begin());
    } else {
        // if(siguiendo_muro and actionconprob.size() == 0){
        // 	SeguirMuro(sensores);
        // }
        if (actionconprob.size() != 0) {
            accion = actionconprob[0];
            actionconprob.erase(actionconprob.begin());
        } else {
            if (bien_situado) {
                accion = this->MovimientoUbicado(sensores);
            } else {
                accion = this->MovimientoNoUbicado(sensores);
            }
        }
    }

    // Si hay un lobo o aldeano esperemos a que este se mueva
    if (sensores.superficie[2] == 'a' or sensores.superficie[2] == 'l') {
        muro = false;
        tendencia = true;
        obstaculo = false;
        accion = rand()%2 == 0 ? actTURN_L : actTURN_R;
        // accion = actTURN_L;
    }

    if (sensores.terreno[0] == 'K' and !tengo_bikini) {
        tengo_bikini = true;
    }
    if (sensores.terreno[0] == 'D' and !tengo_zapatillas) {
        tengo_zapatillas = true;
    }

    if (bien_situado) {
        posicion_estatica.first = fil;
        posicion_estatica.second = col;
    } else {
        posicion_estatica.first = fil_interna;
        posicion_estatica.second = col_interna;
    }

    if (contador_inactividad > 5) {
        if (sensores.terreno[2] != 'M') {
            accion = actFORWARD;
        }
    }

    if((sensores.vida <= 10 or sensores.bateria <= 100) and !inferido){
        inferido = true;
        InferirCasilla();
    }

    cout << endl
         << actionconprob.size() << "," << acciones.size() << endl;

    ultimaAccion = accion;
    return accion;
}

/*----------------------------------------------------------------------------*/

int ComportamientoJugador::interact(Action accion, int valor) {
    return false;
}

void ComportamientoJugador::ActualizarMapaResultado(Sensores sensores) {
    if (brujula == 0) { // Norte
        mapaResultado[fil][col] = sensores.terreno[0];
        matriz_grid[fil][col]++;
        for (int i = -1; i <= 1; i++) {
            mapaResultado[fil - 1][col + i] = sensores.terreno[i + 2];
            matriz_grid[fil - 1][col + i]++;
        }
        for (int i = -2; i <= 2; i++) {
            mapaResultado[fil - 2][col + i] = sensores.terreno[i + 6];
            matriz_grid[fil - 2][col + i]++;
        }
        for (int i = -3; i <= 3; i++) {
            mapaResultado[fil - 3][col + i] = sensores.terreno[i + 12];
            matriz_grid[fil - 3][col + i]++;
        }
    }
    if (brujula == 2) { // Sur
        mapaResultado[fil][col] = sensores.terreno[0];
        matriz_grid[fil][col]++;
        for (int i = -1; i <= 1; i++) {
            mapaResultado[fil + 1][col + i] = sensores.terreno[-i + 2];
            matriz_grid[fil + 1][col + i]++;
        }
        for (int i = -2; i <= 2; i++) {
            mapaResultado[fil + 2][col + i] = sensores.terreno[-i + 6];
            matriz_grid[fil + 2][col + i]++;
        }
        for (int i = -3; i <= 3; i++) {
            mapaResultado[fil + 3][col + i] = sensores.terreno[-i + 12];
            matriz_grid[fil + 3][col + i]++;
        }
    }
    if (brujula == 1) { // Este
        mapaResultado[fil][col] = sensores.terreno[0];
        matriz_grid[fil][col]++;
        for (int i = -1; i <= 1; i++) {
            mapaResultado[fil + i][col + 1] = sensores.terreno[i + 2];
            matriz_grid[fil + i][col + 1]++;
        }
        for (int i = -2; i <= 2; i++) {
            mapaResultado[fil + i][col + 2] = sensores.terreno[i + 6];
            matriz_grid[fil + i][col + 2]++;
        }
        for (int i = -3; i <= 3; i++) {
            mapaResultado[fil + i][col + 3] = sensores.terreno[i + 12];
            matriz_grid[fil + i][col + 3]++;
        }
    }
    if (brujula == 3) { // Oeste
        mapaResultado[fil][col] = sensores.terreno[0];
        matriz_grid[fil][col]++;
        for (int i = -1; i <= 1; i++) {
            mapaResultado[fil + i][col - 1] = sensores.terreno[-i + 2];
            matriz_grid[fil + i][col - 1]++;
        }
        for (int i = -2; i <= 2; i++) {
            mapaResultado[fil + i][col - 2] = sensores.terreno[-i + 6];
            matriz_grid[fil + i][col - 2]++;
        }
        for (int i = -3; i <= 3; i++) {
            mapaResultado[fil + i][col - 3] = sensores.terreno[-i + 12];
            matriz_grid[fil + i][col - 3]++;
        }
    }
}

void ComportamientoJugador::ActualizarMapa_No_Posicionado(Sensores sensores) {
    if (brujula_interna == 0) { // Norte
        mapa_no_posicionado[fil_interna][col_interna] = sensores.terreno[0];
        for (int i = -1; i <= 1; i++)
            mapa_no_posicionado[fil_interna - 1][col_interna + i] = sensores.terreno[i + 2];
        for (int i = -2; i <= 2; i++)
            mapa_no_posicionado[fil_interna - 2][col_interna + i] = sensores.terreno[i + 6];
        for (int i = -3; i <= 3; i++)
            mapa_no_posicionado[fil_interna - 3][col_interna + i] = sensores.terreno[i + 12];
    }
    if (brujula_interna == 2) { // Sur
        mapa_no_posicionado[fil_interna][col_interna] = sensores.terreno[0];
        for (int i = -1; i <= 1; i++)
            mapa_no_posicionado[fil_interna + 1][col_interna + i] = sensores.terreno[-i + 2];
        for (int i = -2; i <= 2; i++)
            mapa_no_posicionado[fil_interna + 2][col_interna + i] = sensores.terreno[-i + 6];
        for (int i = -3; i <= 3; i++)
            mapa_no_posicionado[fil_interna + 3][col_interna + i] = sensores.terreno[-i + 12];
    }
    if (brujula_interna == 1) { // Este
        mapa_no_posicionado[fil_interna][col_interna] = sensores.terreno[0];
        for (int i = -1; i <= 1; i++)
            mapa_no_posicionado[fil_interna + i][col_interna + 1] = sensores.terreno[i + 2];
        for (int i = -2; i <= 2; i++)
            mapa_no_posicionado[fil_interna + i][col_interna + 2] = sensores.terreno[i + 6];
        for (int i = -3; i <= 3; i++)
            mapa_no_posicionado[fil_interna + i][col_interna + 3] = sensores.terreno[i + 12];
    }
    if (brujula_interna == 3) { // Oeste
        mapa_no_posicionado[fil_interna][col_interna] = sensores.terreno[0];
        for (int i = -1; i <= 1; i++)
            mapa_no_posicionado[fil_interna + i][col_interna - 1] = sensores.terreno[-i + 2];
        for (int i = -2; i <= 2; i++)
            mapa_no_posicionado[fil_interna + i][col_interna - 2] = sensores.terreno[-i + 6];
        for (int i = -3; i <= 3; i++)
            mapa_no_posicionado[fil_interna + i][col_interna - 3] = sensores.terreno[-i + 12];
    }
}

vector <vector<unsigned char>>
ComportamientoJugador::ResetearMapa_No_Posicionado(int &brujula_interna, int &fil_interna, int &col_interna) {
    vector <vector<unsigned char>> mapa_no_posicionado;
    vector<unsigned char> aux(mapaResultado.size() * 2, '?');
    for (unsigned int i = 0; i < mapaResultado.size() * 2; i++) {
        mapa_no_posicionado.push_back(aux);
    }
    // Inicializamos en medio del mapa interno, mirando al norte
    fil_interna = col_interna = mapaResultado.size();
    brujula_interna = 0;
    return mapa_no_posicionado;
}

void ComportamientoJugador::rotar_matriz_90_grados(vector <vector<unsigned char>> &M) {
    vector <vector<unsigned char>> aux(M.size(), vector<unsigned char>(M.size(), '?'));
    int t;
    for (int i = 0; i < M.size(); i++) {
        t = 0;
        for (int j = M.size() - 1; j >= 0; --j) {
            aux[i][t] = M[j][i];
            t++;
        }
    }
    M = aux;
}

void ComportamientoJugador::CombinarMapas() {
    int desfase = (brujula - brujula_interna + 4) % 4;
    for (int i = 0; i < desfase; i++) {
        rotar_matriz_90_grados(mapa_no_posicionado);
        int aux = fil_interna;
        fil_interna = col_interna;
        col_interna = mapa_no_posicionado[0].size() - aux - 1;
    }
    for (int i = 0; i < mapaResultado.size(); i++) {
        for (int j = 0; j < mapaResultado[0].size(); j++) {
            if (mapaResultado[i][j] == '?' and mapa_no_posicionado[fil_interna - fil + i][col_interna - col + j] != '?')
                mapaResultado[i][j] = mapa_no_posicionado[fil_interna - fil + i][col_interna - col + j];
        }
    }
    cargar_grid();
}

void ComportamientoJugador::ComprobarVision(Sensores sensores) {
    for (int i = 1; i < sensores.terreno.size(); i++) {
        switch (sensores.terreno[i]) {
            case 'G':
                casilla_posicionamiento = interesante = true;
                posicionamiento = i;
                break;
            case 'X':
                casilla_recarga = interesante = true;
                recarga = i;
                break;
            case 'D':
                casilla_zapatilla = interesante = true;
                zapatilla = i;
                break;
            case 'K':
                casilla_bikini = interesante = true;
                bikini = i;
                break;
        }
    }
}

void ComportamientoJugador::BuscarInteres(int posicion) {
    if (posicion == 2 or posicion == 6 or posicion == 12)
        acciones.push_back(actFORWARD);
    if (posicion == 1 or posicion == 4 or posicion == 5 or posicion == 9 or posicion == 10 or posicion == 11) {
        acciones.push_back(actFORWARD);
        acciones.push_back(actTURN_L);
        acciones.push_back(actFORWARD);
        acciones.push_back(actTURN_R);
    }
    if (posicion == 3 or posicion == 7 or posicion == 13 or posicion == 8 or posicion == 14 or posicion == 15) {
        acciones.push_back(actFORWARD);
        acciones.push_back(actTURN_R);
        acciones.push_back(actFORWARD);
        acciones.push_back(actTURN_L);
    }
}

// bool ComportamientoJugador::PegadoAPared(Sensores sensores){
// 	bool aux = false;
// 	for(int i=1;i<=3;i++)
// 		if(sensores.terreno[i] == 'M' or sensores.terreno[i] == 'P')
// 			aux = true;
// 	return aux;
// }

void ComportamientoJugador::cargar_grid() {
    for (int i = 0; i < mapaResultado.size(); i++) {
        for (int j = 0; j < mapaResultado.size(); j++) {
            if (mapaResultado[i][j] != '?')
                matriz_grid[i][j]++;
        }
    }
}

void ComportamientoJugador::Dividircargas() {
    // Region1 -- Norte
    for (int i = 0; i <= fil; i++) {
        for (int j = col - 3; j <= col + 3; j++) {
            carga_del_grid[0].first++;
            if (matriz_grid[i][j] == 0)
                carga_del_grid[0].second++;
        }
    }
    // Region2 -- Este
    for (int i = fil - 3; i <= fil + 3; i++) {
        for (int j = mapaResultado.size() - 1; j >= col; --j) {
            carga_del_grid[1].first++;
            if (matriz_grid[i][j] == 0)
                carga_del_grid[1].second++;
        }
    }
    // Region3 -- Sur
    for (int i = fil; i < mapaResultado.size(); i++) {
        for (int j = col - 3; j <= col + 3; j++) {
            carga_del_grid[2].first++;
            if (matriz_grid[i][j] == 0)
                carga_del_grid[2].second++;
        }
    }
    // Region4 -- Oeste
    for (int i = fil - 3; i <= fil + 3; i++) {
        for (int j = 0; j <= col; j++) {
            carga_del_grid[3].first++;
            if (matriz_grid[i][j] == 0)
                carga_del_grid[3].second++;
        }
    }
}

int ComportamientoJugador::ElegirRegion() {
    int region;
    int max = 0;
    for (int i = 0; i < 4; i++) {
        int carga1 =
                carga_del_grid[i].second / carga_del_grid[i].first + carga_del_grid[i].second % carga_del_grid[i].first;
        if (carga1 > max) {
            region = i;
            max = carga1;
        }
    }

    return region;
}

void ComportamientoJugador::ResetearRegion() {
    for (int i = 0; i < 4; i++) {
        carga_del_grid[i].first = 0;
        carga_del_grid[i].second = 0;
    }
}

void ComportamientoJugador::Girar(int region) {
    int orientacion_optima = 0;
    switch (region) {
        case 0:
            orientacion_optima = 0;
            break;
        case 1:
            orientacion_optima = 1;
            break;
        case 2:
            orientacion_optima = 2;
            break;
        case 3:
            orientacion_optima = 3;
            break;
    }
    int desfase = (brujula - orientacion_optima + 4) % 4;
    for (int i = 0; i < desfase; i++) {
        actionconprob.push_back(actTURN_L);
    }
}

void ComportamientoJugador::Dividircargas_Interna() {
    // Region1 -- Norte
    for (int i = 0; i <= fil_interna; i++) {
        for (int j = col_interna - 3; j <= col_interna + 3; j++) {
            carga_del_grid[0].first++;
            if (mapa_no_posicionado[i][j] == '?')
                carga_del_grid[0].second++;
        }
    }
    // Region2 -- Este
    for (int i = fil_interna - 3; i <= fil_interna + 3; i++) {
        for (int j = mapa_no_posicionado.size() - 1; j >= col_interna; --j) {
            carga_del_grid[1].first++;
            if (mapa_no_posicionado[i][j] == '?')
                carga_del_grid[1].second++;
        }
    }
    // Region3 -- Sur
    for (int i = fil_interna; i < mapa_no_posicionado.size(); i++) {
        for (int j = col_interna - 3; j <= col_interna + 3; j++) {
            carga_del_grid[2].first++;
            if (mapa_no_posicionado[i][j] == '?')
                carga_del_grid[2].second++;
        }
    }
    // Region4 -- Oeste
    for (int i = fil_interna - 3; i <= fil_interna + 3; i++) {
        for (int j = 0; j <= col_interna; j++) {
            carga_del_grid[3].first++;
            if (mapa_no_posicionado[i][j] == '?')
                carga_del_grid[3].second++;
        }
    }
}

int ComportamientoJugador::comprobacion(){
    ResetearRegion();
    Dividircargas_Interna();
    return ElegirRegion();
}

int ComportamientoJugador::comprobacion2(){
    ResetearRegion();
    Dividircargas();
    return ElegirRegion();
}

Action ComportamientoJugador::MovimientoNoUbicado(Sensores sensores) {
    Action accion;

    // Si hay un muro o precipicio justo delante nos giramos
    if ((sensores.terreno[2] == 'M' or sensores.terreno[2] == 'P') and (region == comprobacion())  and !muro) {
        muro = true;
        tendencia = true;
        cout << "Muro activado" << endl;
        Action retornar = accion = rand()%2 == 0 ? actTURN_L : actTURN_R;
        if(retornar == actTURN_L){
            actL = true;
            actR = false;
        }else{
            actR = true;
            actL = false;
        }
        return retornar;
    }

    if (!tendencia) {
        VUELTA2:
        ResetearRegion();
        Dividircargas_Interna();
        if (obstaculo) {
            carga_del_grid[region].second = 0;
            obstaculo = false;
        }
        int region = ElegirRegion();
        tendencia = true;
        cout << endl
             << region << endl;
        cout << endl
             << carga_del_grid[0].second;
        cout << endl
             << carga_del_grid[1].second;
        cout << endl
             << carga_del_grid[2].second;
        cout << endl
             << carga_del_grid[3].second;
        brujula = brujula_interna;
        if (region != brujula)
            Girar(region);
    } else {
        if(muro){
            if((sensores.terreno[1] == 'M' and sensores.terreno[5] != 'M' and sensores.terreno[11] == 'M')
            or (sensores.terreno[1] == 'P' and sensores.terreno[5] != 'P' and sensores.terreno[11] == 'P')){
                actionconprob.push_back(actFORWARD);
                actionconprob.push_back(actTURN_L);
                actionconprob.push_back(actFORWARD);
                actionconprob.push_back(actFORWARD);
                muro = false;
                tendencia = false;
                obstaculo = false;
                noexplorar = true;
                cout << "Caso: " << 1 << endl;
                return actFORWARD;
            }else if((sensores.terreno[3] == 'M' and sensores.terreno[7] != 'M' and sensores.terreno[13] == 'M')
            or (sensores.terreno[3] == 'P' and sensores.terreno[7] != 'P' and sensores.terreno[13] == 'P')){
                actionconprob.push_back(actFORWARD);
                actionconprob.push_back(actTURN_R);
                actionconprob.push_back(actFORWARD);
                actionconprob.push_back(actFORWARD);
                muro = false;
                tendencia = false;
                obstaculo = false;
                noexplorar = true;
                cout << "Caso: " << 2 << endl;
                return actFORWARD;
            }else if(sensores.terreno[2] == 'M' or sensores.terreno[2] == 'P' 
            or (sensores.terreno[2] == 'B' and !tengo_zapatillas) or (sensores.terreno[2] == 'A' and !tengo_bikini)){
                cout << "Caso: " << 3 << endl;
                if(acabadeaparecer and (sensores.terreno[2] == 'B' or sensores.terreno[2] == 'A'))
                    return actFORWARD;
                if(actL){
                    return actTURN_L;
                }else{
                    return actTURN_R;
                }
            }else{
                cout << "Caso: " << 4 << endl;
                return actFORWARD;
            }
        }else{
            if (sensores.terreno[2] == 'T' or sensores.terreno[2] == 'S' or sensores.terreno[2] == 'G' or
            sensores.terreno[2] == 'K' or sensores.terreno[2] == 'X' or sensores.terreno[2] == 'D' or
            (sensores.terreno[2] == 'A' and tengo_bikini) or (sensores.terreno[2] == 'B' and tengo_zapatillas)) {
                accion = actFORWARD;
                if(!noexplorar){
                    explorado = true;
                    switch (brujula) {
                        case 0:
                            for (int i = -3; i <= 3; i++) {
                                if (mapa_no_posicionado[fil_interna - 4][col_interna + i] == '?')
                                    explorado = false;
                            }
                            break;
                        case 2:
                            for (int i = -3; i <= 3; i++) {
                                if (mapa_no_posicionado[fil_interna + 4][col_interna + i] == '?')
                                    explorado = false;
                            }
                            break;
                        case 1:
                            for (int i = -3; i <= 3; i++) {
                                if (mapa_no_posicionado[fil_interna + i][col_interna + 4] == '?')
                                    explorado = false;
                            }
                            break;
                        case 3:
                            for (int i = -3; i <= 3; i++) {
                                if (mapa_no_posicionado[fil_interna + i][col_interna - 4] == '?')
                                    explorado = false;
                            }
                            break;
                    }
                }
                cout << endl
                    << "Explorado" << explorado
                ? "Si"
                : "No";
                if (explorado and !noexplorar) {
                    tendencia = false;
                    goto VUELTA2;
                }
            } else {
                if(sensores.terreno[0] == 'A' and !tengo_bikini and acabadeaparecer)
                    accion = actFORWARD;
                else if(sensores.terreno[0] == 'B' and !tengo_zapatillas and acabadeaparecer)
                    accion = actFORWARD;
                else{
                    tendencia = false;
                    obstaculo = true;
                    region = brujula;
                    acaba_de_recargar = false;
                    noexplorar = false;
                    goto VUELTA2;
                }
            }
        }
        
    }

    return accion;
}

Action ComportamientoJugador::MovimientoUbicado(Sensores sensores) {
    Action accion;

    // Si hay un muro o precipicio justo delante nos giramos
    if ((sensores.terreno[2] == 'M' or sensores.terreno[2] == 'P') and (region == comprobacion2())  and !muro) {
        muro = true;
        tendencia = true;
        cout << "Muro activado" << endl;
        Action retornar = accion = rand()%2 == 0 ? actTURN_L : actTURN_R;
        if(retornar == actTURN_L){
            actL = true;
            actR = false;
        }else{
            actR = true;
            actL = false;
        }
        return retornar;
    }

    if (!tendencia) {
        VUELTA:
        ResetearRegion();
        Dividircargas();
        if (obstaculo) {
            // if(sensores.terreno[2] == 'M' or sensores.terreno[2] == 'P'){
            // 	int region_ant = region;
            // 	if(region == region_ant){
            // 		siguiendo_muro = true;
            // 		accion = actTURN_L;
            // 		SeguirMuro(sensores);
            // 	}
            // }else{
            // 	carga_del_grid[region].second = 0;
            // 	obstaculo = false;
            // }
            reset_regiones[region]++;
            for (int i = 0; i < 4; i++) {
                if (reset_regiones[i] == 1)
                    carga_del_grid[i].second = 0;
            }
            obstaculo = false;
        }
        int region = ElegirRegion();
        tendencia = true;
        cout << endl
             << region << endl;
        cout << endl
             << carga_del_grid[0].second;
        cout << endl
             << carga_del_grid[1].second;
        cout << endl
             << carga_del_grid[2].second;
        cout << endl
             << carga_del_grid[3].second;
        if (region != brujula)
            Girar(region);
    } else {
        if(muro){
            if((sensores.terreno[1] == 'M' and sensores.terreno[5] != 'M' and sensores.terreno[11] == 'M')
            or (sensores.terreno[1] == 'P' and sensores.terreno[5] != 'P' and sensores.terreno[11] == 'P')){
                actionconprob.push_back(actFORWARD);
                actionconprob.push_back(actTURN_L);
                actionconprob.push_back(actFORWARD);
                actionconprob.push_back(actFORWARD);
                muro = false;
                tendencia = false;
                obstaculo = false;
                noexplorar = true;
                cout << "Caso: " << 1 << endl;
                return actFORWARD;
            }else if((sensores.terreno[3] == 'M' and sensores.terreno[7] != 'M' and sensores.terreno[13] == 'M')
            or (sensores.terreno[3] == 'P' and sensores.terreno[7] != 'P' and sensores.terreno[13] == 'P')){
                actionconprob.push_back(actFORWARD);
                actionconprob.push_back(actTURN_R);
                actionconprob.push_back(actFORWARD);
                actionconprob.push_back(actFORWARD);
                muro = false;
                tendencia = false;
                obstaculo = false;
                noexplorar = true;
                cout << "Caso: " << 2 << endl;
                return actFORWARD;
            }else if(sensores.terreno[2] == 'M' or sensores.terreno[2] == 'P' 
            or (sensores.terreno[2] == 'B' and !tengo_zapatillas) or (sensores.terreno[2] == 'A' and !tengo_bikini)){
                cout << "Caso: " << 3 << endl;
                if(acabadeaparecer and (sensores.terreno[2] == 'B' or sensores.terreno[2] == 'A'))
                    return actFORWARD;
                if(actL){
                    return actTURN_L;
                }else{
                    return actTURN_R;
                }
            }else{
                cout << "Caso: " << 4 << endl;
                return actFORWARD;
            }
        }else{
            if (sensores.terreno[2] == 'T' or sensores.terreno[2] == 'S' or sensores.terreno[2] == 'G' or
            sensores.terreno[2] == 'K' or sensores.terreno[2] == 'X' or sensores.terreno[2] == 'D' or
            (sensores.terreno[2] == 'A' and tengo_bikini) or (sensores.terreno[2] == 'B' and tengo_zapatillas)) {
                accion = actFORWARD;
                if(!noexplorar){
                    explorado = true;
                    switch (brujula) {
                    case 0:
                        for (int i = -3; i <= 3; i++) {
                            if (matriz_grid[fil - 4][col + i] == 0)
                                explorado = false;
                        }
                        break;
                    case 2:
                        for (int i = -3; i <= 3; i++) {
                            if (matriz_grid[fil + 4][col + i] == 0)
                                explorado = false;
                        }
                        break;
                    case 1:
                        for (int i = -3; i <= 3; i++) {
                            if (matriz_grid[fil + i][col + 4] == 0)
                                explorado = false;
                        }
                        break;
                    case 3:
                        for (int i = -3; i <= 3; i++) {
                            if (matriz_grid[fil + i][col - 4] == 0)
                                explorado = false;
                        }
                        break;
                    }
                }
                cout << endl
                    << "Explorado" << explorado
                ? "Si"
                : "No";
                if (explorado and !noexplorar) {
                    tendencia = false;
                    goto VUELTA;
                }
            } else{
                if(sensores.terreno[0] == 'A' and !tengo_bikini and acabadeaparecer)
                    accion = actFORWARD;
                else if(sensores.terreno[0] == 'B' and !tengo_zapatillas and acabadeaparecer)
                    accion = actFORWARD;
                else{
                    tendencia = false;
                    obstaculo = true;
                    region = brujula;
                    acaba_de_recargar = false;
                    noexplorar = false;
                    goto VUELTA;
                }
            }
        }
        
    }

    return accion;
}

unsigned char ComportamientoJugador::suponer(int fil, int col){
    int nodeterminado = 0;
    int casillas[5] = {0,0,0,0,0};
    int pos = 0;
    int max = 0;
    unsigned char tipo[5] = {'S','P','A','B','T'};

    // Si está fuera del límite del mapa
    if(fil < 3 or col < 3 or fil > mapaResultado.size() - 3 or col > mapaResultado.size() - 3)
        return 'P';

    for(int i=fil-1; i<=fil+1;++i){
        for(int j=col-1; j<=col+1;++j){
            switch(mapaResultado[i][j]){
                case 'S':
                    casillas[0]++;
                break;
                case 'P':
                    casillas[1]++;
                break;
                case 'A':
                    casillas[2]++;
                break;
                case 'B':
                    casillas[3]++;
                break;
                case 'T':
                    casillas[4]++;
                break;
                case '?':
                    nodeterminado++;
                break;
            }
        }
    }

    if(nodeterminado == 8)
        return '?';
    else{
        for(int i=0;i<5;i++){
            if(max<casillas[i]){
                pos = i;
                max = casillas[i];
            }
        }
        return tipo[pos];
    }
}

void ComportamientoJugador::InferirCasilla(){
    char caracter;
    bool acabar = false;

    while(!acabar){
        acabar = true;
        for(int i=0;i<mapaResultado.size();++i){
            for(int j=0;j<mapaResultado.size();++j){
                if(mapaResultado[i][j] == '?'){
                    caracter = suponer(i,j);
                    if(caracter != '?')
                        mapaResultado[i][j] = caracter;
                    else
                        acabar = false;
                }
            }
        }
    }
}
