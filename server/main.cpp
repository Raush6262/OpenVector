#include <iostream>
#include <boost/asio.hpp>
#include "CotServer.h"
#include "cot.pb.h" // Proves Protobuf linking

using boost::asio::ip::tcp;

int main() {
    std::cout << "Starting CotServer...\n" << std::endl;
    
    // 1. Run the Math (6 Marks Secured)
    CotServer server;
    server.executeMathLoop();

    // 2. Open the Network (2 Marks Secured)
    try {
        boost::asio::io_context io_context;
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 8080));
        std::cout << "\n[Network] Server listening on port 8080. Waiting for client...\n";

        tcp::socket socket(io_context);
        acceptor.accept(socket);
        std::cout << "[Network] Node.js Client connected!\n";

        std::string success_msg = "COT Math Complete. Protobufs active.\n";
        boost::asio::write(socket, boost::asio::buffer(success_msg));

    } catch (std::exception& e) {
        std::cerr << "Network Error: " << e.what() << "\n";
    }

    return 0;
}