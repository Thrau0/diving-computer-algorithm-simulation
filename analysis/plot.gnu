# =========================================================
# COLOR CONFIG (EDIT ONLY THIS SECTION)
# =========================================================

DepthColor   = "#1f4e79"
BuhlmannCol  = "#555555"
RGBMCol      = "#999999"
LambdaCol    = "#4a90e2"
DeltaCol     = "#7b2cbf"
GridCol      = "#cccccc"

# =========================================================
# STYLE DEFINITIONS
# =========================================================

set style line 1 lc rgb DepthColor  lw 2
set style line 2 lc rgb BuhlmannCol lw 2 dt 2
set style line 3 lc rgb RGBMCol     lw 2 dt 3
set style line 4 lc rgb LambdaCol   lw 1
set style line 5 lc rgb DeltaCol    lw 1   # Δ (RGBM - Bühlmann)

# =========================================================
# MAIN COMPARISON PLOTS
# =========================================================

set terminal pngcairo size 1200,900 enhanced font "Arial,10"
set output "images/dive_profiles.png"

set multiplot layout 2,2 title "Dive Profile Comparison (Bühlmann vs RGBM)"

set grid lc rgb GridCol
set key outside
set xlabel "Time [min]"
set ylabel "Depth / Ceiling [m]"
set y2label "λ RGBM"

set ytics nomirror
set y2tics
set style data lines
set border lw 1
set yrange [-2:*] reverse


# ---------------- PROFILE 1 ----------------
set title "20 m No-Stop Dive"
plot \
"profile1_rec20m.dat" using 1:2 ls 1 title "Depth", \
"" using 1:3 ls 2 title "Ceiling Bühlmann", \
"" using 1:4 ls 3 title "Ceiling RGBM", \
"" using 1:5 axes x1y2 ls 4 title "λ RGBM"

# ---------------- PROFILE 2 ----------------
set title "40 m Deep Dive"
plot \
"profile2_deep40m.dat" using 1:2 ls 1 notitle, \
"" using 1:3 ls 2 notitle, \
"" using 1:4 ls 3 notitle, \
"" using 1:5 axes x1y2 ls 4 notitle

# ---------------- PROFILE 3 ----------------
set title "60 m Technical Dive"
plot \
"profile3_tech60m.dat" using 1:2 ls 1 notitle, \
"" using 1:3 ls 2 notitle, \
"" using 1:4 ls 3 notitle, \
"" using 1:5 axes x1y2 ls 4 notitle

# ---------------- PROFILE 4 ----------------
set title "30 m Recreational Dive"
plot \
"profile4_rec30m.dat" using 1:2 ls 1 notitle, \
"" using 1:3 ls 2 notitle, \
"" using 1:4 ls 3 notitle, \
"" using 1:5 axes x1y2 ls 4 notitle

unset multiplot


# =========================================================
# SPECIAL PROFILES (REPETITIVE + YO-YO)
# =========================================================

set terminal pngcairo size 900,900 enhanced font "Arial,10"
set output "images/dive_profiles_repetitive_yo-yo.png"

set multiplot layout 2,1 title "Special Dive Profiles (Bühlmann vs RGBM)"

set grid lc rgb GridCol
set key top right
set xlabel "Time [min]"
set ylabel "Depth / Ceiling [m]"
set y2label "λ RGBM"

set ytics nomirror
set y2tics
set style data lines
set yrange [-2:*] reverse

# ---------------- PROFILE 5 ----------------
set title "Repetitive 25 m Dive (30 min SI)"
plot \
"profile5_repetitive25m.dat" using 1:2 ls 1 title "Depth", \
"" using 1:3 ls 2 title "Ceiling Bühlmann", \
"" using 1:4 ls 3 title "Ceiling RGBM", \
"" using 1:5 axes x1y2 ls 4 title "λ RGBM"

# ---------------- PROFILE 6 ----------------
set title "Yo-yo Dive"
plot \
"profile6_yoyo.dat" using 1:2 ls 1 title "Depth", \
"" using 1:3 ls 2 title "Ceiling Bühlmann", \
"" using 1:4 ls 3 title "Ceiling RGBM", \
"" using 1:5 axes x1y2 ls 4 title "λ RGBM"

unset multiplot


# =========================================================
# DIFFERENCE-ONLY VIEW
# =========================================================

set terminal pngcairo size 1200,1000 enhanced font "Arial,10"
set output "images/dive_profiles_differences_all.png"

set multiplot layout 3,2 title "RGBM vs Bühlmann Ceiling Differences"

set grid lc rgb "#cccccc"
set xlabel "Time [min]"
set ylabel "Δ Ceiling (RGBM - Bühlmann) [m]"

set zeroaxis lw 1
set yrange [-10:10]

set style line 1 lc rgb DeltaCol lw 1

# ---------------- PROFILE 1 ----------------
set title "20 m No-Stop Dive"
plot "profile1_rec20m.dat" using 1:($4-$3) w l ls 1 notitle

# ---------------- PROFILE 2 ----------------
set title "40 m Deep Dive"
plot "profile2_deep40m.dat" using 1:($4-$3) w l ls 1 notitle

# ---------------- PROFILE 3 ----------------
set title "60 m Technical Dive"
plot "profile3_tech60m.dat" using 1:($4-$3) w l ls 1 notitle

# ---------------- PROFILE 4 ----------------
set title "30 m Recreational Dive"
plot "profile4_rec30m.dat" using 1:($4-$3) w l ls 1 notitle

# ---------------- PROFILE 5 ----------------
set title "Repetitive 25 m Dive"
plot "profile5_repetitive25m.dat" using 1:($4-$3) w l ls 1 notitle

# ---------------- PROFILE 6 ----------------
set title "Yo-yo Dive"
plot "profile6_yoyo.dat" using 1:($4-$3) w l ls 1 notitle

unset multiplot