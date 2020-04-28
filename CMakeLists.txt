cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(TrustWalletCore)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(ANDROID)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libstdc++")
endif()
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if (NOT ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang"))
    message(FATAL_ERROR "You should use clang compiler")
endif()

if ("$ENV{PREFIX}" STREQUAL "")
    set(PREFIX "${CMAKE_SOURCE_DIR}/build/local")
else()
    set(PREFIX "$ENV{PREFIX}")
endif()


include_directories(${PREFIX}/include)
link_directories(${PREFIX}/lib)

add_subdirectory(trezor-crypto)

macro(find_host_package)
  set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
  set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY NEVER)
  set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE NEVER)
  find_package(${ARGN})
  set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM ONLY)
  set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
  set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
endmacro(find_host_package)

find_host_package(Boost REQUIRED)

include(ExternalProject)

# Dependencies
include(cmake/Protobuf.cmake)

option(CODE_COVERAGE "Enable coverage reporting" OFF)
if(CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fprofile-arcs -ftest-coverage")
    set(CMAKE_EXE_LINKER_FLAGS "-fprofile-arcs -ftest-coverage")
endif()

# Source files
if(${ANDROID})
    message("Configuring for JNI")
    file(GLOB_RECURSE sources src/*.c src/*.cc src/*.cpp src/*.h jni/cpp/*.c jni/cpp/*.cpp jni/cpp/*.h jni/cpp/*.c)
    add_library(TrustWalletCore SHARED ${sources} ${PROTO_SRCS} ${PROTO_HDRS})

    find_library(log-lib log)
    target_link_libraries(TrustWalletCore PRIVATE TrezorCrypto protobuf ${log-lib} Boost::boost)
else()
    message("Configuring standalone")
    file(GLOB_RECURSE sources src/*.c src/*.cc src/*.cpp src/*.h)
    add_library(TrustWalletCore ${sources} ${PROTO_SRCS} ${PROTO_HDRS})

    target_link_libraries(TrustWalletCore PRIVATE TrezorCrypto protobuf Boost::boost)
endif()
target_compile_options(TrustWalletCore PRIVATE "-Wall")

set_target_properties(TrustWalletCore
    PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
)

# Define headers for this library. PUBLIC headers are used for compiling the
# library, and will be added to consumers' build paths.
target_include_directories(TrustWalletCore
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/jni/cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/build/local/include
)

if(NOT ANDROID AND NOT IOS_PLATFORM AND NOT WASM)
    add_subdirectory(tests)
    add_subdirectory(walletconsole/lib)
    add_subdirectory(walletconsole)
endif()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/swift/cpp.xcconfig.in ${CMAKE_CURRENT_SOURCE_DIR}/swift/cpp.xcconfig @ONLY)

if(ANDROID)
    # Workaround: <https://developercommunity.visualstudio.com/content/problem/171290/android-ndk-include-paths-are-incorrect.html>
    include_directories(BEFORE SYSTEM
        /home/bartek/Android/Sdk/ndk/21.1.6352462/sources/android/support/include
        /home/bartek/Android/Sdk/ndk/21.1.6352462/toolchains/llvm/prebuilt/linux-x86_64/lib64/clang/9.0.8/include # stddef.h
        /home/bartek/Android/Sdk/ndk/21.1.6352462/sysroot/usr/include
        /home/bartek/Android/Sdk/ndk/21.1.6352462/sources/cxx-stl/llvm-libc++abi/include
        /home/bartek/Android/Sdk/ndk/21.1.6352462/sources/cxx-stl/llvm-libc++/include
    )
else()
    include_directories(BEFORE SYSTEM
        /usr/include/x86_64-linux-gnu/c++/7.5.0
        /usr/include/c++/7.5.0
    )
endif()
