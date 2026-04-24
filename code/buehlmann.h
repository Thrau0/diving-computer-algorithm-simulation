#ifndef BUEHLMANN_H
#define BUEHLMANN_H

/**
 * buehlmann.h -- Bühlmann ZHL-16C decompression algorithm
 *
 * The body is modelled as 16 independent tissue compartments, each
 * tracking dissolved nitrogen. During ascent the algorithm checks
 * whether any compartment is too loaded for the current depth; if so
 * a decompression stop is required.
 *
 * All functions are static inline so this header is self-contained —
 * no separate .c translation unit is needed.
 */

#include <math.h>
#include "profile.h"

/* ------------------------------------------------------------------ */
/* State                                                              */
/* ------------------------------------------------------------------ */

typedef struct {
    double tissue[N_COMPARTMENTS];
} BuehlmannState;

/* ------------------------------------------------------------------ */
/* Initialise all compartments to surface equilibrium                 */
/* ------------------------------------------------------------------ */

static inline void bhl_init(BuehlmannState *s) {
    for (int i = 0; i < N_COMPARTMENTS; i++)
        s->tissue[i] = p_inspired_n2(0.0);
}

/* ------------------------------------------------------------------ */
/* Advance all compartments by dt minutes at the given depth          */
/*                                                                    */
/* Haldane equation:                                                  */
/*   P_new = P + (Pi - P) * (1 - exp(-ln2/ht * dt))                  */
/* ------------------------------------------------------------------ */

static inline void bhl_update(BuehlmannState *s, double depth, double dt) {
    double pi = p_inspired_n2(depth);
    for (int i = 0; i < N_COMPARTMENTS; i++) {
        double k = log(2.0) / COMP[i].ht;
        s->tissue[i] += (pi - s->tissue[i]) * (1.0 - exp(-k * dt));
    }
}

/* ------------------------------------------------------------------ */
/* Compute decompression ceiling (metres)                             */
/*                                                                    */
/* For each compartment the minimum ambient pressure that keeps the   */
/* tissue within its M-value tolerance is:                            */
/*   p_tol = (tissue - a) * b                                         */
/* The ceiling is the depth matching the highest such pressure.       */
/* ------------------------------------------------------------------ */

static inline double bhl_ceiling(const BuehlmannState *s) {
    double p_ceil = 0.0;
    for (int i = 0; i < N_COMPARTMENTS; i++) {
        double p_tol = (s->tissue[i] - COMP[i].a) * COMP[i].b;
        if (p_tol > p_ceil) p_ceil = p_tol;
    }
    double depth = (p_ceil - P_SURFACE) / BAR_PER_METRE;
    return depth > 0.0 ? depth : 0.0;
}

#endif