cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

function(add_chordalg_functional_test arg1)
    add_executable(${arg1} ${arg1}.cxx)
    target_link_libraries(${arg1} ChordAlgLib)
endfunction()

add_chordalg_functional_test(elimination_heuristics)
add_chordalg_functional_test(minfill)
add_chordalg_functional_test(pig_cig_separator_graph)
add_chordalg_functional_test(potential_maxcliques)
add_chordalg_functional_test(separator_graph)
add_chordalg_functional_test(treewidth)
