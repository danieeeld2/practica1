#!/bin/bash

EXECUTABLE="./practica1SG"
MAPDIR="./mapas"

# Make the target
make > /dev/null
echo -e "\e[0;34mCompilando..."

if [[ ! -e $EXECUTABLE ]]; then
    echo -e "\e[1;31mNo se ha encontrado el ejecutable"
    exit 1
fi

# Run all maps
if [[ ! -d  $MAPDIR ]]; then
    echo -e "\e[1;31mNo se han encontrado mapas"
    exit 1
fi

for map in ./mapas/*; do
    echo -e "\e[0;34mRunning ${map}"
    for i in {0..4}; do
        echo -e "Nivel de dificultad ${i}"
        echo "${map} 1 ${i} 6 8 0" | xargs ./practica1SG | grep "Porcentaje" | awk '{print $5}'
    done
done
