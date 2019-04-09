#!/bin/bash

# supprimer les 6 lignes ci-dessous pour pouvoir utiliser le script
#echo Ce programme efface tous les sémaphore ayant des droits précis
#echo cf. commande \"ipcs -s\"
#echo C\'est très dangereux et à utiliser à vos risques et périls
#echo Le concepteur décline toute responsabilité quant aux dégâts résultants
#echo Supprimez les lignes de code adéquates pour utiliser ce script


DROITS=641
nb=`ipcs -s | grep $USER | grep $DROITS | wc -l`

if [ $nb -eq 0 ]
then
    echo "aucun sémaphore à détruire"
else
    pluriel=""
    if [ $nb -gt 1 ]; then pluriel="s"; fi
    echo "vous avez $nb sémaphore$pluriel non détruit$pluriel";

    for id in `ipcs -s | grep $USER | grep $DROITS | awk '{print $2;}'`
    do
        echo "  destruction sématphore " $id
        ipcrm -s $id
    done

    nb=`ipcs -s | grep $USER | grep $DROITS | wc -l`
    pluriel=""
    if [ $nb -gt 1 ]; then pluriel="s"; fi
    echo "il reste $nb sémaphore$pluriel non détruit$pluriel";
fi

rm cvd
rm d2c
