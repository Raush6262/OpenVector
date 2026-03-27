#ifndef COT_SERVER_H
#define COT_SERVER_H

#include <cstdint>
#include <string>

// Wrap the C library headers in extern "C"
extern "C" {
    #include "bignum.h"
    #include "ecdsa.h"
    #include "secp256k1.h"
    #include "rand.h" // For generating our random 32-byte shares
}

class CotServer {
private:
    bignum256 x_share; // Alice's multiplicative share (x)
    bignum256 U_share; // Alice's final additive share (U)

public:
    CotServer(); // Constructor
    
    // The core math loop
    void executeMathLoop(); 
    
    // A helper to neatly print our big numbers
    void printHex(const std::string& label, const bignum256* num);
};

#endif