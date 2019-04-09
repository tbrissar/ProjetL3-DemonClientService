#!/bin/bash

cc="gcc -I. -I./MYASSERT -Wall -Wextra -Wconversion -pedantic -std=c99 -fPIC -g"
commun="./MYASSERT/myassert.c ./lireecrire.c ./config.c ./fichiers.c"

$cc -o ./CONFIG/test_config ./CONFIG/test_config.c $commun	
