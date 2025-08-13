#include "RedisCommandHandler.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

//RESP parser
// *2\r\n$4\r\nPING\r\nTEST\r\n
// *2 -> array has two elements
// $4 -> next string has 4 caracters
// PING
// TEST

std::vector<std::string> parseRespCommand(const std::string &input){
    std::vector<std::string> tokens;
    if(input.empty()) return tokens;

    // if it doesn't start with *, fallback to splitting by whitespace
    if(input[0] != '*')
    {
        std::istringstream iss{input};
        std::string token;
        while(iss >> token)
            tokens.push_back(token);
        return tokens;
    }

    size_t pos = 0;
    // Expect '*' followed by number of elements
    if(input[pos] != '*') return tokens;
    pos++; // skip '*'

    //crlf = carraige return (/r), lien feed (\n)
    size_t crlf = input.find("\r\n", pos);
    if(crlf == std::string::npos) return tokens;

    int numElements = std::stoi(input.substr(pos, crlf - pos));
    pos = crlf + 2;

    for(int i = 0; i<numElements; i++)
    {
        if(pos >= input.size() || input[pos] != '$') break; // format error
        pos++; // skip $

        crlf = input.find("\r\n", pos);
        if(crlf == std::string::npos) break;
        int len = std::stoi(input.substr(pos, crlf - pos));
        pos = crlf + 2;

        if(pos + len > input.size()) break;
        std::string token = input.substr(pos, len);
        tokens.push_back(token);
        pos += len + 2; // skip token and crlf
    }

    return tokens; 
}

RedisCommandHandler::RedisCommandHandler()
{

}

std::string RedisCommandHandler::processCommand(const std::string &commandLine){
    auto tokens = parseRespCommand(commandLine);
    if(tokens.empty())
        return "Error: Empty command\r\n";
    
    for(auto& t: tokens){
        std::cout << t << "\n";
    }
    
    std::string cmd = tokens[0];
    std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::toupper);
    std::ostringstream response;

    // Connect to database

    // check commands
 
    return response.str();
}
