#!/bin/bash

# clients
# =======
#cc="gcc -I. -I./MYASSERT -I./CLIENTS -Wall -Wextra -Wconversion -pedantic -std=c99 -fPIC -g"
cc="gcc -I. -I./MYASSERT -I./CLIENTS -Wall -Wextra -pedantic -std=c99 -fPIC -g"
commun="./MYASSERT/myassert.c ./lireecrire.c ./utils_demon.c ./utils_services.c ./IMAGES/Image.c"
src="client_exit clientSomme clientCompression clientMiroir clientBinarisation"

for f in $src
do
    $cc -o ./CLIENTS/$f ./CLIENTS/$f.c $commun
done
