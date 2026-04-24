# 🫧 Diving Computer Algorithm Simulation

This project is part of the course **“Introduction into Scientific Work” (University of Innsbruck)** and supports the seminar paper:

> **Analysis and Simulation of Decompression Algorithms in Modern Diving Computers**
> - Schmid Yannic
## 📌 Overview

This project simulates and compares decompression algorithms used in modern diving computers. The focus is on analyzing how different models behave under various dive profiles and conditions.

The implemented decompression models are:

- **Bühlmann ZHL-16C model**
- **RGBM (Reduced Gradient Bubble Model)**

The goal is to evaluate and visualize differences in predicted decompression behavior, especially for repetitive and multi-level dives.

## 📁 Project Structure

```
analysis/                   Simulationdata and Gnuplot scripts
analysis/images             Generated plots
code/                       Core simulation implementation (C)
deco_theory/                Theoretical background and supporting figures
```

## ⚙️ Features

### Dive Profile Simulation

The simulation supports multiple predefined dive scenarios:

- Recreational dives (20–30 m)
- Deep and technical dives (40–60 m)
- Repetitive dive profiles
- Yo-yo / multi-level dive patterns

### Computed Outputs

For each dive profile, the program computes:

- Bühlmann decompression ceilings
- RGBM decompression ceilings
- RGBM λ (lambda) bubble model parameter
- Differences between RGBM and Bühlmann predictions

### Visualization

All results are visualized using **Gnuplot**, including:

- Depth vs. time profiles
- Decompression ceiling comparisons
- Model deviation plots (RGBM − Bühlmann)

## 🔨Build & Run

### Compile the simulation

```bash
make run
```

After running, copy the generated `profileX\_...` output files into the analysis/ directory.

```bash
gnuplot plot.gnu
```

### Notes

Make sure the required profile output files are present in the analysis/ folder before running the script.
If new dive profiles are added, update plot.gnu accordingly to ensure the correct datasets are visualized.

## 📈 Results

The simulation highlights significant differences between classical dissolved-gas models and bubble-based decompression models.

Key observations include:

- Increased differences in repetitive dive profiles
- Diverging decompression ceilings in multi-level dives
- Stronger model deviation in deep repetitive exposures

For more details on the theoretical background and methodology, please refer to:

> **"Analysis and Simulation of Decompression Algorithms in Modern Diving Computers"**  
> Schmid Yannic

## ⚠️ Disclaimer

This project is intended for academic and research purposes only. It must not be used for real-world dive planning or safety-critical applications.

© All code and accompanying materials in this repository are the intellectual property of **Yannic Schmid**.
Unauthorized copying, redistribution, modification, or use of this repository or its parts is strictly prohibited without explicit permission from the author.
