# Fluent Bit C Plugins

This project provides a template structure for building native C plugins for Fluent Bit, including input, output, and filter plugins.

## Project Structure

```
.
├── src/
│   └── plugins/
│       ├── input/          # Input plugins
│       ├── output/         # Output plugins
│       └── filter/         # Filter plugins
├── include/                # Header files
├── config/                 # Fluent Bit configuration files
├── build/                  # Build output directory
├── CMakeLists.txt         # CMake build configuration
├── Dockerfile             # Docker build environment
└── build.sh               # Build script
```

## Requirements

- GCC or Clang compiler
- CMake 3.0+
- Fluent Bit development libraries
- pkg-config

## Building

### Local Build

```bash
./build.sh
```

### Docker Build

```bash
docker build -t fluentbit-plugins .
```

## Plugin Types

### Input Plugins
Located in `src/plugins/input/`, these plugins collect logs from various sources.

### Output Plugins
Located in `src/plugins/output/`, these plugins send logs to various destinations.

### Filter Plugins
Located in `src/plugins/filter/`, these plugins process or transform logs.

## Plugin Development

Each plugin requires:
1. C source file implementing the Fluent Bit plugin interface
2. CMakeLists.txt for building
3. Proper initialization, processing, and cleanup callbacks

## Running Fluent Bit with Plugins

```bash
fluent-bit -c config/fluent-bit.conf
```
