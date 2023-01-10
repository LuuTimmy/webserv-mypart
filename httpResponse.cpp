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

void    HttpResponse::response(HttpRequest httpRequest, int clientFd) {
    setInformation(httpRequest, clientFd);

    //verif method ad location
    if (_method == "GET")
        getMethod();
    else if (_method == "POST")
        postMethod();
    else if (_method == "DELETE") {}
    else
        sendResponse(405, _defaultPathError);
}

void    HttpResponse::setInformation(HttpRequest httpRequest, int clientFd) {
    _defaultMainPage = "index.html";
    _clientFd = clientFd;
    _root = "/html";
    _rootPath = std::getenv("PWD"); //PAS C++98 et need to changer en fonction du serv loc
    _defaultPathError = "/error.html";
    _method = httpRequest.getMethod();
    _path = httpRequest.getPath();
    _version = httpRequest.getVersion();
}

void    HttpResponse::getMethod() {
    std::cout << "Get Method" << std::endl;
    //default loc; si vide;


    std::cout << "PATH ::::     " << _path << std::endl;


    if (_path == "/") {
        _path = "/index.html";
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
    std::ifstream fd(_rootPath + _root + path);
    std::cout << _rootPath + _root +path << std::endl;



    std::string page((std::istreambuf_iterator<char>(fd)),std::istreambuf_iterator<char>());
    std::string res = _version + " " + std::to_string(nb)+ " " +_responseStatus.find(nb)->second + "\r\n";
    res = res + "Content-type: text/html" + "\r\n";//a verif le type de docu
    res = res + "Content-length:" + std::to_string((res + page).size()) + "\r\n\r\n";
    res = res + page + "\r\n";
    std::cout << res << std::endl;
    send(_clientFd, res.c_str(), res.size(), 0);
}

