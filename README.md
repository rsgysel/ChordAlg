ChordAlg
========
ChordAlg is a library of chordal-graph / minimal-triangulation algorithms,
geared towards phylogenetic character compatibility analysis.

Requirements
============
Google Mock
    code.google.com/p/googlemock/

ChordAlg expects GoogleMock to be in /usr/src/gmock
You can specify your GoogleMock directory by editing
    ChordAlg/cmake/Modules/gmock.cmake

Installation
============
1) In the root directory, make a `build' directory.

2) In build, type
    ChordAlg/build$ cmake -G "Unix Makefiles" ..

3) Type
    ChordAlg/build$ make

4) Executables will be in
    ChordAlg/build/executables
    ChordAlg/build/heuristic_experiments
    ChordAlg/build/separatorgraph_experiments
