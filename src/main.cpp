#include "RedisServer.hpp"

#include <iostream>
#include <thread>
#include <chrono>

int main(int argc, const char** argv) {

    int port  = 6372;
    if(argc >= 2)
        port = std::stol(argv[1]);
    
    RedisServer server(port);

    // Background persistance: dump db every 300 seconds (5 * 60 save database)
    std::thread persistanceThread([](){
        while(true){
            std::this_thread::sleep_for(std::chrono::seconds(300));
            // Dump the database

        }
    });
    persistanceThread.detach();
 
    server.run();
    return 0;
}