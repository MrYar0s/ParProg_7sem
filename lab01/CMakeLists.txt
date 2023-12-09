add_executable(lab01_first_cycle first.c)

target_include_directories(lab01_first_cycle PRIVATE "${MPI_C_INCLUDE_PATH}")
target_link_libraries(lab01_first_cycle PRIVATE "${MPI_C_LIBRARIES}" m)

add_custom_target(lab01_first_seq
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/lab01_first_cycle sequence ${OUTPUT_LAB01_DIR}/first_seq.txt
    DEPENDS lab01_first_cycle ${OUTPUT_LAB01_DIR}
)

add_custom_target(lab01_first_parallel
    COMMAND mpirun -np 7 parallel ${OUTPUT_LAB01_DIR}/first_par.txt
    DEPENDS lab01_first_cycle ${OUTPUT_LAB01_DIR}
)
