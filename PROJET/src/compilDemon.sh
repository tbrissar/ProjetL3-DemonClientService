#!/bin/bash
clear
#cc="gcc -I./MYASSERT -Wall -Wextra -Wno-unused-parameter -pedantic -std=c99 -fPIC -g"
cc="gcc -I./MYASSERT -Wall -Wextra -Wconversion -pedantic -std=c99 -fPIC -g"
commun="MYASSERT/myassert.c config.c fichiers.c ./lireecrire.c"

$cc -o demon demon.c utils_demon.c $commun
