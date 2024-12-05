#!/bin/bash

# Loop de 0 a 49 para os IDs dos heróis
for id in $(seq 0 49); do
	echo "RESULTADOS HEROI $id"
    if [ $id -lt 10 ]; then
        # Dois espaços para IDs com 1 dígito
        ./teste | grep "HEROI  $id"
    else
        # Um espaço para IDs com 2 dígitos
        ./teste | grep "HEROI $id"
    fi
	echo
done

