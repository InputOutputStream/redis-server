#include "RedisServer.hpp"
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <vector>
#include <thread>

#include "RedisCommandHandler.hpp"
#include <cstring>

static RedisServer* global_server = nullptr;

RedisServer::RedisServer(int port) : port(port), server_socket(-1), running(true){
    global_server = this;
}


void RedisServer::shutdown()
{
    running = false;
    if(server_socket != -1)
    {
        close(server_socket);
    }

    std::cout <<"Server Shutdown Complete\n";
}


void RedisServer::run() {
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket < 0)
    {
        std::cerr <<"Error Creating Server Socket\n";
        return;
    }

    int opt = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if(bind(server_socket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
    {
        std::cerr << "Error Binding server socket\n";
        return;
    }

    if(listen(server_socket, 10) < 0)
    {
        std::cerr <<"Error Listening On Server Socket\n";
        return;
    }

    std::cout <<"Redis Server Listening on port: "<<port<<"\n";
    
    
    std::vector<std::thread> threads;
    RedisCommandHandler cmdHandler;

    while(running){
        int client_socket = accept(server_socket, nullptr, nullptr);
        if(client_socket < 0){
            if(running)
                std::cerr <<"Error Accpeting Client Connection\n";
            break;
        }

        threads.emplace_back([client_socket, &cmdHandler](){
            char buffer[1024];
            while(true)
            {
                memset(buffer, 0, sizeof(buffer));
                int bytes = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
                if(bytes <= 0)
                    break;
                
                std::string request(buffer, bytes);
                std::string response = cmdHandler.processCommand(request);
                send(client_socket, response.c_str(), response.size(), 0);
            }
            close(client_socket);
        });
    }

    for(auto& t: threads){
        if(t.joinable()) t.join();
    }

    // Shutdown
}
