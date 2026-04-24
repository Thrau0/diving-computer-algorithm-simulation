# =============================================================================
# Buehlmann ZHL-12 Decompression Algorithm
# =============================================================================

set encoding iso_8859_1

# -----------------------------------------------------------------------------
# Global style
# -----------------------------------------------------------------------------
set style line 11 lc rgb '#444444' lt 1 lw 1.0
set border 3 back ls 11
set tics nomirror
set grid back lc rgb '#cccccc' lw 0.4 lt 1

# -----------------------------------------------------------------------------
# Line styles (3 colors + dash types)
# -----------------------------------------------------------------------------
# Blue (primary)
set linetype 1  lw 2.4 lc rgb '#1b6ca8' dt 1   # solid
set linetype 2  lw 2.4 lc rgb '#1b6ca8' dt 2   # dashed
set linetype 3  lw 2.4 lc rgb '#1b6ca8' dt 3   # dotted

# Black (secondary)
set linetype 4  lw 2.2 lc rgb '#000000' dt 1
set linetype 5  lw 2.2 lc rgb '#000000' dt 2
set linetype 6  lw 2.2 lc rgb '#000000' dt 3

# Gray (background/reference)
set linetype 7  lw 1.4 lc rgb '#777777' dt 1
set linetype 8  lw 1.2 lc rgb '#999999' dt 2

# Highlight (limit line)
set linetype 10 lw 1.8 lc rgb '#000000' dt 4

# =============================================================================
# FIGURE 1 — Tissue saturation
# =============================================================================
set terminal pngcairo size 1400,900 enhanced font 'Arial,12'
set output 'figure1_saturation.png'

set xlabel 'Time (min)'
set ylabel 'Relative N_2 pressure  P_t / P_i'

set xrange [0:120]
set yrange [0:1.08]
set xtics 20
set ytics 0.2
set format y '%.1f'

# Margin for legend
set rmargin 12

# Legend (outside, readable)
set key outside right \
    box lw 1.0 lc rgb '#666666' \
    spacing 1.6 \
    samplen 4 \
    font 'Arial,12' \
    title 'T_{1/2} (N_2)'

# Reference line
set arrow from 0,1 to 120,1 nohead lt 8
set label 'Equilibrium (P_t = P_i)' at 62,1.03 tc rgb '#777777'

# Half-time labels
set label '2.65 min' at 3.5,0.54 tc rgb '#1b6ca8'
set label '7.94 min' at 9.5,0.54 tc rgb '#1b6ca8'
set label '18.5 min' at 20,0.54 tc rgb '#000000'
set label '37.0 min' at 38.5,0.54 tc rgb '#000000'
set label '79.0 min' at 80.5,0.54 tc rgb '#000000'

plot \
    'halftime_saturation.dat' u 1:2 w l lt 1 title 'C1  (2.65 min)', \
    '' u 1:3 w l lt 2 title 'C2  (7.94 min)', \
    '' u 1:4 w l lt 3 title 'C4  (18.5 min)', \
    '' u 1:5 w l lt 4 title 'C6  (37.0 min)', \
    '' u 1:6 w l lt 5 title 'C8  (79.0 min)', \
    '' u 1:7 w l lt 6 title 'C12 (238 min)', \
    '' u 1:8 w l lt 7 title 'C16 (635 min)'

unset arrow
unset label


# =============================================================================
# FIGURE 2 — M-value diagram
# =============================================================================
set output 'figure2_mvalue.png'

set xlabel 'Ambient pressure  P_{amb} (bar)'
set ylabel 'Inert gas tissue pressure  P_t (bar)'

set xrange [0.8:4.5]
set yrange [0.8:7.0]
set xtics 0.5
set ytics 1.0
set format x '%.1f'
set format y '%.0f'

# More space for legend
set rmargin 14

# Legend
set key outside right \
    box lw 1.0 lc rgb '#666666' \
    spacing 1.8 \
    samplen 4 \
    font 'Arial,12'

# Zone labels
set label 'Safe zone'   at 0.9,1.15 tc rgb '#000000'
set label 'Danger zone' at 0.9,6.6 tc rgb '#000000'

# Diagonal
set arrow from 0.8,0.8 to 4.5,4.5 nohead lt 8
set label 'P_t = P_{amb}' at 3.55,3.15 tc rgb '#777777' rotate by 46

plot \
    'mvalue_compartments.dat' index 0 u 1:2  w l lt 7 notitle, \
    '' index 0 u 1:3  w l lt 7 notitle, \
    '' index 0 u 1:4  w l lt 7 notitle, \
    '' index 0 u 1:5  w l lt 7 notitle, \
    '' index 0 u 1:6  w l lt 7 notitle, \
    '' index 0 u 1:7  w l lt 7 notitle, \
    '' index 0 u 1:8  w l lt 7 notitle, \
    '' index 0 u 1:9  w l lt 7 notitle, \
    '' index 0 u 1:10 w l lt 7 notitle, \
    '' index 0 u 1:11 w l lt 7 notitle, \
    '' index 0 u 1:12 w l lt 7 notitle, \
    '' index 0 u 1:13 w l lt 7 notitle, \
    '' index 0 u 1:14 w l lt 7 notitle, \
    '' index 0 u 1:15 w l lt 7 notitle, \
    '' index 0 u 1:16 w l lt 7 notitle, \
    '' index 0 u 1:17 w l lt 7 notitle, \
    '' index 0 u 1:2  w l lt 1 lw 2.4 title 'C1  (2.65 min)', \
    '' index 0 u 1:6  w l lt 2 lw 2.4 title 'C5  (26.5 min)', \
    '' index 0 u 1:9  w l lt 3 lw 2.4 title 'C8  (79.0 min)', \
    '' index 0 u 1:13 w l lt 4 lw 2.2 title 'C12 (238 min)', \
    '' index 0 u 1:17 w l lt 5 lw 2.2 title 'C16 (635 min)', \
    'mvalue_compartments.dat' index 2 u 1:3 w l lt 10 lw 1.8 title 'M-value limit (C5)'

unset arrow
unset label

set output