# Global Container Format reference implementation

This is the reference implementation for the [GCF](https://github.com:global-container-format/spec.git) specification.

To build the library the following is required:

* CMake
* Build tools (ninja is preferred)
* C compiler (preferred is clang)

## Preparing the build system

To prepare the build, run:

```bash
cmake -S . -B build -G Ninja -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=ON
```

This will initialize the build system and export the compile commands (if supported).

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
