# Getting Started

## Installation

To install from source, clone the Ekizu repo with:

```bash
git clone https://github.com/jontitorr/ekizu
```

Build and install with:

```bash
cd ekizu
cmake -S. -Bbuild
cmake --build build --target install
```

The last command may need sudo. Alternatively, provide a prefix to install somewhere local:

```bash
cmake --build build --target install --prefix $HOME/.local
```
