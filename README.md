# Global Container Format reference implementation

This is the reference implementation for the [GCF](https://github.com:global-container-format/spec.git) specification.

To build the library the following is required:

* CMake
* Build tools (ninja is preferred)
* C compiler (preferred is clang)
* Z-lib
* Python (optional, required to build test resources)
* gcfconv (optional, required to build test resources)

## Preparing the build system

To prepare the build, run:

```bash
cmake -S . -B build -G Ninja -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=ON
```

This will initialize the build system and export the compile commands (if supported).

To prepare the build for testing, you may want to ensure the proper features are enabled in *features.cmake*, then eventually initialise a virtual environment and install *gcfconv*. If a virtual environment is enabled, it must be activated. These steps must be taken **before** runnig the prepare build command above, like:

```bash
${GCFCONV_PATH}/venv/scripts/activate && cmake -S . -B build -G Ninja -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=ON
```

## Running the build

The following interesting build targets are available:

* all: build the library
* test: run the tests
* coverage: generate the code coverage report
* clean: clean the build environment

To build the library, run:

```bash
cmake --build build
```

To run the tests:

```bash
ctest --test-dir build
```

To run the tests and generate a coverage report, run:

```bash
cmake --build build --target coverage
```

**NOTE**: The *coverage* target is only available when the corresponding feature is enabled in the `features.cmake` file.

### Building a shared library

To build a shared library version of *libgcf*, define the boolean CMake variable `GCF_BUILD_DLL` and set it to `ON`.

## Testing

To run the tests, the following is required:

1. A Python 3 installation (or active virtual environment).
2. The *gcfconv* Python package installed.

With the environment active, initialize the build by running `cmake -S . -B build -G Ninja` in the main project directory.
If everything is configured correctly, no warnings will be shown.

With this last step successful, you can run the build and then the tests.
