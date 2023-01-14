#include "location.hpp"

Location::Location(std::string path, std::string root, std::string index, std::vector<std::string> method, bool dir, std::string redir) 
    : _path(path), _root(root), _index(index), _method(method), _dirListing(dir), _redir(redir)    
{

}

Location::~Location() {

}