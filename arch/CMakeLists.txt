target_include_directories(${CORE_LIB}
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/${ARCH}
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(${ARCH})
