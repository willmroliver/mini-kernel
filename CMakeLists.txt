cmake_minimum_required(VERSION 4.2)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

project(OS_ARM)

enable_language(ASM)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT DEFINED ARCH)
  set(ARCH arm64)
endif()

if(${ARCH} STREQUAL arm64)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=armv8-a")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=armv8-a")
  set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -march=armv8-a")

  set(CMAKE_CXX_COMPILER aarch64-none-elf-c++)
  set(CMAKE_C_COMPILER aarch64-none-elf-gcc)
  set(CMAKE_ASM_COMPILER aarch64-none-elf-gcc)
  set(CMAKE_AR aarch64-none-elf-ar)
  set(CMAKE_RANLIB aarch64-none-elf-ranlib)
  set(CMAKE_OBJCOPY aarch64-none-elf-objcopy) 
  set(CMAKE_OBJDUMP aarch64-none-elf-objdump) 
endif()

if(NOT DEFINED TARGET_PLATFORM)
  set(TARGET_PLATFORM qemu-virt)
endif()

function(set_standards _TARGET)
  set_target_properties(${_TARGET} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${ARCH}/${TARGET_PLATFORM}
    C_STANDARD 99
    C_STANDARD_REQUIRED YES
    C_EXTENSIONS ON
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
  )
endfunction()

function(set_freestanding _TARGET)
  target_link_options(${_TARGET}
    PRIVATE -ffreestanding -nostdlib -fno-exceptions
  )
endfunction()

set(CORE_LIB core)
add_library(${CORE_LIB})
set_standards(${CORE_LIB})
set_freestanding(${CORE_LIB})

add_subdirectory(core)
add_subdirectory(sys)
add_subdirectory(mem)
add_subdirectory(drivers)

function(add_platform _ARCH _PLAT)
  set(KERNEL_EXEC kernel)
  add_executable(${KERNEL_EXEC})
  set_standards(${KERNEL_EXEC})
  set_freestanding(${KERNEL_EXEC})
  
  add_subdirectory(arch)
  add_subdirectory(lib)
endfunction()

add_platform(${ARCH} ${TARGET_PLATFORM})
