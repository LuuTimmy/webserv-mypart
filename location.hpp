#ifndef LOCATION_HPP
#define LOCATION_HPP

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

class Location{
        std::string                 _path;
        std::string                 _root;
        std::string                 _index;
        std::vector<std::string>    _method;
        std::string                 _dirListing;
        std::string                 _redir;
        //std::string new loc ? 
    public:
        Location(std::string path, std::string root, std::string index, std::vector<std::string> method, std::string dir, std::string redir);
        ~Location();

        std::string getPath() { return (_path); };
        std::string getRoot() { return (_root); };
        std::string getIndex() { return (_index); };
        std::vector<std::string> getMethod() { return (_method); };
        std::string getDirListing() { return (_dirListing); };
        std::string getRedir() { return (_redir); };
        
};

#endif 