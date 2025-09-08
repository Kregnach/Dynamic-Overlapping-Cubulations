# Dynamic Overlapping Cubulations

This repository implements **Monte Carlo simulations of 3D cubulations** of a 3-ball with spherical boundary. The model explores random geometries built from glued cubes under strict local topological rules, and studies their phase transitions.

---

## Features

- **Geometry representation**
  - `Cube`, `Face`, and `Vector3` classes define cubulated structures.
  - `Ball` class manages the collection of cubes and boundary faces.

- **Monte Carlo simulation**
  - Growth moves (`grow_cube.h`) and shrink moves (`shrink_cube.h`) modify the geometry.
  - Acceptance is computed with detailed balance corrections.
  - Configurable action: bulk and boundary couplings (`lambda`, `alpha`, `kappa`, `epsilon`).

- **Measurements**
  - Volume (`V`), boundary area (`A`), curvature (`K`).
  - Minimal necks and boundary connectivity.
  - Radial histograms of cube distribution.

- **Configuration system**
  - Parameters are read from simple text files (`config.h`).
  - `gen-cfg.sh` script generates batches of config files for sweeps.
  - Key parameters:
    - `seed` — RNG seed  
    - `A`, `V` — target boundary area and volume  
    - `lambda`, `alpha`, `kappa`, `epsilon` — couplings  
    - `steps`, `thermal`, `sweeps` — simulation length  
    - `inname`, `outname` — filenames for saving/loading cubulations  

- **I/O**
  - `iocube.h`: save and load full cubulations with cube positions, neighbors, and boundary faces.
  - Measurement outputs written to `cube-<name>.out`, `necks-<name>.out`, and optional histograms.

---

## Usage

### Generate configs
Use the helper script to create parameter files:

```
./gen-cfg.sh
```

This produces files like `Cfg-kappa-lambda-A.txt`.

---

### Compile
All `.cpp` and `.h` files are self-contained. You can compile with:

```
g++ -std=c++17 main.cpp -o cubulation
```

---

### Run
Run the simulation with a config file:

```
./cubulation Cfg-<name>.txt
```

---

### Outputs

- `cube-<name>.out` — main observables per sweep  
- `necks-<name>.out` — neck statistics (if enabled)  
- `rhist-<name>.out` — radial histograms (if enabled)  
- `conf-<name>.dat` — saved cubulation configuration  

---

## Example

```
./gen-cfg.sh
g++ -std=c++17 main.cpp -o cubulation
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
- `iocube.h` — save/load cubulations  
- `random.h` — RNG (Xoshiro256++)  
- `gen-cfg.sh` — helper script for generating config files  

---

## License

--
