#ifndef HISTO_H
#define HISTO_H


typedef enum {
    H_MAX,
    H_SRC,
    H_REAL
} HistoMode;


int run_histo(const char *csv_path,
              HistoMode mode,
              const char *output_dat_path);

#endif /* HISTO_H */

