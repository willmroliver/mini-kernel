target_sources(${CORE_LIB}
  PRIVATE
    common/endian.s
)

add_subdirectory(${ARM_VERSION_PROFILE})
add_subdirectory(platform/${ARM_PLATFORM})
