target_include_directories(${CORE_LIB}
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/${ARCH}
)

add_subdirectory(${ARCH})
