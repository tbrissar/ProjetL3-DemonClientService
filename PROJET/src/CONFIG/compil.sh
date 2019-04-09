#!/bin/bash
clear
cc="gcc -I.. -I../MYASSERT -Wall -Wextra -Wconversion -pedantic -std=c99 -fPIC -g"
commun="../MYASSERT/myassert.c ../config.c ../fichiers.c"

$cc -o test_config test_config.c $commun	
