COMPILER = g++
OPTFLAGS = -O3 -std=c++0x
OBJ = elimination_order.o file_reader.o graph.o lex_trie.o intersection_graph.o separator.o vertex_utilities.o

all:
	make classic_elimination.o
	make elimination_order.o
	make file_reader.o
	make graph.o
	make intersection_graph.o
	make lb_elimination.o
	make lex_trie.o
	make mixed_elimination.o
	make separator.o
	make vertex_utilities.o
	make CellIntersectionGraph
	make Composite_heuristic
	make MMC_heuristic
	make Mixed_heuristic
	make PDRS_ratio_heuristic
	make PDRS_wsum_heuristic

CellIntersectionGraph: CellIntersectionGraph.cpp file_reader.h intersection_graph.h
	${COMPILER} ${OPTFLAGS} CellIntersectionGraph.cpp ${OBJ} -o CellIntersectionGraph

Composite_heuristic: Composite_heuristic.cpp atom_subgraphs.hpp classic_elimination.o file_reader.o intersection_graph.o lb_elimination.o
	${COMPILER} ${OPTFLAGS} Composite_heuristic.cpp classic_elimination.o lb_elimination.o ${OBJ} -o Composite_heuristic

Mixed_heuristic: Mixed_heuristic.cpp atom_subgraphs.hpp mixed_elimination.o file_reader.o intersection_graph.o
	${COMPILER} ${OPTFLAGS} Mixed_heuristic.cpp lb_elimination.o mixed_elimination.o ${OBJ} -o Mixed_heuristic

MMC_heuristic: MMC_heuristic.cpp atom_subgraphs.hpp classic_elimination.o file_reader.o intersection_graph.o
	${COMPILER} ${OPTFLAGS} MMC_heuristic.cpp classic_elimination.o ${OBJ} -o MMC_heuristic

PDRS_ratio_heuristic: PDRS_ratio_heuristic.cpp atom_subgraphs.hpp lb_elimination.o file_reader.o intersection_graph.o
	${COMPILER} ${OPTFLAGS} PDRS_ratio_heuristic.cpp lb_elimination.o ${OBJ} -o PDRS_ratio_heuristic

PDRS_wsum_heuristic: PDRS_wsum_heuristic.cpp atom_subgraphs.hpp lb_elimination.o file_reader.o intersection_graph.o
	${COMPILER} ${OPTFLAGS} PDRS_wsum_heuristic.cpp lb_elimination.o ${OBJ} -o PDRS_wsum_heuristic

classic_elimination.o: classic_elimination.h classic_elimination.cpp elimination_order.h intersection_graph.h vertex_utilities.h
	${COMPILER} ${OPTFLAGS} -c classic_elimination.cpp

elimination_order.o: elimination_order.h elimination_order.cpp atom_subgraphs.hpp file_reader.h graph.h vertex_utilities.h
	${COMPILER} ${OPTFLAGS} -c elimination_order.cpp

file_reader.o: file_reader.h file_reader.cpp lex_trie.h
	${COMPILER} ${OPTFLAGS} -c file_reader.cpp

graph.o: graph.h graph.cpp file_reader.o vertex_utilities.h
	${COMPILER} ${OPTFLAGS} -c graph.cpp

intersection_graph.o: intersection_graph.h intersection_graph.cpp graph.h lex_trie.h vertex_utilities.h
	${COMPILER} ${OPTFLAGS} -c intersection_graph.cpp

lb_elimination.o: lb_elimination.h lb_elimination.cpp elimination_order.h intersection_graph.h separator.h vertex_utilities.h
	${COMPILER} ${OPTFLAGS} -c lb_elimination.cpp

lex_trie.o: lex_trie.h lex_trie.cpp
	${COMPILER} ${OPTFLAGS} -c lex_trie.cpp

mixed_elimination.o: mixed_elimination.h mixed_elimination.cpp classic_elimination.h lb_elimination.h intersection_graph.h vertex_utilities.h
	${COMPILER} ${OPTFLAGS} -c mixed_elimination.cpp

separator.o: separator.h separator.cpp graph.h vertex_utilities.h
	${COMPILER} ${OPTFLAGS} -c separator.cpp

vertex_utilities.o: vertex_utilities.h vertex_utilities.cpp
	${COMPILER} ${OPTFLAGS} -c vertex_utilities.cpp

clean:
	rm -f *.o
	rm -f *.exe
	make all

