# MultiDimGrid

C++ library for creating discrete functions on a multi-dimensional grid with various available coordinate spacings

## Installation

### Download and compilation

Clone the repository into the desired location and change into the root directory by running

```bash
git clone https://github.com/rlilow/MultiDimGrid.git
cd MultiDimGrid
```

Afterwards run

```bash
make
```

## Usage

To use MultiDimGrid in your own code, link your program to the static library file `libmultidimgrid.a` and include the header file `MultiDimGrid.hpp`. Both files are in the root directory.

A small program demonstrating the usage of MultiDimGrid can be found in the directory `demo`. If you modify this, just re-run `make` in the root directory to rebuild it.

## Documentation 

If you have Doxygen (https://www.doxygen.nl/index.html) installed, you can build a detailed documentation of the different classes and functions in CORAS by running

```bash
make doc
```

from within the root directory.
This will create the directory `doc` containing the documentation.
To view it, open the file `doc/documentation.html` in the browser.