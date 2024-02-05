#include "include.hpp"

int main() {
    Server server;

    server.InitSocket();
    server.bindSocket();
    server.listenSocket();
    server.acceptSocket();
    server.loopSocket();
    server.closeSocket();

    int test =  0;

    std::cout << "test: " << test << std::endl;

    return 0;
}
