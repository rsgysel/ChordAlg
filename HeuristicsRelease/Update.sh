#!/bin/bash

printf "\n"
echo ...Cleaning up source...
rm -f *.h
rm -f *.cpp
rm -f *.hpp
printf "\n"
ls -l

printf "\n"
echo ...Copying files...
# Copy classes (both .h and .cpp)
classes[0]="classic_elimination"
classes[1]="elimination_order"
classes[2]="file_reader"
classes[3]="graph"
classes[4]="intersection_graph"
classes[5]="lb_elimination"
classes[6]="lex_trie"
classes[7]="mcs"
classes[8]="mixed_elimination"
classes[9]="separator"
classes[10]="tree_representation"
classes[11]="vertex_utilities"

for filename in "${classes[@]}"
do
	cp ../include/${filename}.h .
	cp ../src/${filename}.cpp .
done

# Copy header-only (.h and .hpp)
headers[0]="atom_subgraphs.hpp"
headers[1]="chordalg_types.h"
headers[3]="utilities.hpp"

for filename in "${headers[@]}"
do
	cp ../include/${filename} .
done

# Copy executable src
executables[0]="CellIntersectionGraph"
executables[1]="Composite_heuristic"
executables[2]="MMC_heuristic"
executables[3]="Mixed_heuristic"
executables[4]="PDRS_ratio_heuristic"
executables[5]="PDRS_wsum_heuristic"
executables[6]="MMC_MaxCompatSupertree"

for filename in "${executables[@]}"
do
	printf "%s\n" "${filename}"
	cp ../exec/${filename}.cpp .
done

ls -al

printf "\n"
echo ...Testing compilation...
make

printf "\n"
echo ...Cleaning up...
rm -f *.o

for filename in "${heuristics[@]}"
do
	rm -f ${filename}
done

ls -al

printf "\n"
echo ...Done...
