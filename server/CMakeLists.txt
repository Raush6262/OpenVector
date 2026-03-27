cmake_minimum_required(VERSION 3.10)
project(CotServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 99)

find_package(Boost REQUIRED)
# THE FIX: Use CONFIG to pull in all Abseil hidden dependencies
find_package(protobuf CONFIG REQUIRED)

file(GLOB TREZOR_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/libs/trezor-crypto/*.c")
list(REMOVE_ITEM TREZOR_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/libs/trezor-crypto/bip32.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/libs/trezor-crypto/bip39.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/libs/trezor-crypto/nem.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/libs/trezor-crypto/shamir.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/libs/trezor-crypto/slip39.c"
)

include_directories(
    ${Boost_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/trezor-crypto
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(CotServer main.cpp CotServer.cpp cot.pb.cc ${TREZOR_SOURCES})
target_link_libraries(CotServer PRIVATE protobuf::libprotobuf)