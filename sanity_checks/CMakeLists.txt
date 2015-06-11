cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

function(add_chordalg_sanity_check arg1)
    add_executable(${arg1} ${arg1}.cxx)
    target_link_libraries(${arg1} ChordAlgLib)
endfunction()

add_chordalg_sanity_check(elimination_heuristics)
add_chordalg_sanity_check(lbtriang)
add_chordalg_sanity_check(minfill)
add_chordalg_sanity_check(pig_cig_separator_graph)
add_chordalg_sanity_check(potential_maxcliques)
add_chordalg_sanity_check(separator_graph)
add_chordalg_sanity_check(treewidth)
