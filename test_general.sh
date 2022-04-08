#!/bin/bash
#Author: Inma

mkdir -p prueba

rm prueba/porcentajes.txt

for i in {0..4}
	do
		echo "Mapa 30 nivel " $i >> prueba/porcentajes.txt
		./practica1SG mapas/mapa30_e.map 1 $i 24 4 0 | grep "Porcentaje de mapa descubierto:" >> prueba/porcentajes.txt
		
		
	done
		
for i in {0..4}
	do
		
		echo "Mapa 70 nivel " $i >> prueba/porcentajes.txt
		./practica1SG mapas/mapa70_e.map 1 $i 33 19 0 | grep "Porcentaje de mapa descubierto:" >> prueba/porcentajes.txt
		
	done
		
for i in {0..4}
	do
		
		echo "Mapa vértigo nivel " $i >> prueba/porcentajes.txt
		./practica1SG mapas/vertigo_e.map 1 $i 79 79 0 | grep "Porcentaje de mapa descubierto:" >> prueba/porcentajes.txt
	done
		

