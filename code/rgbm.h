#ifndef RGBM_H
#define RGBM_H

/**
 * rgbm.h -- Simplified RGBM decompression algorithm
 *
 * RGBM extends Bühlmann ZHL-16C by tracking bubble growth. Pre-existing
 * gas micronuclei in the body can expand into bubbles during ascent. The
 * more bubbles present, the more conservative the ceiling becomes. This
 * is captured by a reduction factor lambda (0.5 – 1.0):
 *
 *   lambda = 1.0  →  no bubbles, ceiling identical to Bühlmann
 *   lambda < 1.0  →  bubbles present, ceiling is set deeper
 *
 * All functions are static inline so this header is self-contained —
 * no separate .c translation unit is needed.
 */

#include <math.h>
#include "profile.h"

/* ------------------------------------------------------------------ */
/* RGBM tuning constants                                              */
/* ------------------------------------------------------------------ */

#define RGBM_BUBBLE_GROWTH_RATE  0.03
#define RGBM_BUBBLE_DECAY_RATE   0.008
#define RGBM_LAMBDA_MIN          0.50

/* ------------------------------------------------------------------ */
/* State                                                              */
/* ------------------------------------------------------------------ */

typedef struct {
    double tissue[N_COMPARTMENTS];
    double bubbles;                /* normalised bubble volume scalar [0, 1]      */
} RgbmState;

/* ------------------------------------------------------------------ */
/* Initialise to surface equilibrium, no bubbles                      */
/* ------------------------------------------------------------------ */

static inline void rgbm_init(RgbmState *s) {
    for (int i = 0; i < N_COMPARTMENTS; i++)
        s->tissue[i] = p_inspired_n2(0.0);
    s->bubbles = 0.0;
}

/* ------------------------------------------------------------------ */
/* Advance tissue compartments by dt minutes (same Haldane equation)  */
/* ------------------------------------------------------------------ */

static inline void rgbm_update_tissues(RgbmState *s, double depth, double dt) {
    double pi = p_inspired_n2(depth);
    for (int i = 0; i < N_COMPARTMENTS; i++) {
        double k = log(2.0) / COMP[i].ht;
        s->tissue[i] += (pi - s->tissue[i]) * (1.0 - exp(-k * dt));
    }
}

/* ------------------------------------------------------------------ */
/* Advance bubble scalar by dt minutes                                */
/*                                                                    */
/* Bubbles grow when any compartment is supersaturated relative to    */
/* ambient pressure (squared to emphasise high supersaturation).      */
/* They reabsorb slowly, faster at depth (compression effect).        */
/* ------------------------------------------------------------------ */

static inline void rgbm_update_bubbles(RgbmState *s, double depth, double dt) {
    double p_amb = pressure(depth);
    double growth = 0.0;

    for (int i = 0; i < N_COMPARTMENTS; i++) {
        double supersat = (s->tissue[i] - p_amb) / p_amb;
        if (supersat > 0.0)
            growth += supersat * supersat;
    }

    s->bubbles += RGBM_BUBBLE_GROWTH_RATE * growth * dt;

    double decay = RGBM_BUBBLE_DECAY_RATE * dt * (1.0 + depth * 0.05);
    s->bubbles -= decay;

    if (s->bubbles < 0.0) s->bubbles = 0.0;
    if (s->bubbles > 1.0) s->bubbles = 1.0;
}

/* ------------------------------------------------------------------ */
/* Combined per-step update (tissues + bubbles)                       */
/* ------------------------------------------------------------------ */

static inline void rgbm_update(RgbmState *s, double depth, double dt) {
    rgbm_update_tissues(s, depth, dt);
    rgbm_update_bubbles(s, depth, dt);
}

/* ------------------------------------------------------------------ */
/* Compute lambda from current bubble volume                          */
/* ------------------------------------------------------------------ */

static inline double rgbm_lambda(const RgbmState *s) {
    return 1.0 - (1.0 - RGBM_LAMBDA_MIN) * s->bubbles;
}

/* ------------------------------------------------------------------ */
/* Compute decompression ceiling (metres), bubble-penalised           */
/* ------------------------------------------------------------------ */

static inline double rgbm_ceiling(const RgbmState *s) {
    double lambda       = rgbm_lambda(s);
    double bubble_penalty = 1.0 + (1.0 - lambda);
    double p_ceil       = 0.0;

    for (int i = 0; i < N_COMPARTMENTS; i++) {
        double adjusted_m = COMP[i].a + COMP[i].b * P_SURFACE / bubble_penalty;
        double excess     = s->tissue[i] - adjusted_m;
        if (excess > p_ceil) p_ceil = excess;
    }

    double depth = (p_ceil - P_SURFACE) / BAR_PER_METRE;
    return depth > 0.0 ? depth : 0.0;
}

#endif