target_sources(${CORE_LIB}
  PUBLIC FILE_SET HEADERS
    BASE_DIRS ${CMAKE_SOURCE_DIR}
    FILES 
      endian.h
      mem.h
      types.h 
  PRIVATE 
    mem.c
)

target_include_directories(${CORE_LIB} 
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)
