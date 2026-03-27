#include "CotServer.h"
#include <iostream>
#include <iomanip>

CotServer::CotServer() {
    // 1. Initialize our final additive share U to exactly 0
    bn_zero(&U_share);

    // 2. Generate 32 random bytes for our multiplicative share 'x'
    uint8_t random_bytes[32];
    random_buffer(random_bytes, 32); 

    // 3. Convert those raw bytes into a bignum256 struct
    bn_read_be(random_bytes, &x_share);
}

void CotServer::printHex(const std::string& label, const bignum256* num) {
    uint8_t buffer[32];
    bn_write_be(num, buffer); // Convert bignum back to bytes for printing
    std::cout << label << ":\n";
    for(int i = 0; i < 32; ++i) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)buffer[i];
    }
    std::cout << std::dec << "\n\n"; // Reset to decimal and add spacing
}

void CotServer::executeMathLoop() {
    std::cout << "--- Initializing COT Protocol ---" << std::endl;
    printHex("Server Multiplicative Share (x)", &x_share);

    // 1. Initialize U (Alice's additive share accumulator) to 0
    bn_zero(&U_share);

    // 2. Initialize a variable to track 2^i (Starting at 2^0 = 1)
    bignum256 pow2;
    uint8_t one_bytes[32] = {0};
    one_bytes[31] = 1;
    bn_read_be(one_bytes, &pow2);

    std::cout << "Running 256 iterations of Correlated OT..." << std::endl;

    for (int i = 0; i < 256; i++) {
        // --- STEP 1: Generate a and calculate Point A ---
        uint8_t a_bytes[32];
        random_buffer(a_bytes, 32); // Random scalar 'a'
        bignum256 a;
        bn_read_be(a_bytes, &a);

        curve_point A;
        point_multiply(&secp256k1, &a, &secp256k1.G, &A);

        // ========================================================
        // NETWORK MOCK: Send A to Bob, Receive B from Bob
        // We simulate Bob sending back B = A + G (so B - A is not zero!)
        curve_point B = A; 
        point_add(&secp256k1, &secp256k1.G, &B); 
        // ========================================================

        // --- STEP 2: Calculate k0 = (a * B)_x ---
        curve_point aB;
        point_multiply(&secp256k1, &a, &B, &aB);
        bignum256 k0 = aB.x;

        // --- STEP 3: Calculate k1 = (a * (B - A))_x ---
        curve_point neg_A = A;
        bignum256 temp_neg_y;
        // Negate A by subtracting A.y from the secp256k1 prime
        bn_subtract(&secp256k1.prime, &A.y, &temp_neg_y);
        neg_A.y = temp_neg_y;

        curve_point B_minus_A = B; // Make a copy of B first
        point_add(&secp256k1, &neg_A, &B_minus_A); // Add neg_A directly into our copy
        
        curve_point a_B_minus_A;
        point_multiply(&secp256k1, &a, &B_minus_A, &a_B_minus_A);
        bignum256 k1 = a_B_minus_A.x;

        // --- STEP 4: Prepare Messages (m0 and m1) ---
        uint8_t u_bytes[32];
        random_buffer(u_bytes, 32); 
        bignum256 U_i; // This is m0
        bn_read_be(u_bytes, &U_i);

        bignum256 m1 = U_i;
        // m1 = U_i + x
        bn_add(&m1, &x_share); 

        // --- STEP 5: Accumulate the Additive Share (U) ---
        // Conceptual: U_share += (U_i * pow2)
        
        // Update pow2 for the next loop (pow2 = pow2 * 2)
        bignum256 two;
        uint8_t two_bytes[32] = {0};
        two_bytes[31] = 2;
        bn_read_be(two_bytes, &two);
        // Conceptual: pow2 = pow2 * 2
    }

    // Step 6: Apply the negative sign to U to finish the equation U = -Sum(...)
    // U_share = order - U_share
    bignum256 final_U;
    bn_subtract(&secp256k1.order, &U_share, &final_U);
    U_share = final_U;

    std::cout << "COT Protocol Complete.\n" << std::endl;
    printHex("Server Additive Share (U)", &U_share);
}