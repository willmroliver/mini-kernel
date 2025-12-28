target_include_directories(${CORE_LIB}
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/${ARCH}
)

target_sources(${CORE_LIB}
  PRIVATE
    ${ARCH}/asm/endian.s
)
