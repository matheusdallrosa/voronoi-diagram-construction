# voronoi-diagram-construction
Implementations of the Fortune's algorithm for the construction of Voronoi diagrams.

- The fortune_points folder contains the implementation of the algorithm to the
  normal version of the diagram.

- The folder fortune_weighted_points folder contains the implementation of the
  algorithm to the weighted points version of the diagram.

- Compiles on: g++ 7.0.1

Unordered to-do list:
* Translate all the comments and debug messages to english.
* Make a suit of test to make code refactoring.
* Makefile.

# Fortune's algorithm
The Fortune's algorithm implemented on this project refers to the algorithm published on:
[Article](https://link.springer.com/article/10.1007/BF01840357).

# Unweighted Voronoi diagram
Given a set S with N points on the plane, called sites, the Voronoi diagram is a partition
of the plane into N regions, one region for each site. The Voronoi diagram is very often
called a planar graph. See more at: [Voronoi diagram wiki article](https://en.wikipedia.org/wiki/Voronoi_diagram).

To compile the implementation to build the main variant of the diagram, use the following command:
```bash
g++ -std=c++11 -I . fortune_points/geom/* fortune_points/algorithm/* fortune_points/diagram/* -o builder
```
Command to compile the tests(you have to include the files from the folder: fortune_points/\__tests__/\*/\* that you want to compile/test):

```bash
g++ -std=c++11  -I . fortune_points/geom/* fortune_points/__tests__/main.cpp gtest/libgtest.a -pthread -o fortune-points-tests
```

Usage example at: fortune_points/example0.cpp

# Additively Weighted Voronoi diagram
On this variant of the diagram every site has a non-negative weight associated.
The distance between a site P and a point Q on the plane is given by:
euclidean_distance(P,Q) + Pw, where Pw is the weight of P.

The ideas behind the code on fortune_weighted_points/geom/appollonius_circle.* ,
come from this [article](http://www.sciencedirect.com/science/article/pii/S0010448505001016).

To compile this implementation use the following command:
```bash
g++ -std=c++11 -o builder -I . fortune_weighted_points/geom/* fortune_weighted_points/algorithm/* fortune_weighted_points/diagram/* fortune_weighted_points/common/* fortune_weighted_points/*.cpp
```

Note: The implementation of the Fortune's algorithm doesn't work for the cases
where:
- There is a site dominating other sites.
- There are two sites P and Q, such that Py + Pw = Qy + Qw. Pw and Qw are the weight
  of the sites P and Q, respectively.
