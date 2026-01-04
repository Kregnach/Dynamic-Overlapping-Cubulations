# Dynamic Overlapping Cubulations

This repository implements **Monte Carlo simulations of 3D cubulations** of a 3-ball with spherical boundary. The model explores random geometries built from glued cubes under local topological rules.

---

## Quickstart

Build:

```
g++ -std=c++17 -O3 main.cpp -o cubulation
```

Run (with a config file):

```
./cubulation Cfg-your-run.txt
```

---

## What’s implemented

- **Geometry representation**
  - `Cube`, `Face`, and `Vector3` classes define cubulated structures.
  - `Ball` class manages the collection of cubes and boundary faces.

- **Monte Carlo simulation**
  - Growth moves (`grow_cube.h`) and shrink moves (`shrink_cube.h`) modify the geometry.
  - Acceptance is computed with detailed balance corrections.
  - Configurable action: bulk and boundary couplings (`lambda`, `alpha`, `epsilon`).

- **Measurements**
  - Volume (number of cubes), boundary area (number of boundary faces), and radius moments (see `measure.h`).

- **Configuration system**
  - Parameters are read from simple text files (`config.h`).
  - `gen-cfg.sh` script generates batches of config files for sweeps.

---

## Usage

### Generate configs
Use the helper script to create parameter files:

```
./gen-cfg.sh
```

This produces files like `Cfg-...txt` (see the script for naming).

---

### Compile
All code is built by compiling `main.cpp` (it includes the required headers). You can compile with:

```
g++ -std=c++17 -O3 main.cpp -o cubulation
```

Optional (often faster on your machine):

```
g++ -std=c++17 -O3 -march=native main.cpp -o cubulation
```

---

### Run
Run the simulation with a config file:

```
./cubulation Cfg-<name>.txt
```

---

## Config file format

The config file is whitespace-separated `key value` pairs (one per line). Required keys:

- `seed`
- `A`, `V`
- `startsize`
- `lambda`, `alpha`, `epsilon`
- `steps`, `thermal`, `sweeps`
- `name`

Example:

```
seed 1
A 48
V 40
startsize 1
lambda -0.2
alpha 0.05
epsilon 0.0
steps 50
thermal 3
sweeps 1
name demo
```

---

### Outputs

- `cube-<name>.out` — main observables appended over time (see `measure.h`)
- `Boundary-<name>.out` — boundary-face adjacency list
- `Cubulation-<name>.out` — cube neighbor list
- `CubeDensity-<name>.out` — cube coordinates (ID and x/y/z)

---

## Example

```
./gen-cfg.sh
g++ -std=c++17 -O3 main.cpp -o cubulation
./cubulation Cfg--0.4-2400.txt
```

This will run a cubulation simulation at `λ = -0.4`, area `A = 2400`, saving outputs with the chosen `name`.

---

## File Overview

- `main.cpp` — entry point, runs the Monte Carlo loop  
- `ball.h` — main `Ball` class, simulation manager  
- `cube.h` — `Cube`, `Face`, and `Vector3` classes  
- `grow_cube.h` / `shrink_cube.h` — Monte Carlo moves  
- `measure.h` — observable measurements  
- `mc.h` — coupling tuning utilities  
- `config.h` — config reader  
- `random.h` — RNG (Xoshiro256++)  
- `gen-cfg.sh` — helper script for generating config files  

---

## License

--
