# voronoi-diagram-construction
Implementations of the Fortune's algorithm for the construction of Voronoi diagrams.

- The fortune_points folder contains the implementation of the algorithm to the
  normal version of the diagram.

- The folder fortune_weighted_points folder contains the implementation of the
  algorithm to the weighted points version o the diagram.

- Compiles on: g++ 7.0.1

Unordered to-do list:
* Translate all the comments to english.
* References to the papers i've read to make this project.
* Make a suit of test to make code refactoring.
* Makefile.

To compile the implementation for the main variant of the diagram, inside the fortune_points folder make:
```bash
g++ -std=c++11 -I . fortune_points/geom/* fortune_points/algorithm/* fortune_points/diagram/* -o builder
```
Command to compile the tests(you have to include the files inside the folder\__tests__/\*/\* that you want to compile):

```bash
g++ -std=c++11  -I . fortune_points/geom/* fortune_points/__tests__/main.cpp gtest/libgtest.a -pthread -o fortune-points-tests
```

To compile the weighted version check fortune_weighted_points folder.
