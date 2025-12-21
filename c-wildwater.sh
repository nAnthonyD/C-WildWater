#!/bin/bash

function help_me {
    echo "Usage: $0 [fichier_csv] [action] [parametre]"
    echo "Actions :"
    echo "  histo : Statistiques (paramètres : max, src, real)"
    echo "  leaks : Détection fuites (paramètre : Identifiant station)"
    echo "  -h    : Affiche cette aide"
    exit 0
}

if [[ "$*" == *"-h"* ]]; then
    help_me
fi

if [ "$#" -lt 3 ]; then
    echo "Erreur : Nombre d'arguments insuffisant."
    help_me
fi

FICHIER=$1
ACTION=$2
PARAM=$3
EXECUTABLE="c-wildwater"

if [ ! -f "$FICHIER" ]; then
    echo "Erreur : Le fichier '$FICHIER' n'existe pas."
    exit 1
fi

if [ -f "Makefile" ]; then
    if [ ! -x "$EXECUTABLE" ]; then
        echo "Compilation en cours..."
        make clean > /dev/null 2>&1
        make
        if [ $? -ne 0 ]; then
            echo "Erreur : La compilation a échoué."
            exit 1
        fi
    fi
else
    echo "Erreur : Makefile introuvable."
    exit 1
fi

rm -rf tmp && mkdir tmp
[ ! -d "tests" ] && mkdir tests

DEBUT=$(date +%s)

case $ACTION in
    "histo")
        if [[ "$PARAM" != "max" && "$PARAM" != "src" && "$PARAM" != "real" ]]; then
            echo "Erreur : Paramètre invalide (max, src, real)."
            help_me
        fi

        echo "Traitement histogramme : $PARAM"
        ./"$EXECUTABLE" "$FICHIER" "$ACTION" "$PARAM"

        if [ $? -ne 0 ] || [ ! -f "data.csv" ]; then
            echo "Erreur : Échec du programme C ou fichier de sortie manquant."
            exit 2
        fi
        
        sort -t";" -k2n data.csv > tmp/data_sorted.tmp

        head -n 50 tmp/data_sorted.tmp > tmp/data_min.dat
        tail -n 10 tmp/data_sorted.tmp > tmp/data_max.dat

        # --- Modifications Gnuplot ---
        # 1. size 1200,600 : On élargit l'image pour que les 50 barres soient moins serrées
        # 2. font "arial,10" : On réduit la police (standard est souvent 12)
        # 3. boxwidth 0.4 : On réduit la largeur des barres pour espacer les éléments
        gnuplot << EOF
        set terminal png size 1200,600 font "arial,10"
        set datafile separator ";"
        set style data histograms
        set style fill solid 1.0 border -1
        set ylabel "Volume (m3)"
        set xlabel "Station"
        set xtics rotate by -45
        set grid y
        set boxwidth 0.4
        
        set output 'tests/graph_${PARAM}_min.png'
        set title 'Les 50 plus petites stations ($PARAM)'
        plot 'tmp/data_min.dat' using 2:xtic(1) notitle linecolor rgb "blue"

        set output 'tests/graph_${PARAM}_max.png'
        set title 'Les 10 plus grandes stations ($PARAM)'
        plot 'tmp/data_max.dat' using 2:xtic(1) notitle linecolor rgb "red"
EOF
        echo "Graphiques générés dans 'tests/'."
        ;;

    "leaks")
        if [ -z "$PARAM" ]; then
            echo "Erreur : Identifiant manquant."
            help_me
        fi

        echo "Détection de fuites pour : $PARAM"
        ./"$EXECUTABLE" "$FICHIER" "$ACTION" "$PARAM"

        if [ $? -ne 0 ]; then
            echo "Erreur lors de l'exécution du programme C."
            exit 2
        fi
        echo "Calcul terminé. Résultats enregistrés."
        ;;

    *)
        echo "Erreur : Action '$ACTION' inconnue."
        help_me
        ;;
esac

FIN=$(date +%s)
DUREE=$((FIN - DEBUT))
echo "Durée totale : $((DUREE * 1000)) millisecondes."
exit 0
