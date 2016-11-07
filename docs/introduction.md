# blackhole: Introduction

> Introduction to Blackhole

**Table of Contents**

* [terminology](#terms)
* [dependencies](#deps)
* [build](#build)
* [setup](#setup)


<a name="terms"></a>
## terminology:

* **blackhole directory**: a git repository, configured for use by
  blackhole, containing a `.blackhole` directory
* **origin**: the main remote repository, named as **origin**
  (in `git remote -v`)
* **remotes**: all remote repositories available to blackhole, excluding
  **origin**


<a name="deps"></a>
## dependencies:

The dependencies used in the project:

* [libgit2 v0.24.1][libgit2] (compiled with libssh2 support)
* [iniparser v4.0][iniparser]
* [clib][clib] (for installing other deps)

[iniparser]:https://github.com/ndevilla/iniparser/releases/tag/v4.0
[libgit2]:https://github.com/libgit2/libgit2/releases/tag/v0.24.1
[clib]:https://github.com/clib/clib


<a name="build"></a>
## build and install:

We are using [CMake][cmake] to build the project:

1. Ensure you have installed the dependencies [above](#deps) and that they
   are discovarable to `cmake`.
   * **Note**: `make pc`: places the pkg-config config files in `misc/` i.e.
   `misc/*.pc` in `/usr/local/lib/pkgconfig/`.
1. `clib install`
1. `mkdir build`
1. `cmake ..`
1. `make install`

On a successful build, the executable will be placed at `${PREFIX}/bin/bh`.
`${PREFIX}` defaults to `/usr/local`.

Run `bh --help` to ensure it is working fine.

[cmake]:https://cmake.org/


<a name="setup"></a>
## setup:

To setup a blackhole directory:

1. `git init` a repository
1. `mkdir .blackhole`
1. `touch .blackhole/config.ini`
1. `git remote add origin <Git-URL using SSH e.g. git@github.com:YourUsername/blackhole>`
1. [optional] `git remote add redudant-backup <Git-URL using SSH>`

**Note**: blackhole expects to find the origin for pulling changes.
