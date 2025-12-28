add_executable(${BOOT_EXEC} 
  ${MACHINE}/boot.s
  boot.c
)

add_subdirectory(${MACHINE})

target_link_options(${BOOT_EXEC} 
  PUBLIC 
    -T ${CMAKE_CURRENT_SOURCE_DIR}/linker.ld
    -Wl,-defsym,LOAD_ADDRESS=${LOAD_ADDRESS}
)

target_link_libraries(${BOOT_EXEC} 
  PRIVATE ${CORE_LIB} ${DRIVERS_LIB}
)
