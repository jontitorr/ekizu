# Ekizu

A WIP C++ library for Discord applications.

## Features

- Low memory footprint
- Discord API v10 support
- Gateway support, with auto-reconnect
- JSON support, convert and serialize events/structures to/from JSON

## Getting Started

### Prerequisites

- [CMake](https://cmake.org/download/) (version >= 3.16)
- Compiler with C++17 support, i.e. MSVC, GCC, Clang

### Installing

This library uses [CPM.cmake](https://github.com/cpm-cmake/CPM.cmake) to manage dependencies. It is an amazing package manager for CMake projects and allows us to install the entire library using the following commands:

```bash
  git clone https://www.github.com/xminent/ekizu
  cd ekizu
  cmake -S . -B build
  cmake --build build --target install
```

From there you can simply integrate it into your CMake project like so:

```cmake
    find_package(ekizu REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE ekizu::ekizu)
```

## Usage/Examples

See [examples](https://github.com/Xminent/ekizu/tree/dev/examples).

## Dependencies

### Third party Dependencies

- [fmt](https://github.com/fmtlib) (comes bundled with project)
- [net](https://github.com/xminent/net) (comes bundled with project)
- [nlohmann_json](https://github.com/nlohmann/json) (comes bundled with project)
- [zlib](https://github.com/madler/zlib) (comes bundled with project, unless you have it installed)

## License

[MIT](https://choosealicense.com/licenses/mit/)