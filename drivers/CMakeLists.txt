target_sources(${CORE_LIB}
  PUBLIC FILE_SET HEADERS
    BASE_DIRS ${CMAKE_SOURCE_DIR}
    FILES
      arm/gic-v2.h
      arm/uart-pl011.h
  PRIVATE
    arm/gic-v2.c
    arm/uart-pl011.c
)

target_include_directories(${CORE_LIB} 
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)
