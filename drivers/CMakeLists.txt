target_sources(${CORE_LIB}
  PUBLIC FILE_SET HEADERS
    BASE_DIRS ${CMAKE_SOURCE_DIR}
    FILES
      arm/gic.h
      arm/uart-pl011.h
  PRIVATE
    arm/gic.c
    arm/uart-pl011.c
)

target_include_directories(${CORE_LIB} 
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)
