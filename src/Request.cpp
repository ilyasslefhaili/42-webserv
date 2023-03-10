

#include "Request.hpp"

Request::Request(const Request &src)
{
	*this = src;
}

Request & Request::operator=(const Request &rhs)
{
	this->_raw = rhs._raw;
	this->_method = rhs._method;
	this->_protocol_ver = rhs._protocol_ver;
	this->_header = rhs._header;
	// this->_body = rhs._body;
	this->_path = rhs._path;
	return (*this);
}


void customSplit(std::string str, std::vector<std::string> &strings, char c)
{
    int startIndex = 0, endIndex = 0;
    for (int i = 0; i <= str.size(); i++)
    {
        if (str[i] == c || i == str.size()) {
            endIndex = i;
            std::string temp;
            temp.append(str, startIndex, endIndex - startIndex);
            strings.push_back(temp);
            startIndex = endIndex + 1;
        }
    }
}

Request::Request(const char *request)
{
    parse_request(request);
}

Request::~Request() {}

// GET /fish.png HTTP/1.1
void    Request::get_method_and_path(std::string &line)
{
    int pos = line.find("/");
    if (pos == std::string::npos)
        return ;
    this->_method = line.substr(0, pos - 1);


    std::string temp =  line.substr(pos);
    pos = temp.find(" ");
    if (pos == std::string::npos)
        return ;
    this->_path = temp.substr(0, pos);
    // std::cout << "get_method_and_path " << this->_path << std::endl;
    this->_protocol_ver = temp.substr(pos + 1);
    this->_protocol_ver = _protocol_ver.substr(0, _protocol_ver.size() - 1);
    
}

void    Request::print_request() const
{
    std::cout << "Method: " << this->_method << std::endl;
    std::cout << "Path: " << this->_path << std::endl;
    std::cout << "Procol ver: " << this->_protocol_ver << std::endl;
    std::map<std::string, std::string>::const_iterator it = _header.begin();
    while (it != _header.end())
    {
        std::cout << it->first << ": " << it->second << std::endl;
        ++it;
    }
    std::cout << "Body :" << std::endl;
    std::cout << this->_body << std::endl;
}

// GET /fish.png HTTP/1.1
// User-Agent: PostmanRuntime/7.29.2
// Accept: */*
// Postman-Token: f55e3a00-adae-4f5d-8564-f483c52e994c
// Host: 127.0.0.1:8080
// Accept-Encoding: gzip, deflate, br
// Connection: keep-alive

# include <cstring>

void    Request::parse_request(const char *request)
{
    std::string req(request);
    int pos = req.find("\r\n\r\n");
    if (pos == std::string::npos)
        return ;
    
    std::string header = req.substr(0, pos + 1);
    // this->_body = req.substr(pos + 4);
	const char *s = strstr(request, "\r\n\r\n");
	_body = (char *) s + 4;

    std::vector < std::string > strings;
    customSplit(std::string(header), strings, '\n');
    // std::vector<std::string>::iterator it = strings.begin();

    std::cout << "currently parsing the request" << std::endl;
    get_method_and_path(strings[0]);

    int i;
    for (i = 1; i < strings.size(); i++)
    {
        int pos = strings[i].find(": ");
        if (pos != std::string::npos)
        {
            std::string value = strings[i].substr(pos + 2);
            _header[strings[i].substr(0, pos)] = value.substr(0, value.size() - 1); // to rmeove the \r

        }
    }
	_body_len = atoi(_header["Content-Length"].c_str());
	std::cout << "size of body: " << _body_len << std::endl;
    // print_request();

}

bool Request::request_is_complete(const char* buffer, int length)
{
    const char* end = strnstr(buffer, "\r\n\r\n", length); // double CRLF sequence that marks the end of the header
    if (end == nullptr) {
        return false; // header was not yet received
    }

    // Check if the ontent-length is specified
    const char* content_length_str = strnstr(buffer, "Content-Length:", length);
    if (content_length_str != nullptr) {
        content_length_str += strlen("Content-Length:");
        int content_length = atoi(content_length_str);
        if (length < (end - buffer) + 4 + content_length) {
            return false; // Not enough data received
        }
    }
    return true; // Request is complete
}
