#include "include.hpp"

int main() {
    Server server;

    server.selectInit();
    server.selectLoop();

    return 0;
}
