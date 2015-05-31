cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

function(add_chordalg_sanitycheck arg1)
    add_executable(${arg1} ${arg1}.cxx)
    target_link_libraries(${arg1} ChordAlgLib)
endfunction()

add_chordalg_sanitycheck(elimination_heuristics)
add_chordalg_sanitycheck(minfill)
add_chordalg_sanitycheck(potential_maxcliques)
add_chordalg_sanitycheck(separator_graph)
add_chordalg_sanitycheck(treewidth)
