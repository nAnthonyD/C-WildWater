#ifndef CORE_H
#define CORE_H

#include <stdio.h>

/*
 * Structure d’une usine :
 * - id            : nom de l’usine
 * - capacite_max  : capacité maximale (k.m3/an)
 * - volume_src    : volume total capté avant fuites
 * - volume_real   : volume réellement traité (après fuites)
 */
typedef struct {
    char id[128];
    int  capacite_max;
    long volume_src;
    long volume_real;
} Usine;

/*
 * Noeud d’un AVL stockant les usines.
 */
typedef struct AVL_Usine {
    Usine              data;
    struct AVL_Usine  *left;
    struct AVL_Usine  *right;
    int                height;
} AVL_Usine;

/*========================================================
 * Fonctions AVL
 *========================================================*/

/*
 * Insère une usine dans l'AVL (triée par id).
 * Si l’usine existe déjà → on ne la recrée pas.
 * Retourne la nouvelle racine.
 */
AVL_Usine *avl_usine_insert(AVL_Usine *root, const char *id);

/*
 * Recherche une usine dans l'AVL.
 * Retourne NULL si elle n'existe pas.
 */
Usine *avl_usine_find(AVL_Usine *root, const char *id);

/*
 * Libère complètement l’AVL.
 */
void avl_usine_free(AVL_Usine *root);

/*========================================================
 * Parsing CSV → Remplir l’AVL d’usines
 *========================================================*/

/*
 * Analyse le fichier CSV et remplit l’AVL passé par adresse.
 * Met à jour automatiquement :
 *   - capacite_max
 *   - volume_src
 *   - volume_real (après fuites)
 *
 * Retour :
 *   0 → succès
 *   >0 → erreur
 */
int build_usines_from_csv(const char *csv_path, AVL_Usine **root);

#endif /* CORE_H */

