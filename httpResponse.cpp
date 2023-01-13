#include "httpResponse.hpp"
#include <cstdlib>

HttpResponse::HttpResponse() {
    _responseStatus.insert(std::pair<int, std::string>(200, "OK"));
    _responseStatus.insert(std::pair<int, std::string>(201, "Created"));
    _responseStatus.insert(std::pair<int, std::string>(400, "Bad Request"));
    _responseStatus.insert(std::pair<int, std::string>(404, "Not Found"));
    _responseStatus.insert(std::pair<int, std::string>(405, "Method Not Allowed"));
    _responseStatus.insert(std::pair<int, std::string>(505, "Version Not Supported"));
}

void    HttpResponse::response(HttpRequest httpRequest, int clientFd, TestServ serv) {
    setInformation(httpRequest, clientFd, serv);

    //verif method ad location
    if (_methodRequest == "GET")
        getMethod();
    else if (_methodRequest == "POST")
        postMethod();
    else if (_methodRequest == "DELETE") {}
    else
        sendResponse(405, _defaultPathError);
}

void    HttpResponse::setInformation(HttpRequest httpRequest, int clientFd, TestServ serv) {
    _methodRequest = httpRequest.getMethod();
    _clientFd = clientFd;
    _version = httpRequest.getVersion();
    _rootPath = std::getenv("PWD"); //egegzgezgzggeegegg
    _defaultPathError = serv.getErrorPage();

    _allowedMethod = serv.getMethod();
    _root = serv.getRoot();

    int pos; //A TOUT VERIF
    if ((pos = verifLocation(httpRequest.getPath(), serv.getLocation())) >= 0) {
        Location* loc = serv.getLocation()[pos];

        _path = httpRequest.getPath().substr(loc->getPath().size(), httpRequest.getPath().size());
        _index = loc->getIndex(); //oblifer d'avoir ? 
        _root = loc->getRoot(); //oblige d'avoir
        _allowedMethod = loc->getMethod();

        _dirListing = loc->getDirListing();
        _redir = loc->getRedir();
    }
    std::cout << "index:  " << _index << std::endl; 
    std::cout << "path:  " << _path << std::endl; 

    if (_index.empty())
        _index = serv.getIndex();
    if (_path.empty())
        _path = _index;
        //need to clear;
    
}

int HttpResponse::verifLocation(std::string path, std::vector<Location *> locs) {
    if (locs.size() <= 0)
        return (-1);
    if (path.find("/", 1) != std::string::npos)
        path = path.substr(0, path.find("/", 1));
    for (int i = 0; i < locs.size(); i++) {
        if (path == locs[i]->getPath())
            return (i);
    }
    return (-1);
}

void    HttpResponse::getMethod() {
    std::cout << "Get Method" << std::endl;
    //default loc; si vide;


    std::cout << "PATH ::::     " << _path << std::endl;


    if (_path == "/") {
        _path = _index;
        sendResponse(200, _path);
    }
    else {
        try {
            std::ifstream fd(_rootPath + _root +_path);
            if (fd) {
                sendResponse(200, _path);
            }
            else {
                sendResponse(404, _defaultPathError);
            }
        }
        catch (const std::ios_base::failure & fail) {
            std::cerr << fail.what() << std::endl; //a enlever
            sendResponse(400, _defaultPathError);
        }
    }
}

void    HttpResponse::postMethod() {}

void HttpResponse::sendResponse(int nb, std::string path) {
    if (nb >= 400)
        _root = "/";

    std::ifstream fd(_rootPath + _root + path);
    std::cout << _rootPath + _root + path << std::endl;

    std::string page((std::istreambuf_iterator<char>(fd)),std::istreambuf_iterator<char>());
    std::string res = _version + " " + std::to_string(nb)+ " " +_responseStatus.find(nb)->second + "\r\n";
    res = res + "Content-type: text/html" + "\r\n";//a verif le type de docu
    res = res + "Content-length:" + std::to_string((res + page).size()) + "\r\n\r\n";
    res = res + page + "\r\n";
    std::cout << res << std::endl;
    send(_clientFd, res.c_str(), res.size(), 0);
}

