target_sources(${CORE_LIB}
  PUBLIC FILE_SET HEADERS
  BASE_DIRS ${CMAKE_SOURCE_DIR}
    FILES
      sys.h
  PRIVATE 
    exception.c
)

target_include_directories(${CORE_LIB}
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)
