# Advent of Code 2017

URL: https://adventofcode.com/2017

## Prerequisites

* [Clang](https://clang.llvm.org/) (confirmed to work with Clang 19.1.1)
  - I used C++20 private module fragment this time, however, [GCC 14 didn't support it yet](https://gcc.gnu.org/onlinedocs/gcc-14.2.0/gcc/C_002b_002b-Modules.html).
* C++ Standard Library (one of the following)
  - [libc++](https://libcxx.llvm.org/) (confirmed to work with libc++ 19.1.1)
  - [libstdc++](https://gcc.gnu.org/onlinedocs/libstdc++/) (confirmed to work with the version included in GCC 14.2)
* [CMake](https://cmake.org/) (confimed to work with CMake 3.30.1)
* [Ninja](https://ninja-build.org/) (confimed to work with Ninja 1.12.1)
* [GoogleTest](https://github.com/google/googletest) (confimed to work with GoogleTest v1.16.0)

## Solutions

* [Day 1: Inverse Captcha](./src/day01.cpp)
* [Day 2: Corruption Checksum](./src/day02.cpp)
* [Day 3: Spiral Memory](./src/day03.cpp)
* [Day 4: High-Entropy Passphrases](./src/day04.cpp)
* [Day 5: A Maze of Twisty Trampolines, All Alike](./src/day05.cpp)
* [Day 6: Memory Reallocation](./src/day06.cpp)
* [Day 7: Recursive Circus](./src/day07.cpp)
* [Day 8: I Heard You Like Registers](./src/day08.cpp)
* [Day 9: Stream Processing](./src/day09.cpp)
* [Day 10: Knot Hash](./src/day10.cpp)
* [Day 11: Hex Ed](./src/day11.cpp)
* [Day 12: Digital Plumber](./src/day12.cpp)
* [Day 13: Packet Scanners](./src/day13.cpp)
* [Day 14: Disk Defragmentation](./src/day14.cpp)
* [Day 15: Dueling Generators](./src/day15.cpp)
* [Day 16: Permutation Promenade](./src/day16.cpp)
* [Day 17: Spinlock](./src/day17.cpp)
* [Day 18: Duet](./src/day18.cpp)
* [Day 19: A Series of Tubes](./src/day19.cpp)
* [Day 20: Particle Swarm](./src/day20.cpp)
* [Day 21: Fractal Art](./src/day21.cpp)
* [Day 22: Sporifica Virus](./src/day22.cpp)
* [Day 23: Coprocessor Conflagration](./src/day23.cpp)
* [Day 24: Electromagnetic Moat](./src/day24.cpp)
* [Day 25: The Halting Problem](./src/day25.cpp)

## How to build
If you want to use libstdc++ instead of libc++, please modify the CMakeLists.txt in the top level of source tree.

```console
$ cmake --preset AoC2017
$ cmake --build ./build
```

## How to use

### Place puzzle input data files into each resource folder in advance

The default file name of the puzzle input data is `input`.

```console
$ ls ./inputs/*/input
./inputs/01/input  ./inputs/02/input
./inputs/03/input  ./inputs/04/input
  ...
./inputs/23/input  ./inputs/24/input
./inputs/25/input
$
```

### Run solver
If you want to run the solver for Day 1, please do the following:

```console
$ sh solve.sh 1
```

The output will be as follows.

```console
[Day 1]
****
****
Elapsed time: #.###ms
```

You can also run all solutions at once.

```console
$ ./build/src/aoc2017
```

### Unit test
If you want to run unit tests:

```console
$ ctest --test-dir ./build
```

Please read the [./test/README.md](./test/README.md) in advance. You ought to need to modify unit test files.

## Note

There are no input data files in this repository.
Please get them from the AoC 2017 site.

See [here](https://adventofcode.com/about#faq_copying) for the reasons.
