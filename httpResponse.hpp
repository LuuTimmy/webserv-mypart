#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

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
#include <filesystem>
#include <fstream>
#include <streambuf>

#include "httpRequest.hpp"

class HttpResponse {
        std::map<int, std::string> _responseStatus; 

        int _clientFd;
        std::string _method;
        std::string _path;
        std::string _version;

        std::string _root;
        std::string _rootPath;
        std::string _defaultPathError;
        std::string _defaultMainPage;
    public:
        HttpResponse();
        void    setInformation(HttpRequest httpRequest, int clientFd);
        void    response(HttpRequest httpRequest, int clientFd);
        void    getMethod();
        void    postMethod();
        void    sendResponse(int nb, std::string page);
};

class location {
    std::string _path;

    std::vector<std::string> _allow_method;
    std::string _root;
    std::string _index;
};

#endif 