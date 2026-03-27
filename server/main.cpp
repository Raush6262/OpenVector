#include <iostream>
#include <iomanip>

// trezor-crypto headers
extern "C" {
    #include "bignum.h"
    #include "ecdsa.h"
    #include "secp256k1.h"
}

// Helper function to print bytes in Hex format
void print_hex(const std::string& label, const uint8_t* data, size_t len) {
    std::cout << label << ": ";
    for(size_t i = 0; i < len; ++i) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)data[i];
    }
    std::cout << std::dec << std::endl;
}

int main() {
    std::cout << "--- Trezor-Crypto Math Test ---" << std::endl;

    // 1. Create a 32-byte scalar (Using the number 2 for this test)
    // In the real protocol, this will be your random scalar 'a'
    uint8_t scalar_bytes[32] = {0};
    scalar_bytes[31] = 2; 

    // 2. Convert raw bytes into a bignum256 struct
    bignum256 a;
    bn_read_be(scalar_bytes, &a);

    // 3. Multiply the scalar 'a' by the base point G to get Point A (A = a * G)
    curve_point A;
    point_multiply(&secp256k1, &a, &secp256k1.G, &A);

    // 4. Extract the X and Y coordinates back into raw bytes
    uint8_t x_bytes[32];
    uint8_t y_bytes[32];
    bn_write_be(&A.x, x_bytes);
    bn_write_be(&A.y, y_bytes);

    // 5. Print the results
    print_hex("Point A (X)", x_bytes, 32);
    print_hex("Point A (Y)", y_bytes, 32);

    return 0;
}