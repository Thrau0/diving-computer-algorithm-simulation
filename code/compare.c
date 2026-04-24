/**
 * compare.c -- side-by-side Bühlmann ZHL-16C vs RGBM comparison
 *
 * Runs both decompression algorithms in lockstep through every dive
 * profile defined in profile.h and writes one .dat file per profile:
 *
 *   # <profile name>
 *   # time  depth  ceil_bhl  ceil_rgbm  lambda_rgbm
 *   0.0   0.0   0.0   0.0   1.000
 *   ...
 *   # BHL:  total=XX.X min, final_ceil=X.X m
 *   # RGBM: total=XX.X min, final_ceil=X.X m, final_lambda=X.XXX
 *
 * Compile: gcc -O2 -Wall -o compare compare.c -lm
 * Run:     ./compare
 */

#include <stdio.h>
#include <math.h>

#include "profile.h"
#include "buehlmann.h"
#include "rgbm.h"

/* ------------------------------------------------------------------ */
/* Simulate one profile, write one .dat file                          */
/* ------------------------------------------------------------------ */

static void simulate(const Profile *prof) {

    /* open output file */
    char path[256];
    snprintf(path, sizeof(path), "%s.dat", prof->filename);
    FILE *f = fopen(path, "w");
    if (!f) {
        fprintf(stderr, "ERROR: could not open '%s'\n", path);
        return;
    }

    /* header */
    fprintf(f, "# %s\n", prof->name);
    fprintf(f, "# %-8s  %-7s  %-9s  %-9s  %s\n",
            "time", "depth", "ceil_bhl", "ceil_rgbm", "lambda_rgbm");

    /* initialise both algorithm states */
    BuehlmannState bhl;
    RgbmState      rgb;
    bhl_init(&bhl);
    rgbm_init(&rgb);

    double time = 0.0;
    double dt   = 1.0 / 60.0;   /* 1-second steps expressed in minutes */

    for (int s = 0; s < prof->n; s++) {
        const Segment *seg   = &prof->segments[s];
        int            steps = (int)(seg->duration_min * 60.0);

        for (int step = 0; step < steps; step++) {
            double frac  = (double)step / steps;
            double depth = seg->start_m + frac * (seg->end_m - seg->start_m);

            /* advance both algorithms by one second */
            bhl_update(&bhl, depth, dt);
            rgbm_update(&rgb, depth, dt);

            /* print one row per minute */
            if (step % 60 == 0) {
                double t         = time + step / 60.0;
                double ceil_bhl  = bhl_ceiling(&bhl);
                double ceil_rgbm = rgbm_ceiling(&rgb);
                double lambda    = rgbm_lambda(&rgb);

                fprintf(f, "  %-8.1f  %-7.1f  %-9.1f  %-9.1f  %.3f\n",
                        t, depth, ceil_bhl, ceil_rgbm, lambda);
                printf(    "  %-8.1f  %-7.1f  %-9.1f  %-9.1f  %.3f\n",
                        t, depth, ceil_bhl, ceil_rgbm, lambda);
            }
        }

        time += seg->duration_min;
    }

    /* summary footer */
    double final_bhl  = bhl_ceiling(&bhl);
    double final_rgbm = rgbm_ceiling(&rgb);
    double final_lam  = rgbm_lambda(&rgb);

    fprintf(f, "# BHL:  total=%.1f min, final_ceil=%.1f m\n",
            time, final_bhl);
    fprintf(f, "# RGBM: total=%.1f min, final_ceil=%.1f m, final_lambda=%.3f\n",
            time, final_rgbm, final_lam);

    printf("# BHL:  total=%.1f min, final_ceil=%.1f m\n", time, final_bhl);
    printf("# RGBM: total=%.1f min, final_ceil=%.1f m, final_lambda=%.3f\n\n",
           time, final_rgbm, final_lam);

    fclose(f);
    printf(" → wrote %s\n\n", path);
}

/* ------------------------------------------------------------------ */

int main(void) {
    for (int p = 0; p < N_PROFILES; p++) {
        printf("=== %s ===\n", PROFILES[p].name);
        simulate(&PROFILES[p]);
    }
    return 0;
}