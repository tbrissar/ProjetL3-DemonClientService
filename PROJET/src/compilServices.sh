#!/bin/bash

# services
# ========
#cc="gcc -I. -I./MYASSERT -Wall -Wextra -Wno-unused-parameter -Wconversion -pedantic -std=c99 -fPIC -g -pthread"
cc="gcc -I. -I./MYASSERT -Wall -Wextra -pedantic -std=c99 -fPIC -g -pthread"
commun="./MYASSERT/myassert.c ./lireecrire.c ./utils_services.c"
src="somme miroir compression binarisation"

for f in $src
do
    $cc -o ./SERVICES/$f ./SERVICES/$f.c $commun
done

