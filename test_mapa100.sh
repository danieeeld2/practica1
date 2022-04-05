#!/bin/bash
#Author: Nasr El Farissi

mkdir -p resultados

rm resultados/porcentajes100.txt
rm resultados/soloporcentajes100.txt
rm resultados/sumatotal100.txt

for i in {3..96}
    do 
        for j in {3..96}
            do
                echo "$i $j" >> resultados/porcentajes100.txt
                ./practica1SG mapas/mapa100.map 0 2 $i $j 0 |  grep "Porcentaje de mapa descubierto:" >> resultados/porcentajes100.txt
            done
    done


grep -oP '(?<=Porcentaje de mapa descubierto: )[0-9]+'.'*[0-9]*' resultados/porcentajes100.txt > resultados/soloporcentajes100.txt

awk '{sum += $1 } END { print sum }' resultados/soloporcentajes100.txt > resultados/sumatotal100.txt

var1=$(wc -l < resultados/soloporcentajes100.txt)
var2=$(cat resultados/sumatotal100.txt)
echo "scale=2 ; $var2 / $var1" | bc

