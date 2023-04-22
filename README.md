# Global Container Format reference implementation

This is the reference implementation for the [GCF](https://github.com:global-container-format/spec.git) specification.

To build the library the following is required:

* CMake
* C compiler (preferred is clang)
* Conan

## Preparing the build system

The dependencies of this library are managed via Conan. To install these, run:

```bash
conan install --build=missing --deploy=full_deploy -s build_type=Debug .
```

To prepare the build, run:

```bash
cmake --preset conan-debug -S . -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=ON
```

This will initialize the build system and export the compile commands (if supported).

To prepare the build for testing, you may want to ensure the proper features are enabled in *features.cmake*, then eventually initialise a virtual environment and install the required dependencies. If a virtual environment is enabled, it must be activated. These steps must be taken **before** runnig the prepare build command above, like:

```bash

# Install dependencies
pipenv install -d

# Activate environment
pipenv shell

# Now you can create the build system
```

## Running the build

The following interesting build targets are available:

* all: build the library
* test: run the tests
* coverage: generate the code coverage report
* clean: clean the build environment

To build the library, run:

```bash
cmake --build --preset conan-debug
```

### Building a shared library

To build a shared library version of *libgcf*, define the boolean CMake variable `GCF_BUILD_DLL` and set it to `ON`.

## Testing

With the virtual environment active, initialize the build by running `cmake --preset conan-debug` in the main project directory.

```bash
ctest --output-on-failure --preset conan-debug
```

To run the tests and generate a coverage report, run:

```bash
cmake --build --preset conan-debug --target coverage
```

**NOTE**: The *coverage* target is only available when the corresponding feature is enabled in the `features.cmake` file.

## Installing

To install the library, run:

```bash
cmake --install YOUR_BUILD_DIR --prefix INSTALLATION_PREFIX_PATH
```
