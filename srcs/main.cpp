#include "include.hpp"

int main(int argc, char **argv) {
    Server server;

   #ifdef BUILD 
    if (argc != 3) {std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl; exit(1);}

    int port = atoi(argv[1]);
    if (port < 1024 || port > 65535) { std::cerr << "Invalid port" << std::endl; exit(1); }
    server.set_port(port);

    std::string key = argv[2];
    if (key.size() < 1) { std::cerr << "Invalid password" << std::endl; exit(1);}
    if (key.size() > 50) { std::cerr << "Password too long" << std::endl; exit(1);}
    server.set_keypass(key);
    #endif

    server.selectInit();
    server.selectLoop();

    return 0;
}
