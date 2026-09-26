# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added

- a tester of the module, `test/`, that needs the core alone: each of the 26
  functions is loaded from a stream and checked for its dimension, its value
  at the starting point and at the optimum of the literature, and its
  linearization at random points, against finite differences, against the
  function itself when it is convex, and between the `Range` and the
  `Subset` forms, together with the arguments `load()` refuses, the kinks, a
  change of the variables and of the parameters seen by `compute()`, and the
  netCDF round trip. The pipeline of the module builds the module alone and
  runs this tester, the suite that minimizes the functions with the
  BundleSolver running where that module is

### Changed

- whoever links the module keeps it: the classes of a module register
  themselves in the factory from a static initialiser, and a linker that
  drops what looks unused takes the registration away with it, so the target
  now tells whoever links it to keep the symbol that forces the module in,
  and on ELF, where naming the symbol is not enough, the library as a whole

### Fixed

- the subgradients of three functions are those of the function computed:
  Wolfe has 9 and not 9 x_0 as the derivative in x_0 of 9 x_0 + 16 |x_1|,
  and at the origin gives ( 15 , 0 ) rather than ( 0 , 0 ), which is no
  subgradient there, or 0 / 0 in the `Subset` form; Colville1 subtracts the
  rows of A of its penalty 50 max( 0 , b - A x ) instead of adding them; Gill
  has the derivative of ( x_i^2 - 0.25 )^2 in its first piece, the sum over
  the 29 points of the chain rule in its second one, and in its third one no
  longer reads past the last variable

- the subgradient of MaxQR is b_j ( x - c_j ) / || x - c_j || of the active
  piece b_j || x - c_j || + a_j, and 0 at x = c_j, instead of 2 b_j ( x - c_j ),
  the gradient of the square of the norm; its data in ( - 0.5 , 0.5 ] are
  drawn as 0.5 - u, which gives the same numbers as before without the
  distribution with the bounds swapped, whose behaviour is undefined

- `LukFiFunction::get_int_par()` returns the current value of a parameter of
  the base class, not its default

- the documentation of Lewis says what the function is: nonconvex and
  unbounded below, the origin being a stationary point and not a minimum

- `load()` refuses a function beyond the 26 there are, which it accepted up
  to 29 as a function valued 0

- the netCDF round trip of a `LukFiBlock` works: `serialize()` writes the
  function, which `deserialize()` looks for, and writes no ComputeConfig when
  all the parameters are at their default, where it dereferenced a null
  pointer, while `deserialize()` builds the Variable and the Objective of the
  Block as `load()` does

- `makefile-c` includes the makefile of the core from where it is

- on macOS a program linking the module lost the classes the module
  registers in the factories when the linker dropped the library, as it
  does under `-dead_strip_dylibs`, which conda sets: the target now asks the
  linker for the symbol that forces the module in (`-u`), which ld64,
  unlike the ELF linker, counts as a use of the library

- the starting point of `Maxq` and `Maxl` has `x_i = - i - 1` computed in
  double, the subtraction of an unsigned index from zero having given a very
  large number instead of a negative one

- `LukFiFunction::compute()` returns a status, as the interface asks, and not
  the value of the function, which is read with `get_value()` as everywhere
  else
## [0.4.0] - 2026-09-12

### Changed

- the version of the module is the git tag of its repository, or the
  VERSION.txt of a release tarball, and the shared library carries it: its
  SONAME is major.minor while the major is 0, and it is installed with an
  RPATH relative to itself, so that an installed tree keeps working wherever
  it is moved

### Fixed

- the global structures of the test functions are static, so that the library
  links on macOS with no duplicate symbol

## [0.3.2] - 2024-02-28

### Changed

- adapted to new CMake / makefile organisation

## [0.3.1] - 2023-05-23

### Changed

- adapted to new load/print interface

### Fixed

- updated long-neglected code to current testing and interface
  standards

## [0.3.0] - 2021-02-05

### Added

- Maintenance release.

## [0.2.0] - 2020-03-06

### Added

- Changelog.

## [0.1.0] - 2019-11-29

### Added

- First test release.

[Unreleased]: https://gitlab.com/smspp/lukfiblock/-/compare/0.4.0...develop
[0.4.0]: https://gitlab.com/smspp/lukfiblock/-/compare/0.3.2...0.4.0
[0.3.2]: https://gitlab.com/smspp/lukfiblock/-/compare/0.3.1...0.3.2
[0.3.1]: https://gitlab.com/smspp/lukfiblock/-/compare/0.3.0...0.3.1
[0.3.0]: https://gitlab.com/smspp/lukfiblock/-/compare/0.2.0...0.3.0
[0.2.0]: https://gitlab.com/smspp/lukfiblock/-/compare/0.1.0...0.2.0
[0.1.0]: https://gitlab.com/smspp/lukfiblock/-/tags/0.1.0
