#ifndef REDIS_SERVER_H
#define REDIS_SERVER_H

#include <string>
#include <atomic>

class RedisServer
{
    private:
        int port;
        int server_socket;
        std::atomic<bool> running;
    public:
        RedisServer(int port);
        void run();
        void shutdown();
};

    




#endif // !REDIS_SERVER_H