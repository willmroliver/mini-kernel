add_library(${CORE_LIB})

target_sources(${CORE_LIB}
  PUBLIC FILE_SET HEADERS
    BASE_DIRS ${CMAKE_SOURCE_DIR}
    FILES 
      endian.h
      strings.h 
      types.h 
  PRIVATE 
    endian.c
    strings.c
)

target_include_directories(${CORE_LIB} 
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)
