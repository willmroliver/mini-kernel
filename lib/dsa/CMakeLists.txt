target_sources(${CORE_LIB}
  PUBLIC FILE_SET HEADERS
    BASE_DIRS ${CMAKE_SOURCE_DIR}
    FILES
      ring.h
  PRIVATE
    ring.c
)

target_include_directories(${CORE_LIB}
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
