# blackhole: Introduction

**Table of Contents**

* [terminology](#terms)
* [dependencies](#deps)
* [build](#build)


<a name="terms"></a>
## terminology

* **blackhole directory**: a git repository, configured for use by
  blackhole, containing both `.blackhole` and `.git` directories.
* **configuration directory:** the actual `.blackhole` directory,
  usually located next to a `.git` directory.
* **origin**: the main remote repository, named as **origin**
  (in `git remote -v`).
* **remotes**: all remote repositories available to blackhole, excluding
  **origin**.


<a name="deps"></a>
## dependencies

The dependencies used in the project:

* [libgit2 v0.24.1][libgit2] (compiled with libssh2 support)
* [iniparser v4.0][iniparser]
* [clib][clib] (for installing other deps)


<a name="build"></a>
## build and install

We are using [CMake][cmake] to build the project
and [clib][clib] to manage some dependencies.
Prepare to build:

```bash
# Deps.
make deps

# Create build directory.
mkdir build && cd build

# Create cmake environment.
cmake ..
```

You can now run `make` commands during development:

```bash
# Install the `bh` binary at `${PREFIX}/bin/bh`.
make install

# Build.
make bh               # Build the main binary.
make blackhole        # Build the library.

# Run tests.
make run-tests

# Dependency-related tasks.
make build-iniparser  # iniparser
make build-libgit2    # libgit2

# Test-related tasks.
make gen-test-headers # Generate test headers.
make bh-tests         # Build the test binary.
make setup-tests      # Setup test data.
make clean-tests      # Clean up junk files from tests.
```

* `${PREFIX}` defaults to `/usr/local`
* `${TEST_FILTER}`: Set to filter specific tests.
* `${TEST_NO_NETWORK}`: Set to skip tests using network.

[clib]:https://github.com/clibs/clib
[cmake]:https://cmake.org/
[iniparser]:https://github.com/ndevilla/iniparser/releases/tag/v4.0
[libgit2]:https://github.com/libgit2/libgit2/releases/tag/v0.24.1
