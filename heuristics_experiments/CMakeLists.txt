cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

add_library(HeuristicsLib heuristic_options.cpp)

function(add_heuristic_experiment arg1)
    add_executable(${arg1} ${arg1}.cxx)
    target_link_libraries(${arg1} ChordAlgLib)
    target_link_libraries(${arg1} HeuristicsLib)
endfunction()

add_heuristic_experiment(Composite_heuristic)
add_heuristic_experiment(Mixed_heuristic)
add_heuristic_experiment(MMC_heuristic)
add_heuristic_experiment(PDRS_ratio_heuristic)
add_heuristic_experiment(PDRS_wsum_heuristic)
add_heuristic_experiment(sanity_check)
