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
└── Dockerfile             # Docker build environment
```

## Requirements

- Docker

## Building

```bash
docker build -t fluentbit-plugin:latest .
```

This creates a complete container with:
- Fluent Bit v5.0.3
- All custom plugins compiled
- Pre-configured with sample configurations
- Ready to run: `docker run -it fluentbit-plugin:latest`

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
