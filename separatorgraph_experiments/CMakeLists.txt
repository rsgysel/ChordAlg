cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

function(add_separatorgraph_experiment arg1)
    add_executable(${arg1} ${arg1}.cxx)
    target_link_libraries(${arg1} ChordAlgLib)
endfunction()

add_separatorgraph_experiment(AtomsExperiment)
add_separatorgraph_experiment(AtomSeparatorGraphExperiment)
add_separatorgraph_experiment(SeparatorGraphExperiment)
