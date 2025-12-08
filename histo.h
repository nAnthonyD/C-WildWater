#ifndef HISTO_H
#define HISTO_H

#include "core.h"

/*
 * Modes d'histogramme :
 *   H_MAX  : capacité maximale
 *   H_SRC  : volume total capté
 *   H_REAL : volume réellement traité
 */
typedef enum {
    H_MAX,
    H_SRC,
    H_REAL
} HistoMode;

/*
 * Fonction principale pour générer les fichiers .dat des histogrammes.
 *
 * - csv_path        : chemin du fichier CSV
 * - mode            : H_MAX / H_SRC / H_REAL
 * - output_dat_path : fichier .dat de sortie
 *
 * Retour :
 *   0  → OK
 *   >0 → erreur
 */
int run_histo(const char *csv_path,
              HistoMode mode,
              const char *output_dat_path);

#endif /* HISTO_H */

