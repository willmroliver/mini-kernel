add_library(devicetree STATIC)

target_sources(devicetree 
  PUBLIC FILE_SET HEADERS
    BASE_DIRS ${CMAKE_SOURCE_DIR}/lib
    FILES 
      fdt.h
  PRIVATE
    fdt.c
)

target_include_directories(devicetree
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(devicetree ${CORE_LIB})

