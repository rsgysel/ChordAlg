cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

function(add_chordalg_executable arg1)
    add_executable(${arg1} ${arg1}.cxx)
    target_link_libraries(${arg1} ChordAlgLib)
endfunction()

add_chordalg_executable(Atoms)
add_chordalg_executable(CellIntersectionGraph)
add_chordalg_executable(MCS_triangulation)
#add_chordalg_executable(MaxCompatSupertree)