project (mscc-modular-firmware)

cmake_minimum_required(VERSION 2.8)
include(.cmake/version.cmake)
include(.cmake/flags.cmake)

option(STANDALONE "Build as a stand-alone application" OFF)
option(DYNAMIC "Build a dynamic linked application" OFF)

if (${STANDALONE})
    add_definitions(-DSTANDALONE)
endif()

if (NOT ${DYNAMIC})
    # Build static executables!
    SET(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
    SET(BUILD_SHARED_LIBRARIES OFF)
    SET(CMAKE_EXE_LINKER_FLAGS "-static -pthread")
endif()

set(LINK_LIBRARY z pthread)
# enable mbedtls signing by default

option(ALLOW_UNSIGNED_IMAGES "Allow using a unsigned image" ON)
if (${ALLOW_UNSIGNED_IMAGES})
    add_definitions(-DALLOW_UNSIGNED_IMAGES)
endif()

option(MBEDTLS_SIGNING "Enable signing using the mbedtls library" ON)
if(${MBEDTLS_SIGNING})
  find_package(Threads)
  find_library(MBEDTLS_LIBRARY NAMES mbedtls)
  find_library(MBEDCRYPTO_LIBRARY NAMES mbedcrypto)
  list(APPEND LINK_LIBRARY ${MBEDTLS_LIBRARY} ${MBEDCRYPTO_LIBRARY})
  add_definitions(-DMBEDTLS_SIGNING)
endif(${MBEDTLS_SIGNING})

# xz
#add_definitions(-DXZ_DEC_SINGLE=1)
include_directories(xz-embedded/linux/include/)
include_directories(xz-embedded/linux/include/linux)
include_directories(xz-embedded/userspace)
set(xz_src
    xz-embedded/linux/lib/xz/xz_crc32.c
    xz-embedded/linux/lib/xz/xz_crc64.c
    xz-embedded/linux/lib/xz/xz_dec_bcj.c
    xz-embedded/linux/lib/xz/xz_dec_lzma2.c
    xz-embedded/linux/lib/xz/xz_dec_stream.c)

# Write defines to a config file
configure_file (
    "${CMAKE_CURRENT_SOURCE_DIR}/src/config.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/src/config.h"
    ESCAPE_QUOTES)

include_directories(src)
include_directories("${CMAKE_CURRENT_BINARY_DIR}/src")
set(src
    src/main.c
    src/ramload.c
    src/tar.c
    src/run.c
    src/basic_linux_system_init_urandom.c
    src/firmware_vimage.c
    src/vtss_mtd.c
    src/service.c
    src/service_parse.c
    ${xz_src}
)

add_executable(stage2-loader ${src})
target_link_libraries(stage2-loader ${LINK_LIBRARY})

install(TARGETS stage2-loader
        RUNTIME
        DESTINATION ${INSTALL_BIN_DIR})

install(FILES stage2-secure-loader DESTINATION ${INSTALL_BIN_DIR})

add_custom_command(
    TARGET stage2-loader POST_BUILD
    COMMAND ln -sf stage2-loader stage2-secure-loader
)
