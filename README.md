# Heisenberg Model in 3d
### Computational Statistical Physics
$Mateo C\'{a}rdenes Wuttig, Justus Grabowsky, Daniel Schwarzenbach, Constan\c{c}a Tropa$

# CSP Project FS24

### To Do

- Daniel Schwarzenbach
    -   implement Memory tracker
    -   merge incoming changes
    -   Investigate Ippl further, cuda?
- Mateo Cárdenes Wuttig
    - Implement measure functions insider ./code/Measure
- Constança Tropa
    - Implemente Wolf algorithm, one where the cluster can go over the Boundry and one where it can't(for parralelisation)
- Justus
    - Implement the Metropolis Algorightm
    - Implement the adaptive Metropolis Algorightm


## Mac-Os

First u need to install [Homebrew](https://docs.brew.sh/Installation)

install GCC 11 for c++23
```bash
brew install gcc@11 --without-multilib
brew link gcc@11 --force
```
If you’ve previously installed GCC using Homebrew, you can reinstall it with:
```bash
brew reinstall gcc@11 --without-multilib
```

install cmake
```bash
brew install cmake
```

install eigen
```bash
brew install eigen
```

install openmpi
```bash
brew install openmpi --build-from-source --cc=gcc-11
```

## Linux

install GCC 11 for c++23
```bash
sudo apt-get update
sudo apt-get install g++-11
```
install cmake
```bash
brew install cmake
```

install openmpi on **Debian**
```bash
sudo apt-get install openmpi-bin libopenmpi-dev
```

install openmpi on **other Linux dirtros**
- Download the Open MPI source code from the main Open MPI website.
```bash
# Extract the tarball:
tar xf openmpi-<version>.tar.bz2
cd openmpi-<version>
# Configure and build Open MPI
# Replace <path> with your desired installation directory.
./configure --prefix=<path> [ ...options...]
make [-j N] all
# Install Open MPI:
sudo make install

```

## IPPL-Library

Ippl only suborts Linux and Cray!

Documentation can be found in ./docs/ippl_user_guide.pdf but is outdated

build entire library and dependencies:
```bash
sh ./libraries/Ippl/ippl_build.sh
```

[Github-IPPL](https://github.com/IPPL-framework/ippl)



## Matplot++-Library

Is auto installed through CMake

[Documentation](https://alandefreitas.github.io/matplotplusplus/)

[Github-Matplot++](https://github.com/alandefreitas/matplotplusplus)
