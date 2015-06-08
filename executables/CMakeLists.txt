cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

add_library(ChordAlgExec chordalg_options)

function(add_chordalg_executable arg1)
    add_executable(${arg1} ${arg1}.cxx)
    target_link_libraries(${arg1} ChordAlgLib)
    target_link_libraries(${arg1} ChordAlgExec)
endfunction()

# Formats
add_chordalg_executable(DOTFormat)
add_chordalg_executable(GMLFormat)

# Chordal Graphs
add_chordalg_executable(BTMinimumFill)
add_chordalg_executable(BTTreewidth)
add_chordalg_executable(ChordalGraph)
add_chordalg_executable(CliqueTree)
add_chordalg_executable(FillEliminationOrder)
add_chordalg_executable(LBTriang)
add_chordalg_executable(MCS)
add_chordalg_executable(MinimalTriangulation)
add_chordalg_executable(PerfectEliminationOrder)
add_chordalg_executable(PotentialMaxcliques)
add_chordalg_executable(SeparatorGraph)

# Graph Computations
add_chordalg_executable(Atoms)

# Phylogenetic Character Compatibility
add_chordalg_executable(CellIntersectionGraph)
add_chordalg_executable(PartitionIntersectionGraph)
