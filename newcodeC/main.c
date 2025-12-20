#include <stdio.h>
#include <string.h>
#include "histo.h"
#include "managefiles.h"
#include "leakage.h"

static void print_usage(const char* prog) {
    fprintf(stderr,
        "Usage: %s <fichier_csv> <action> <parametre>\n"
        "Actions:\n"
        "  histo <max|src|real>\n"
        "  leaks <ID_STATION>\n"
        "  -h\n", prog);
}

static int parse_histo_mode(const char* s, HistoMode* out_mode) {
    if (strcmp(s, "max") == 0) { *out_mode = H_MAX; return 0; }
    if (strcmp(s, "src") == 0) { *out_mode = H_SRC; return 0; }
    if (strcmp(s, "real") == 0) { *out_mode = H_REAL; return 0; }
    return 1;
}

int main(int argc, char** argv) {
    if (argc >= 2 && strcmp(argv[1], "-h") == 0) {
        print_usage(argv[0]);
        return 0;
    }
    if (argc < 4) {
        fprintf(stderr, "Erreur: nombre d'arguments insuffisant.\n");
        print_usage(argv[0]);
        return 1;
    }

    const char* csv_path = argv[1];
    const char* action = argv[2];
    const char* param = argv[3];

    if (strcmp(action, "histo") == 0) {
        HistoMode mode;
        if (parse_histo_mode(param, &mode) != 0) {
            fprintf(stderr, "Erreur: parametre histo invalide (max, src, real).\n");
            return 2;
        }

        const char* out = "data.csv";
        int err = run_histo(csv_path, mode, out);
        if (err != 0) {
            fprintf(stderr, "Erreur: run_histo a echoue (code %d)\n", err);
            return 3;
        }
        return 0;
    }

    if (strcmp(action, "leaks") == 0) {
        FILE* f = fopen(csv_path, "r");
        if (!f) {
            fprintf(stderr, "Erreur: impossible d'ouvrir '%s'\n", csv_path);
            return 10;
        }

        
        Avl_Plant* plants = getAllPlantsFromFile(f);

       
        getPlantColAndProcVolume(f, plants);

        
        float lost = leakage((char*)param, f, plants);

        fclose(f);

       
        const char* leaks_file = "leaks.csv";

       
        FILE* test = fopen(leaks_file, "r");
        int exists = (test != NULL);
        if (test) fclose(test);

        FILE* out = fopen(leaks_file, "a");
        if (!out) {
            fprintf(stderr, "Erreur: impossible de creer/ouvrir %s\n", leaks_file);
            return 11;
        }

        if (!exists) {
            fprintf(out, "Identifier;Leak volume (M.m3.year-1)\n");
        }

        fprintf(out, "%s;%.6f\n", param, lost);
        fclose(out);

        printf("LEAKS %s = %.6f (M.m3)\n", param, lost);

        

        return 0;
    }
}

