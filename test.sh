#!/bin/bash
./practica1SG ./mapas/cuadricula.map 1 1 84 39 3 | grep Porcentaje > ./Resultados/porcentajes.dat
./practica1SG ./mapas/medieval.map 1 1 84 39 3 | grep Porcentaje >> ./Resultados/porcentajes.dat
./practica1SG ./mapas/mapa100.map 1 1 84 39 3 | grep Porcentaje >> ./Resultados/porcentajes.dat
./practica1SG ./mapas/picto.map 1 1 68 14 1 | grep Porcentaje >> ./Resultados/porcentajes.dat
./practica1SG ./mapas/mapa50.map 1 1 42 19 3| grep Porcentaje >> ./Resultados/porcentajes.dat
./practica1SG ./mapas/vertigo.map 1 1 78 79 3 | grep Porcentaje >> ./Resultados/porcentajes.dat
./practica1SG ./mapas/mapa30.map 1 1 25 11 3 | grep Porcentaje >> ./Resultados/porcentajes.dat
./practica1SG ./mapas/mapa75.map 1 1 63 23 3 | grep Porcentaje >> ./Resultados/porcentajes.dat

