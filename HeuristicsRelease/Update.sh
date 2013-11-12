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
classes[0]="classic_elimination"
classes[1]="elimination_order"
classes[2]="file_reader"
classes[3]="graph"
classes[4]="intersection_graph"
classes[5]="lb_elimination"
classes[6]="lex_trie"
classes[7]="mixed_elimination"
classes[8]="separator"
classes[9]="vertex_utilities"

for filename in "${classes[@]}"
do
	cp ../include/${filename}.h .
	cp ../src/${filename}.cpp .
done

# Because we have .h and .hpp, add the entire filename here
headers[0]="atom_subgraphs.hpp"
headers[1]="chordalg_types.h"
headers[2]="graph_types.h"
headers[3]="utilities.h"
headers[4]="utilities.hpp"

for filename in "${headers[@]}"
do
	cp ../include/${filename} .
done

executables[0]="CellIntersectionGraph"
executables[1]="Composite_heuristic"
executables[2]="MMC_heuristic"
executables[3]="Mixed_heuristic"
executables[4]="PDRS_ratio_heuristic"
executables[5]="PDRS_wsum_heuristic"

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
