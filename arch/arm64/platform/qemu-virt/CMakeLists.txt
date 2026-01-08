set(LOAD_ADDRESS 0x40000000)
set(LOAD_OFFSET 0x200000)

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
    -Wl,-defsym,LOAD_OFFSET=${LOAD_OFFSET}
)

set_target_properties(${KERNEL_EXEC} PROPERTIES
  LINK_DEPENDS ${LINKER_SCRIPT}
)
