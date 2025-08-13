#ifndef REDIS_COMMAND_HANDLER_H
#define REDIS_COMMAND_HANDLER_H

#include <iostream>
#include <string>

class RedisCommandHandler
{
private:
    /* data */
public:
    RedisCommandHandler();

    // Process a command from a client and return RESP-formatted repsonse
    std::string processCommand(const std::string &commandLine);
};




#endif // !REDIS_COMMAND_HANDLER_H