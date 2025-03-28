# Advent of Code 2017

URL: https://adventofcode.com/2017

## Prerequisites

* [Clang](https://clang.llvm.org/) (confirmed to work with Clang 19.1.1)
* [libc++](https://libcxx.llvm.org/) (confirmed to work with libc++ 19.1.1)
* [CMake](https://cmake.org/) (confimed to work with CMake 3.30.1)
* [Ninja](https://ninja-build.org/) (confimed to work with Ninja 1.12.1)

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
```console
$ mkdir build
$ env CXX=/usr/bin/clang++ cmake -S . -B ./build -G Ninja
$ cmake --build ./build
```

## How to use

### Place puzzle input data files into each resource folder in advance

The default file name of the puzzle input data is `input`:

```console
$ ls ./inputs/*/input
./inputs/01/input  ./inputs/02/input
./inputs/03/input  ./inputs/04/input
  ...
./inputs/23/input  ./inputs/24/input
./inputs/25/input
$
```

## Run solver
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

## Note

There are no input data files in this repository.
Please get them from the AoC 2017 site.

See [here](https://adventofcode.com/about#faq_copying) for the reasons.
