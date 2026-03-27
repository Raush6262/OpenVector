cmake_minimum_required(VERSION 3.10)
project(CotServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 99)

find_package(Boost REQUIRED)

# Grab all the C files from the root of trezor-crypto
file(GLOB TREZOR_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/libs/trezor-crypto/*.c")

# Remove the hardware wallet and altcoin files that require the ed25519 curve
list(REMOVE_ITEM TREZOR_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/libs/trezor-crypto/bip32.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/libs/trezor-crypto/bip39.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/libs/trezor-crypto/nem.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/libs/trezor-crypto/shamir.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/libs/trezor-crypto/slip39.c"
)

# Include directories so your code can find the headers
include_directories(
    ${Boost_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/trezor-crypto
)

# Compile CotServer using main.cpp AND the filtered trezor-crypto files
add_executable(CotServer main.cpp ${TREZOR_SOURCES})