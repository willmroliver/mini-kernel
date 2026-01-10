set(LOAD_ADDRESS 0x40000000)

target_sources(${KERNEL_EXEC}
  PRIVATE
    boot.c
    boot.S
)

set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/linker.ld)

target_link_options(${KERNEL_EXEC} 
  PUBLIC 
  -T ${LINKER_SCRIPT}
    -Wl,-defsym,LOAD_ADDRESS=${LOAD_ADDRESS}
)

set_target_properties(${KERNEL_EXEC} PROPERTIES
  LINK_DEPENDS ${LINKER_SCRIPT}
)
