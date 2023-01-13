#ifndef TESTSERV_HPP
#define TESTSERV_HPP

#define PORT 8016
#include "location.hpp"
#include "httpRequest.hpp"

class TestServ 
{
    std::string                 _serverName;
    int                         _listen;
    std::vector<std::string>    _method;
    std::string                 _root;
    std::string                 _errorPage;
    std::string                 _index;
    int                         _clientBodySize;
    std::string                 _listing;

    std::vector<Location*>      _locs;

    public: 
        TestServ();
        ~TestServ();

        void                    insertLocation();
        std::string             getServerName() { return (_serverName); };
        int                     getListen() { return (_listen); };
        std::vector<std::string>    getMethod() { return (_method); };
        std::string             getRoot() { return (_root); };
        std::string             getErrorPage() { return (_errorPage); };
        std::string             getIndex() { return (_index); };
        int                     getClientBodySize() { return (_clientBodySize); };
        std::string             getListing() { return (_listing); };
        std::vector<Location*>  getLocation() { return (_locs); };

};

#endif