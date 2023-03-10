#include "TestServ.hpp"


TestServ::TestServ() :
    _serverName("Base"), _listen(PORT), _root("/html"), 
    _errorPage("/html/error.html"), _index("/index.html"),
    _clientBodySize(1000), _dirListing(false), _locs(0), _redir("")
{
    _method.push_back("GET");
    _method.push_back("DELETE");
    _method.push_back("POST");
}

TestServ::~TestServ() {
    // for (size_t i = 0; i < _locs.size(); i++)
	// 	delete _locs[i];
}

void    TestServ::insertLocation() {
    Location *loc1 = new Location("/dirlisting", "/html/deleteTEST", "/index.html",
        std::vector<std::string> {"GET", "POST" , "DELETE"}, true, "");
    _locs.push_back(loc1);
    Location *loc2 = new Location("/painPerdu", "/html/test1", "/test1.html",
        std::vector<std::string> {"POST", "GET"}, false, "");
    _locs.push_back(loc2);
    Location *loc3 = new Location("/saladeTomate", "/html/test2", "/test2.html",
        std::vector<std::string> {"GET", "POST"}, false, "");
    _locs.push_back(loc3);
    Location *loc4 = new Location("/redir", "/html/test1/kk", "/test2.html",
        std::vector<std::string> {"GET", "POST"}, false, "https://www.google.com/");
    _locs.push_back(loc4);
}


