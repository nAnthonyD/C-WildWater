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

DEBUT=$(date +%s%3N)

case $ACTION in
    "histo")
    if [[ "$PARAM" != "max" && "$PARAM" != "src" && "$PARAM" != "real" && "$PARAM" != "all" ]]; then
        echo "Erreur : Paramètre invalide (max, src, real, all)."
        help_me
    fi

    echo "Traitement histogramme : $PARAM"
    ./"$EXECUTABLE" "$FICHIER" "$ACTION" "$PARAM"

    if [ $? -ne 0 ] || [ ! -f "data.csv" ]; then
        echo "Erreur : Échec du programme C ou fichier de sortie manquant."
        exit 2
    fi

    # --- Renommage du fichier de sortie selon le mode ---
    case "$PARAM" in
        max)  OUTCSV="data_max_cap.csv" ;;
        src)  OUTCSV="data_col_vol.csv" ;;
        real) OUTCSV="data_pro_vol.csv" ;;
        all)  OUTCSV="data_all.csv" ;;
    esac

    mv -f data.csv "$OUTCSV"

    # --- Tri + extraction min/max ---
    if [ "$PARAM" = "all" ]; then
        # on trie sur la colonne 2 (max_cap) -> adapte si tu préfères trier sur real (col 4)
        sort -t";" -k2n "$OUTCSV" > tmp/data_sorted.tmp

        head -n 50 tmp/data_sorted.tmp > tmp/data_min.dat
        tail -n 10 tmp/data_sorted.tmp > tmp/data_max.dat

        
        gnuplot << EOF
        set terminal png size 1200,600 font "arial,10"
        set datafile separator ";"
        set style data histograms
        set style histogram rowstacked
        set style fill solid 1.0 border -1
        set ylabel "Volume (M.m3)"
        set xlabel "Station"
        set xtics rotate by -45
        set grid y
        set boxwidth 0.4
        set key outside

        set output 'tests/graph_${PARAM}_min.png'
        set title 'Les 50 plus petites stations (all)'
        plot 'tmp/data_min.dat' using 2:xtic(1) title 'max - source' lc rgb "green", \
             '' using 3 title 'source - real' lc rgb "red", \
             '' using 4 title 'real' lc rgb "blue"

        set output 'tests/graph_${PARAM}_max.png'
        set title 'Les 10 plus grandes stations (all)'
        plot 'tmp/data_max.dat' using 2:xtic(1) title 'max - source' lc rgb "green", \
             '' using 3 title 'source - real' lc rgb "red", \
             '' using 4 title 'real' lc rgb "blue"
EOF

    else
        # cas simple (2 colonnes)
        sort -t";" -k2n "$OUTCSV" > tmp/data_sorted.tmp

        head -n 50 tmp/data_sorted.tmp > tmp/data_min.dat
        tail -n 10 tmp/data_sorted.tmp > tmp/data_max.dat

        gnuplot << EOF
        set terminal png size 1200,600 font "arial,10"
        set datafile separator ";"
        set style data histograms
        set style fill solid 1.0 border -1
        set ylabel "Volume (M.m3)"
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
    fi

    echo "CSV généré : $OUTCSV"
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

FIN=$(date +%s%3N)
DUREE=$((FIN - DEBUT))
echo "Durée totale : $((DUREE)) millisecondes."
exit 0
