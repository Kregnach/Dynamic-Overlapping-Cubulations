# Dynamic Overlapping Cubulations

A C++ implementation of **Monte Carlo simulations of 3D cubulations** of a 3-ball with spherical boundary. This code explores random geometries built from glued cubes under strict local topological rules, studying phase transitions and geometric properties of cubulated structures.

**Author:** Dániel Németh  
**Last Updated:** 2026-01-07

---

## Table of Contents

- [Overview](#overview)
- [Quickstart](#quickstart)
- [Installation & Requirements](#installation--requirements)
- [Usage](#usage)
- [Configuration](#configuration)
- [Monte Carlo Algorithm](#monte-carlo-algorithm)
- [Output Files](#output-files)
- [Performance Optimizations](#performance-optimizations)
- [Code Structure](#code-structure)
- [Examples](#examples)
- [Troubleshooting](#troubleshooting)

---

## Overview

This simulation implements a Monte Carlo method for generating and studying 3D cubulations—geometric structures composed of unit cubes glued together along their faces. The model:

- **Maintains topological constraints** ensuring valid cubulation structures
- **Uses detailed balance** for correct Monte Carlo sampling
- **Supports configurable actions** with bulk and boundary couplings
- **Measures observables** including volume, boundary area, and radius moments
- **Optimized for performance** with caching and efficient data structures

The action functional is:
```
S = α·A + λ·V + κ·(additional terms) + ε·(V - V₀)²
```
where:
- `A` = boundary area (number of boundary faces)
- `V` = volume (number of cubes)
- `α` = boundary coupling
- `λ` = bulk coupling
- `κ` = additional coupling parameter (kappa)
- `ε` = volume constraint strength
- `V₀` = target volume

---

## Quickstart

```bash
# Generate configuration files
./gen-cfg.sh

# Compile
g++ -std=c++17 -O3 main.cpp -o cubulation

# Run simulation
./cubulation Cfg--0.4-2400.txt
```

---

## Installation & Requirements

### Requirements

- **C++17** compatible compiler (g++, clang++)
- **Bash** (for configuration generation script)
- **Linux/Unix-like system** (or WSL on Windows)

### Compilation

Basic compilation:
```bash
g++ -std=c++17 -O3 main.cpp -o cubulation
```

Optimized for your CPU architecture:
```bash
g++ -std=c++17 -O3 -march=native main.cpp -o cubulation
```

With debugging symbols:
```bash
g++ -std=c++17 -g -O0 main.cpp -o cubulation
```

---

## Usage

### Step 1: Generate Configuration Files

The `gen-cfg.sh` script generates parameter sweep files:

```bash
./gen-cfg.sh
```

This creates configuration files like `Cfg-<kappa>-<lambda>-<A>.txt` with different parameter combinations. Edit the script to customize the parameter ranges.

### Step 2: Compile

```bash
g++ -std=c++17 -O3 main.cpp -o cubulation
```

### Step 3: Run Simulation

```bash
./cubulation Cfg-<name>.txt
```

The simulation will:
1. Initialize a cubulation structure
2. Perform thermalization steps
3. Run the Monte Carlo simulation
4. Output measurements to files

---

## Configuration

Configuration files use whitespace-separated `key value` pairs (one per line).

### Required Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `seed` | int | Random number generator seed |
| `A` | int | Target boundary area (number of boundary faces) |
| `V` | int | Target volume (number of cubes) |
| `startsize` | int | Initial cubic structure size (N×N×N) |
| `lambda` | double | Bulk coupling (λ) |
| `alpha` | double | Boundary coupling (α) |
| `epsilon` | double | Volume constraint strength (ε) |
| `steps` | int | Monte Carlo steps per thermal cycle |
| `thermal` | int | Number of thermalization cycles |
| `sweeps` | int | Number of measurement sweeps |
| `name` | string | Output file identifier |

### Additional Parameters

| Parameter | Type | Description |
|-----------|------|-------------|
| `kappa` | double | Additional coupling parameter used in Monte Carlo action calculations |
| `tuneAV` | int | Tuning mode selector: `0` = tune volume (V), `1` = tune area (A). Controls whether `tuneV()` or `tuneA()` is called during thermalization |
| `initialsteps` | int | Initial growth steps before equilibration |
| `inname`, `outname` | string | Input/output cubulation state files (for saving/loading configurations) |
| `fromfile` | int | Flag to load initial cubulation from file (`1` = load from `inname`, `0` = start fresh) |
| `badjacency` | int | Output flag: `1` = write boundary adjacency to `Boundary-<name>.out` |
| `cadjacency` | int | Output flag: `1` = write cube adjacency to `Cubulation-<name>.out` |
| `cdensity` | int | Output flag: `1` = write cube density/coordinates to `CubeDensity-<name>.out` |
| `neckstat` | int | Output flag: `1` = write neck statistics to `necks-<name>.out` |
| `rhist` | int | Output flag: `1` = write radial histograms to `rhist-<name>.out` |

### Example Configuration

```
seed            12345
tuneAV          0
A               2400
V               1000
startsize       10
lambda          -0.4
alpha           1.2
kappa           0.0
epsilon         0.002
initialsteps    1000
steps           1000
thermal         20000
sweeps          0
name            test-run
badjacency      1
cadjacency      1
cdensity        1
neckstat        0
rhist           0
```

---

## Monte Carlo Algorithm

The simulation uses a Metropolis-Hastings Monte Carlo algorithm with:

### Moves

1. **Grow Move**: Add a cube to a boundary face
   - Selects a random boundary face
   - Validates topological constraints
   - Accepts/rejects based on action difference

2. **Shrink Move**: Remove a cube from the boundary
   - Selects a random boundary face
   - Validates removal is topologically valid
   - Accepts/rejects based on action difference

### Acceptance Probability

For a move that changes volume by ΔV and boundary area by ΔA:

```
P(accept) = min(1, exp(-ΔS) · (A_old/A_new) · factor)
```

where:
- `ΔS = α·ΔA + λ·ΔV + ε·(2V·ΔV + ΔV²)`
- `factor` accounts for the number of ways to perform the reverse move

### Thermalization

The simulation performs:
1. **Initial growth**: Grows to target volume V (or uses `initialsteps`)
2. **Equilibration**: Random grow/shrink moves to reach equilibrium
3. **Thermal cycles**: Multiple cycles with automatic tuning:
   - If `tuneAV=0`: Calls `tuneV()` to adjust `lambda` and maintain target volume V
   - If `tuneAV=1`: Calls `tuneA()` to adjust `alpha` and maintain target boundary area A

---

## Output Files

The simulation generates several output files:

| File | Description |
|------|-------------|
| `cube-<name>.out` | Main observables: volume, boundary area, radius moments (R, R², R³, R⁴), couplings |
| `Boundary-<name>.out` | Boundary face adjacency list (if `badjacency=1`) |
| `Cubulation-<name>.out` | Cube neighbor connectivity list (if `cadjacency=1`) |
| `CubeDensity-<name>.out` | Cube coordinates (ID, x, y, z) (if `cdensity=1`) |
| `necks-<name>.out` | Neck statistics (if `neckstat=1`) |
| `rhist-<name>.out` | Radial histograms (if `rhist=1`) |

### Output Format: `cube-<name>.out`

Each line contains (tab-separated):
```
V    A    R    R²    R³    R⁴    λ    α
```

where:
- `V` = current volume (number of cubes)
- `A` = current boundary area (number of boundary faces)
- `R`, `R²`, `R³`, `R⁴` = radius moments (average distance from centroid)
- `λ` = current bulk coupling
- `α` = current boundary coupling

---

## Performance Optimizations

**Optimized:** 2026-01-07 by Dániel Németh

The code includes several performance optimizations:

- **Member variable caching**: Frequently accessed variables (`nextCubeId`, `nextFaceBId`) are cached in tight loops
- **Precomputed inverses**: Division operations replaced with multiplication by precomputed inverse values
- **Orthogonals caching**: Direction arrays cached to avoid redundant allocations
- **Action probability optimization**: Cached coupling parameters and precomputed double conversions
- **Loop optimizations**: Reduced redundant calculations in measurement and tuning functions

These optimizations provide significant speedup in long-running simulations, especially during thermalization and measurement phases.

---

## Code Structure

### Core Classes

- **`Ball`** (`ball.h`): Main simulation manager
  - Manages cubes and faces
  - Implements Monte Carlo moves
  - Handles boundary operations
  - Performs measurements

- **`Cube`** (`cube.h`): Represents a unit cube
  - Stores position (Vector3)
  - Maintains neighbor connections (27 possible neighbors)
  - Links to 6 faces

- **`Face`** (`cube.h`): Represents a cube face
  - Can be boundary or interior
  - Maintains adjacency relationships
  - Links to associated cubes

- **`Vector3`** (`cube.h`): 3D integer vector
  - Optimized for axis-aligned operations
  - Efficient neighbor indexing

### Key Files

| File | Purpose |
|------|---------|
| `main.cpp` | Entry point, simulation loop |
| `ball.h` | Main Ball class definition |
| `cube.h` | Cube, Face, Vector3 classes |
| `grow_cube.h` | Cube growth move implementation |
| `shrink_cube.h` | Cube shrink move implementation |
| `measure.h` | Observable measurements |
| `mc.h` | Coupling tuning (tuneV, tuneA) |
| `config.h` | Configuration file reader |
| `random.h` | RNG (Xoshiro256++) |
| `objects.h` | Object creation/deletion with pooling |
| `initialize.h` | Initial structure setup |
| `helper.h` | Helper functions for cube/face operations |
| `print.h` | Output formatting functions |
| `checks.h` | Validation functions |
| `gen-cfg.sh` | Configuration file generator |

### Data Structures

- **Object pooling**: Reuses Cube and Face objects to minimize allocations
- **Efficient indexing**: Uses arrays with direct indexing for neighbors
- **Boundary tracking**: Maintains separate boundary face list for fast random access

---

## Examples

### Basic Run

```bash
# Generate configs
./gen-cfg.sh

# Compile
g++ -std=c++17 -O3 main.cpp -o cubulation

# Run
./cubulation Cfg--0.4-0-2400.txt
```

This runs a simulation with:
- λ = -0.4 (bulk coupling)
- α = 0 (boundary coupling, from script default)
- A = 2400 (target boundary area)

### Parameter Sweep

Edit `gen-cfg.sh` to create a parameter sweep:

```bash
# Example: sweep over lambda values
for l in -0.5 -0.4 -0.3 -0.2; do
    # ... generate configs ...
done
```

Then run each config file sequentially or in parallel.

### Analyzing Output

```bash
# View main observables
cat cube-test-run.out

# Plot radius evolution (requires plotting tools)
# Column 3 is average radius R
awk '{print NR, $3}' cube-test-run.out > radius.dat
```

---

## Troubleshooting

### Common Issues

**Problem**: Compilation errors about C++17 features
- **Solution**: Ensure your compiler supports C++17. Use `g++ --version` to check.

**Problem**: "Failed to open file for output"
- **Solution**: Check write permissions in the current directory.

**Problem**: Simulation runs very slowly
- **Solution**: 
  - Use `-O3` optimization flag
  - Consider reducing `thermal` or `steps` for testing
  - Check that you're using the optimized version (not debug build)

**Problem**: Volume doesn't converge to target V
- **Solution**: 
  - Increase `thermal` cycles
  - Adjust `epsilon` (volume constraint strength)
  - Check that `lambda` tuning is working (see `tuneV()` in `mc.h`)

**Problem**: Config file not found
- **Solution**: Ensure config file path is correct and file exists. Use absolute path if needed.

### Performance Tips

- Use `-march=native` for CPU-specific optimizations
- Reduce measurement frequency for faster runs (modify `main.cpp`)
- For large simulations, monitor memory usage (max cubes/faces: 100,000)

### Debugging

Compile with debug symbols:
```bash
g++ -std=c++17 -g -O0 main.cpp -o cubulation-debug
```

Enable validation checks (uncomment validation calls in code).

---

## License

See repository for license information.

---

## Contributing

For questions, issues, or contributions, please refer to the repository.

**Last Updated:** 2026-01-07  
**Performance Optimizations:** Dániel Németh
