#ifndef PROFILE_H
#define PROFILE_H

/**
 * 
 * This file defines dive profiles and the shared ZHL-16C compartment parameters.
 * Both buehlmann.c and rgbm.c read everything they need from this file here. 
 * 
 * A dive profile is a sequence of segments. Each segment describes a 
 * constant-rate depth change (or holding a fixed depth) and how long 
 * it lasts. The algorithms step through these segments second by second.
 * 
 */

#include <stddef.h>



/* ------------------------------------------------------------------ */
/* Physical Constraints                                               */
/* ------------------------------------------------------------------ */

#define P_SURFACE       1.0         // surface pressure
#define BAR_PER_METRE   0.1         // pressure increases by 1 bar every 10 metres of seawater
#define FIN2            0.79        // nitrogen fraction in air
#define N_COMPARTMENTS  16          // ZHL-16C tissue compartments


static inline double pressure(double depth_m){
    return P_SURFACE + depth_m * BAR_PER_METRE;
}

static inline double p_inspired_n2(double depth_m){
    return pressure(depth_m) * FIN2;
}

/**
 * ZHL-16C compartment parameters
 * 
 * half_time    : nitrogen half-times (minutes)
 * a            : M-value intercept (bar)
 * b            : M-value slope (dimensionless)
 */

typedef struct {double ht, a, b; } Compartment;

static const Compartment COMP[N_COMPARTMENTS] = {
    {   5.0, 1.1696, 0.5578 },
    {   8.0, 1.0000, 0.6514 },
    {  12.5, 0.8618, 0.7222 },
    {  18.5, 0.7562, 0.7825 },
    {  27.0, 0.6667, 0.8126 },
    {  38.3, 0.5600, 0.8434 },
    {  54.3, 0.4947, 0.8693 },
    {  77.0, 0.4500, 0.8910 },
    { 109.0, 0.4187, 0.9092 },
    { 146.0, 0.3798, 0.9222 },
    { 187.0, 0.3497, 0.9319 },
    { 239.0, 0.3223, 0.9403 },
    { 305.0, 0.2850, 0.9477 },
    { 390.0, 0.2737, 0.9544 },
    { 498.0, 0.2523, 0.9602 },
    { 635.0, 0.2327, 0.9653 }
};

/* ------------------------------------------------------------------ */
/* Dive segment and profile                                           */
/* ------------------------------------------------------------------ */


typedef struct {
    double start_m;             // depth at start of the segment (metres)
    double end_m;               // depth at the end of the segment (metres)
    double duration_min;        // duration of the segment (in minutes)
} Segment;

typedef struct {
    const char *name;
    const Segment *segments;
    const char *filename;
    int n;
} Profile;

/* ------------------------------------------------------------------ */
/* Dive profiles                                                      */
/* ------------------------------------------------------------------ */

/**
 * A template profile was given to the AI Model 
 * Claude Code to generate specific dive profiles.
 */

/* Profile 1: short recreational dive to 20 m, no decompression needed */
static const Segment SEG1[] = {
    {  0, 20,  2 },   /* descend to 20 m         */
    { 20, 20, 25 },   /* stay at 20 m            */
    { 20,  0,  4 }    /* ascend to surface       */
};

/* Profile 2: deeper dive to 40 m with safety stops */
static const Segment SEG2[] = {
    {  0, 40,  4 },   /* descend to 40 m         */
    { 40, 40, 20 },   /* stay at 40 m            */
    { 40,  5,  7 },   /* ascend to 5 m           */
    {  5,  5,  5 },   /* safety stop at 5 m      */
    {  5,  0,  1 }    /* ascend to surface       */
};
 
/* Profile 3: technical dive to 60 m with staged decompression */
static const Segment SEG3[] = {
    {  0, 60,  6 },   /* descend to 60 m         */
    { 60, 60, 20 },   /* stay at 60 m            */
    { 60, 21,  8 },   /* ascend to first stop    */
    { 21, 21,  5 },   /* deco stop at 21 m       */
    { 21, 12,  2 },   /* ascend to next stop     */
    { 12, 12,  8 },   /* deco stop at 12 m       */
    { 12,  6,  1 },   /* ascend to next stop     */
    {  6,  6, 10 },   /* deco stop at 6 m        */
    {  6,  3,  1 },   /* ascend to last stop     */
    {  3,  3,  5 },   /* deco stop at 3 m        */
    {  3,  0,  1 }    /* ascend to surface       */
};

/* Profile 4: Normal 30 m recreational dive — both algorithms should agree */
static const Segment SEG4[] = {
    {  0, 30,  3 },   /* descend to 30 m         */
    { 30, 30, 20 },   /* stay at 30 m            */
    { 30,  5,  5 },   /* ascend to safety stop   */
    {  5,  5,  3 },   /* safety stop at 5 m      */
    {  5,  0,  1 }    /* ascend to surface       */
};

/* Profile 5: Repetitive dive with short surface interval
 *
 * Dive 1: 25 m for 30 min, surface.
 * Surface interval: 30 min  (modelled as a segment at 0 m)
 * Dive 2: 25 m for 20 min, surface.
 */
static const Segment SEG5[] = {
    /* --- Dive 1 --- */
    {  0, 25,  3 },   /* descend to 25 m         */
    { 25, 25, 30 },   /* stay at 25 m            */
    { 25,  0,  5 },   /* ascend to surface       */
    /* --- Surface interval --- */
    {  0,  0, 30 },   /* 30 min surface interval */
    /* --- Dive 2 --- */
    {  0, 25,  3 },   /* descend to 25 m again   */
    { 25, 25, 20 },   /* stay at 25 m            */
    { 25,  5,  4 },   /* ascend to safety stop   */
    {  5,  5,  3 },   /* safety stop at 5 m      */
    {  5,  0,  1 }    /* ascend to surface       */
};

/* Profile 6: Yo-yo / interrupted ascent
 *
 * Descend to 40 m, then make a fast partial ascent to 10 m (bubble
 * nucleation phase), hold briefly, then re-descend to 30 m, and
 * finally do a slow ascent with stops.
 */
static const Segment SEG6[] = {
    {  0, 40,  4 },   /* descend to 40 m              */
    { 40, 40, 25 },   /* bottom time at 40 m          */
    { 40, 10,  5 },   /* fast ascent to 10 m (yo-yo!) */
    { 10, 10,  5 },   /* shallow hold — bubbles form  */
    { 10, 30,  4 },   /* re-descend to 30 m           */
    { 30, 30, 10 },   /* second bottom phase          */
    { 30,  6,  5 },   /* ascend to first deco stop    */
    {  6,  6,  8 },   /* deco stop at 6 m             */
    {  6,  3,  1 },   /* ascend to last stop          */
    {  3,  3,  5 },   /* deco stop at 3 m             */
    {  3,  0,  1 }    /* ascend to surface            */
};

static const Profile PROFILES[] = {
    { "Recreational 20 m no-stop dive",                 SEG1, "profile1_rec20m",        3  },
    { "Deep 40 m dive with safety stops",               SEG2, "profile2_deep40m",       5  },
    { "Technical 60 m dive with staged deco stops",     SEG3, "profile3_tech60m",       11 },
    { "Normal 30 m recreational dive",                  SEG4, "profile4_rec30m",       5  },
    { "Repetitive 25 m dive, 30 min surface interval",  SEG5, "profile5_repetitive25m", 9  },
    { "Yo-yo 40->10->30 m interrupted ascent",          SEG6, "profile6_yoyo",          11 }
};
 
static const int N_PROFILES = 6;


#endif