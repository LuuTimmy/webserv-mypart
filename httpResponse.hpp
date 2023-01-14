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
#include <vector>

#include <dirent.h>

#include "httpRequest.hpp"
#include "testServ.hpp"

class HttpResponse {
        std::map<int, std::string> _responseStatus; 

        //request
        int _clientFd;
        std::string _methodRequest;
        std::string _version;
        std::string _rootPath;

        //serv
        std::string _path;
        std::string _root;
        std::vector<std::string> _allowedMethod;
        std::string _defaultPathError;
        std::string _index;
        bool        _dirListing;
        std::string _redir;

        TestServ _serv;
    public:
        HttpResponse();
        void    setInformation(HttpRequest httpRequest, int clientFd, TestServ serv);
        void    response(HttpRequest httpRequest, int clientFd, TestServ serv);
        int     verifLocation(std::string path, std::vector<Location *> locs);
        
        void    toRedir();
        void    toDirListing();
        void    getMethod();
        void    postMethod();
        void    deleteMethod();
        
        std::string getContentType(std::string path);
        void    sendResponse(int nb, std::string page);

};

#endif 