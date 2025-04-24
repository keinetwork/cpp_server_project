#include "client/client.hpp"
#include "common/logger.hpp"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    try {
        if (argc != 3) {
            std::cerr << "Usage: " << argv[0] << " <host> <port>" << std::endl;
            return 1;
        }

        Logger::init();
        
        boost::asio::io_context io_context;
        Client client(io_context, argv[1], argv[2]);
        
        io_context.run();
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
} 