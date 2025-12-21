#include "histo.h"

// Prints usage information for the program
void print_usage(const char* prog) {
    fprintf(stderr,
        "Usage: %s <csv_file> <action> <parameter>\n"
        "Actions:\n"
        "  histo <max|src|real>\n"
        "  leaks <STATION_ID>\n"
        "  -h\n", prog);
}

// Parses histogram mode from string
int parse_histo_mode(const char* s, HistoMode* out_mode) {
    if (strcmp(s, "max") == 0) { *out_mode = H_MAX; return 0; }
    if (strcmp(s, "src") == 0) { *out_mode = H_SRC; return 0; }
    if (strcmp(s, "real") == 0) { *out_mode = H_REAL; return 0; }
    if (strcmp(s, "all") == 0) { *out_mode = H_ALL; return 0; }
    return 1;
}

// Main entry point
int main(int argc, char** argv) {
    if (argc >= 2 && strcmp(argv[1], "-h") == 0) {
        print_usage(argv[0]);
        return 0;
    }
    if (argc < 4) {
        fprintf(stderr, "Error: insufficient number of arguments.\n");
        print_usage(argv[0]);
        return 1;
    }

    const char* csv_path = argv[1];
    const char* action = argv[2];
    const char* param = argv[3];

    if (strcmp(action, "histo") == 0) {
        HistoMode mode;
        if (parse_histo_mode(param, &mode) != 0) {
            fprintf(stderr, "Error: invalid histo parameter (max, src, real).\n");
            return 2;
        }

        const char* out = "data.csv";
        int err = run_histo(csv_path, mode, out);
        if (err != 0) {
            fprintf(stderr, "Error: run_histo failed (code %d)\n", err);
            return 3;
        }
        return 0;
    }

    if (strcmp(action, "leaks") == 0) {
        FILE* f = fopen(csv_path, "r");
        if (!f) {
            fprintf(stderr, "Error: unable to open '%s'\n", csv_path);
            return 10;
        }
        char biggestleakageid[40];
        char biggestleak_parent[40];
        float biggestleakage;
        printf("Calculating leaks for plant ID: %s\n", param);
        Avl_Plant* plants = getAllPlantsFromFile(f);

       
        getPlantColAndProcVolume(f, plants);

        
        float lost = leakage((char*)param, f, plants, biggestleakageid, biggestleak_parent, &biggestleakage);
        freeAvlPlant(plants);

        fclose(f);

       
        const char* leaks_file = "leaks.csv";

       
        FILE* test = fopen(leaks_file, "r");
        int exists = (test != NULL);
        if (test) fclose(test);

        FILE* out = fopen(leaks_file, "a");
        if (!out) {
            fprintf(stderr, "Error: unable to create/open %s\n", leaks_file);
            return 11;
        }

        if (!exists) {
            fprintf(out, "Identifier;Leak volume (M.m3./year);Biggest Leak Section;Section Parent;Leak In Section (M.m3/year)\n");
        }

        fprintf(out, "%s;%.6f;%s;%s;%.6f,\n", param, lost,biggestleakageid,biggestleak_parent,biggestleakage);
        fclose(out);
        printf("LEAKS %s = %.6f (M.m3)\n", param, lost);

        

        return 0;
    }
}
