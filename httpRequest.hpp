#pragma once

#include <arpa/inet.h>
#include <fcntl.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>
#include <map>
#include <functional>
#include <sstream>
#include <algorithm>

class HttpRequest {
        std::vector<std::string> _validMethods;

        std::string _request;
        std::string _method;
        std::string _path;
        std::string _version;
        std::string _query;
        //std::map<std::string, std::string> _query;
    public:
        HttpRequest();
        std::string copy_until(const std::string & str, char c);
        void readRequest(std::string & request);
        // void verifMethod();
        // void verifPath();
        // void verifVersion();

        std::string getMethod();
        std::string getPath();
        std::string getVersion();
};