add_library(${DRIVERS_LIB} STATIC)

target_sources(${DRIVERS_LIB}
  PUBLIC FILE_SET HEADERS
    BASE_DIRS ${CMAKE_SOURCE_DIR}
    FILES
      uart-pl011.h
  PRIVATE
    uart-pl011.c
)

target_include_directories(${DRIVERS_LIB} 
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${DRIVERS_LIB} ${CORE_LIB})

