cmake_minimum_required(VERSION 4.2)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

project(OS_ARM)

enable_language(ASM)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_COMPILER aarch64-none-elf-c++)
set(CMAKE_C_COMPILER aarch64-none-elf-gcc)
set(CMAKE_ASM_COMPILER aarch64-none-elf-gcc)
set(CMAKE_AR aarch64-none-elf-ar)
set(CMAKE_RANLIB aarch64-none-elf-ranlib)
set(CMAKE_OBJCOPY aarch64-none-elf-objcopy) 
set(CMAKE_OBJDUMP aarch64-none-elf-objdump) 

set(COMMON_FLAGS "-march=armv8-a -ffreestanding -nostdlib -fno-exceptions")
set(CMAKE_CXX_FLAGS "${COMMON_FLAGS} -std=c++17")
set(CMAKE_C_FLAGS "${COMMON_FLAGS} -std=gnu99")
set(CMAKE_ASM_FLAGS "${COMMON_FLAGS}")

if(NOT DEFINED ARCH)
  set(ARCH arm64)
endif()

if(NOT DEFINED TARGET_PLATFORM)
  set(TARGET_PLATFORM qemu-virt)
endif()

set(CORE_LIB core)
add_library(${CORE_LIB})
add_subdirectory(core)
add_subdirectory(sys)
add_subdirectory(mem)
add_subdirectory(drivers)

function(add_platform _ARCH _PLAT)
  set(KERNEL_EXEC kernel)
  add_executable(${KERNEL_EXEC})
  set_target_properties(${KERNEL_EXEC} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY
      ${CMAKE_BINARY_DIR}/${_ARCH}/${_PLAT}
  )

  add_subdirectory(arch)
  add_subdirectory(lib)
endfunction()

add_platform(${ARCH} ${TARGET_PLATFORM})
