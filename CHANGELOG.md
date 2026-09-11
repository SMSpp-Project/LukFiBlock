# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added

### Changed

### Fixed

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

- updated long-neglected code

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
