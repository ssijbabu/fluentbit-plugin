# Development Setup Guide

## Prerequisites

### Local Development

1. **Install dependencies** (Ubuntu/Debian):
```bash
sudo apt-get install -y build-essential cmake pkg-config libfluent-bit-dev
```

2. **Install Fluent Bit**:
```bash
curl https://raw.githubusercontent.com/fluent/fluent-bit/master/install.sh | sh
```

### Docker Development

The project includes a Dockerfile that provides a complete build environment:

```bash
docker build -t fluentbit-plugins .
```

## Project Structure

```
fluentbit-plugin/
├── src/plugins/
│   ├── input/           # Input plugins
│   ├── output/          # Output plugins
│   └── filter/          # Filter plugins
├── include/             # Shared headers
├── config/              # Fluent Bit configuration
├── build/               # Build output (generated)
├── CMakeLists.txt       # Root CMake configuration
├── Dockerfile           # Docker build
├── build.sh             # Build helper script
└── clean.sh             # Clean helper script
```

## Building

### Local Build

```bash
./build.sh
```

This will:
1. Create a `build/` directory
2. Run CMake configuration
3. Compile all plugins
4. Output `.so` files in `build/` directories

### Docker Build

```bash
docker build -t fluentbit-plugins .
```

The resulting image:
- Contains Fluent Bit and all built plugins
- Runs Fluent Bit by default with custom configuration
- Output plugins available in `/app/plugins/`

## Creating New Plugins

### 1. Create plugin file

Create a new C file in the appropriate directory:
- `src/plugins/input/your_plugin.c`
- `src/plugins/output/your_plugin.c`
- `src/plugins/filter/your_plugin.c`

### 2. Implement plugin interface

Use existing sample plugins as templates:

```c
#include <fluent-bit/flb_input.h>  // or flb_output.h, flb_filter.h

static int init(/* parameters */) {
    /* Initialize plugin */
    return 0;
}

static int cb_collect(/* parameters */) {
    /* Collect/process data */
    return 0;
}

static int cb_exit(void *data) {
    /* Cleanup */
    return 0;
}

struct flb_input_plugin input_plugin = {
    .name         = "your_plugin",
    .description  = "Your plugin description",
    .cb_init      = init,
    .cb_collect   = cb_collect,
    .cb_exit      = cb_exit,
};
```

### 3. Update CMakeLists.txt

Add plugin to the appropriate `CMakeLists.txt`:

```cmake
add_library(input_your_plugin MODULE your_plugin.c)
target_link_libraries(input_your_plugin ${FLUENT_BIT_LIBRARIES})
set_target_properties(input_your_plugin PROPERTIES PREFIX "")
```

### 4. Configure Fluent Bit

Edit `config/fluent-bit.conf` to use your plugin:

```ini
[PLUGINS]
    Path /path/to/your_plugin.so

[INPUT]
    Name your_plugin
    Tag your.tag
```

## Running

### Test locally

```bash
# Build plugins
./build.sh

# Run Fluent Bit with custom plugins
fluent-bit -c config/fluent-bit.conf
```

### Test with Docker

```bash
# Build image
docker build -t fluentbit-plugins .

# Run container
docker run -it fluentbit-plugins
```

## Plugin Types

### Input Plugins
- Collect data from sources
- Implement `cb_init`, `cb_collect`, `cb_exit`
- Schedule collection callbacks
- Push data to pipeline

### Output Plugins
- Send data to destinations
- Implement `cb_init`, `cb_flush`, `cb_exit`
- Handle batched events from pipeline

### Filter Plugins
- Process/transform log records
- Implement `cb_init`, `cb_filter`, `cb_exit`
- Modify records in-flight
- Important for data enrichment, masking, etc.

## Debugging

### Enable debug logging

```bash
fluent-bit -c config/fluent-bit.conf -d
```

Or set log level in `fluent-bit.conf`:
```ini
[SERVICE]
    Log_Level debug
```

### Check plugin load errors

```bash
fluent-bit -c config/fluent-bit.conf -vv
```

## Additional Resources

- [Fluent Bit Plugin Documentation](https://docs.fluentbit.io/manual/development/plugins-development)
- [Fluent Bit GitHub](https://github.com/fluent/fluent-bit)
- [MessagePack C API](https://github.com/msgpack/msgpack-c)

## Common Issues

### Plugin not loading

1. Check CMake output for compilation errors
2. Verify plugin path in configuration
3. Ensure plugin is built as `.so` (not `.a`)
4. Check Fluent Bit version compatibility

### Link errors

1. Verify `libfluent-bit-dev` is installed
2. Check `pkg-config` can find fluent-bit:
```bash
pkg-config --cflags --libs fluent-bit
```

### Runtime errors

1. Check plugin logs with `-vv` flag
2. Verify all Fluent Bit API calls are correct
3. Ensure proper memory management
