#include "include.hpp"

int main() {
    Server server;


    server.InitSocket();
    server.bindSocket();
    server.listenSocket();
    server.acceptSocket();
    server.loopSocket();

    server.closeSocket();

    
    

    return 0;
}