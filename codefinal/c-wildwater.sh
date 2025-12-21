#!/bin/bash

function help_me {
    echo "Usage: $0 [csv_file] [action] [parameter]"
    echo "Actions:"
    echo "  histo: Statistics (parameters: max, src, real, all)"
    echo "  leaks: Leak detection (parameter: Station identifier)"
    echo "  -h: Display this help"
    exit 0
}

if [[ "$*" == *"-h"* ]]; then
    help_me
fi

if [ "$#" -lt 3 ]; then
    echo "Error: Insufficient number of arguments."
    help_me
fi

FILE=$1
ACTION=$2
PARAM=$3
EXECUTABLE="c-wildwater"

if [ ! -f "$FILE" ]; then
    echo "Error: The file '$FILE' does not exist."
    exit 1
fi

if [ -f "Makefile" ]; then
    if [ ! -x "$EXECUTABLE" ]; then
        echo "Compilation in progress..."
        make clean > /dev/null 2>&1
        make
        if [ $? -ne 0 ]; then
            echo "Error: Compilation failed."
            exit 1
        fi
    fi
else
    echo "Error: Makefile not found."
    exit 1
fi

rm -rf tmp && mkdir tmp
[ ! -d "tests" ] && mkdir tests

DEBUT=$(date +%s%3N)

case $ACTION in
    "histo")
    if [[ "$PARAM" != "max" && "$PARAM" != "src" && "$PARAM" != "real" && "$PARAM" != "all" ]]; then
        echo "Error: Invalid parameter (max, src, real, all)."
        help_me
    fi

    echo "Processing histogram: $PARAM"
    ./"$EXECUTABLE" "$FILE" "$ACTION" "$PARAM"

    if [ $? -ne 0 ] || [ ! -f "data.csv" ]; then
        echo "Error: C program failure or missing output file."
        exit 2
    fi

    # --- Renaming output file according to mode ---
    case "$PARAM" in
        max)  OUTCSV="data_max_cap.csv" ;;
        src)  OUTCSV="data_col_vol.csv" ;;
        real) OUTCSV="data_pro_vol.csv" ;;
        all)  OUTCSV="data_all.csv" ;;
    esac

    mv -f data.csv "$OUTCSV"

    # --- Sort + extraction min/max ---
    if [ "$PARAM" = "all" ]; then
        # we sort on column 2 (max_cap) -> adapt if you prefer to sort on real (col 4)
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
        # simple condition (2 colonnes)
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

    echo "CSV generated: $OUTCSV"
    echo "Graphs generated in 'tests/'."
    ;;

    "leaks")
        if [ -z "$PARAM" ]; then
            echo "Error: Missing identifier."
            help_me
        fi

        echo "Leak detection for: $PARAM"
        ./"$EXECUTABLE" "$FILE" "$ACTION" "$PARAM"

        if [ $? -ne 0 ]; then
            echo "Error during C program execution."
            exit 2
        fi
        echo "Calculation completed. Results saved."
        ;;

    *)
        echo "Error: Unknown action '$ACTION'."
        help_me
        ;;
esac

FIN=$(date +%s%3N)
DUREE=$((FIN - DEBUT))
echo "Total duration: $((DUREE)) milliseconds."
exit 0
