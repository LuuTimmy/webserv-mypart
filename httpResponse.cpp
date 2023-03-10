#include "httpResponse.hpp"

HttpResponse::HttpResponse() {
    _responseStatus.insert(std::pair<int, std::string>(200, "OK"));
    _responseStatus.insert(std::pair<int, std::string>(201, "Created"));
    _responseStatus.insert(std::pair<int, std::string>(400, "Bad Request"));
    _responseStatus.insert(std::pair<int, std::string>(403, "Forbidden"));
    _responseStatus.insert(std::pair<int, std::string>(404, "Not Found"));
    _responseStatus.insert(std::pair<int, std::string>(405, "Method Not Allowed"));
    _responseStatus.insert(std::pair<int, std::string>(505, "Version Not Supported"));
}

void    HttpResponse::response(HttpRequest httpRequest, int clientFd, TestServ serv) {
    setInformation(httpRequest, clientFd, serv);

    //si aucun error 405
    if (_redir != "")
        toRedir();
    else if (_methodRequest == "GET" 
        && std::find(_allowedMethod.begin(), _allowedMethod.end(), "GET") != _allowedMethod.end())
        getMethod();
    else if (_methodRequest == "POST" 
        && std::find(_allowedMethod.begin(), _allowedMethod.end(), "POST") != _allowedMethod.end())
        postMethod();
    else if (_methodRequest == "DELETE" 
        && std::find(_allowedMethod.begin(), _allowedMethod.end(), "DELETE") != _allowedMethod.end())
        deleteMethod();
    else
        sendResponse(403, _defaultPathError);
}

void    HttpResponse::setInformation(HttpRequest httpRequest, int clientFd, TestServ serv) {
    _rootPath = std::getenv("PWD"); //A changer car pas c++98
    _clientFd = clientFd;
    _version = httpRequest.getVersion();
    _methodRequest = httpRequest.getMethod();

    _defaultPathError = serv.getErrorPage();

    _allowedMethod = serv.getMethod();
    _dirListing = serv.getDirListing();
    _index = serv.getIndex();
    _path = httpRequest.getPath();
    if ((_path.empty() || _path == "/") && _dirListing == false)
        _path = _index;
    _root = serv.getRoot();
    _redir = serv.getRedir();

    int pos;
    if ((pos = verifLocation(httpRequest.getPath(), serv.getLocation())) >= 0) {
        Location* loc = serv.getLocation()[pos];

        _allowedMethod = loc->getMethod();
        _dirListing = loc->getDirListing();
        _index = loc->getIndex();
        _path = httpRequest.getPath().substr(loc->getPath().size(), httpRequest.getPath().size());
        if ((_path.empty() || _path == "/") && _dirListing == false)
            _path = _index;
        _root = loc->getRoot();
        _redir = loc->getRedir();
    }
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
        try {
            std::ifstream fd(_rootPath + _root + _path);
            std::cout << "root = " << _root << " path " << _path << std::endl;
            if (fd)
                sendResponse(200, _path);
            else
                sendResponse(404, _defaultPathError);
        }
        catch (const std::ios_base::failure & fail) {
            if (_dirListing == true)
                toDirListing();
            else
                sendResponse(200, _path);
        }
}

void    HttpResponse::postMethod() {
}

void    HttpResponse::deleteMethod() {
    try {
        std::ifstream fd(_rootPath + _root + _path);
        if (fd) {
            std::remove((_rootPath + _root + _path).c_str());
            sendResponse(200, _defaultPathError); //a check ce qu'il faut renvoyer
        }
        else
            sendResponse(404, _defaultPathError);
    }
    catch(const std::ios_base::failure & fail) {
        sendResponse(403, _defaultPathError);
    }
}

void    HttpResponse::toDirListing() {
    struct dirent *file;
    std::string res =  _version + " 200 OK\n\n<!DOCTYPE html><html><body><h1>";
    std::string resEnd = "</h1></body></html>";

    DIR *dir;
    dir = opendir((_rootPath + _root + _path).c_str());
    if (!dir)
        return ; 
    while ((file = readdir(dir)) != NULL)
        res += "<p>" + std::string(file->d_name) + "</p>";
    res += resEnd;
    //close(dir);
    send(_clientFd, res.c_str(), res.size(), 0);
    std::cout << "Dir listing finish" << std::endl;
}

void    HttpResponse::toRedir() {
    std::string res = _version + " 200 OK\n\n<head><meta http-equiv=\"Refresh\" content=\"0;url=" 
        + _redir + "\"/></head>"; 
    send(_clientFd, res.c_str(), res.size(), 0);
}

void    HttpResponse::sendResponse(int nb, std::string path) {
    if (nb >= 400)
        _root.clear();
    std::ifstream fd(_rootPath + _root + path);
    std::cout << _rootPath + _root + path << std::endl;
    std::string page((std::istreambuf_iterator<char>(fd)),std::istreambuf_iterator<char>());
    std::string res = _version + " " + std::to_string(nb)+ " " +_responseStatus.find(nb)->second + "\r\n";
    res = res + "Content-type:" + getContentType(path) + "\r\n";
    res = res + "Content-length:" + std::to_string((res + page).size()) + "\r\n\r\n";
    res = res + page + "\r\n";
    std::cout << res << std::endl;
    send(_clientFd, res.c_str(), res.size(), 0);
}

std::string HttpResponse::getContentType(std::string path) {
    size_t found = path.find_last_of('.');

    if (found == std::string::npos)
        return ("text/plain");
    std::string last(path, found);
    if (last == ".html") { return ("text/html"); }
    else if (last == ".css") { return ("text/css"); }
    else if (last == ".csv") { return ("text/csv"); }
    else if (last == ".xml") { return ("text/xml"); }
    else if (last == ".gif") { return ("image/gif"); }
    else if (last == ".jpeg") { return ("image/jpeg"); }
    else if (last == ".png") { return ("image/png"); }
    else if (last == ".tiff") { return ("image/tiff"); }
    else if (last == ".javascript") { return ("application/javascript"); }
    else if (last == ".pdf") { return ("application/pdf"); }
    else if (last == ".json") { return ("application/json"); }
    else if (last == ".javascript") { return ("application/javascript"); }
    else { return ("text/plain"); }
}
