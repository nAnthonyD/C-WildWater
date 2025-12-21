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

# Compilation si nécessaire
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

# Nettoyage et création des dossiers temporaires
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
        
        # Tri numérique pour extraire les données selon la consigne v1.1
        sort -t";" -k2n data.csv > tmp/data_sorted.tmp

        # Extraction : 50 plus petites et 10 plus grandes stations
        head -n 50 tmp/data_sorted.tmp > tmp/data_min.dat
        tail -n 10 tmp/data_sorted.tmp > tmp/data_max.dat

        gnuplot << EOF
        set terminal png size 1000,600
        set datafile separator ";"
        set style data histograms
        set style fill solid 1.0 border -1
        set ylabel "Volume (m3)"
        set xlabel "Station"
        set xtics rotate by -45 font ",8"
        set grid y
        set boxwidth 0.7

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

    if [ ! -f "leaks.csv" ]; then
        echo "Erreur: leaks.csv non généré."
        exit 3
    fi

    cp leaks.csv "tests/leaks.csv"
    echo "Historique leaks exporté dans tests/leaks.csv"
    ;;


    *)
        echo "Erreur : Action '$ACTION' inconnue."
        help_me
        ;;
esac

FIN=$(date +%s)
DUREE=$((FIN - DEBUT))
# Conversion en millisecondes pour respecter la consigne v1.1
echo "Durée totale : $((DUREE * 1000)) millisecondes."
exit 0
