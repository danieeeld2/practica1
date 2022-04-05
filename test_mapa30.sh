#!/bin/bash
#Author: Nasr El Farissi

mkdir -p resultados

rm resultados/porcentajes30.txt
rm resultados/soloporcentajes30.txt
rm resultados/sumatotal30.txt

for i in {3..26}
    do 
        for j in {3..26}
            do
                echo "$i $j" >> resultados/porcentajes30.txt
                ./practica1SG mapas/mapa30.map 0 2 $i $j 0 |  grep "Porcentaje de mapa descubierto:" >> resultados/porcentajes30.txt
            done
    done


grep -oP '(?<=Porcentaje de mapa descubierto: )[0-9]+'.'*[0-9]*' resultados/porcentajes30.txt > resultados/soloporcentajes30.txt

awk '{sum += $1 } END { print sum }' resultados/soloporcentajes30.txt > resultados/sumatotal30.txt

var1=$(wc -l < resultados/soloporcentajes30.txt)
var2=$(cat resultados/sumatotal30.txt)
echo "scale=2 ; $var2 / $var1" | bc

