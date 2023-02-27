

#include "Request.hpp"

void customSplit(std::string str, std::vector<std::string> &strings)
{
    int startIndex = 0, endIndex = 0;
    for (int i = 0; i <= str.size(); i++)
    {

        // If we reached the end of the word or the end of the input.
        if (str[i] == '\n' || i == str.size()) {
            endIndex = i;
            std::string temp;
            temp.append(str, startIndex, endIndex - startIndex);
            strings.push_back(temp);
            startIndex = endIndex + 1;
        }
    }
}

Request::Request(const char *request) : _raw(std::string(request))
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
    this->_protocol_ver = temp.substr(pos + 1);
    this->_protocol_ver = _protocol_ver.substr(0, _protocol_ver.size() - 1);
    
}

// GET /fish.png HTTP/1.1
// User-Agent: PostmanRuntime/7.29.2
// Accept: */*
// Postman-Token: f55e3a00-adae-4f5d-8564-f483c52e994c
// Host: 127.0.0.1:8080
// Accept-Encoding: gzip, deflate, br
// Connection: keep-alive
void    Request::parse_request(const char *request)
{

    std::string req(request);
    int pos = req.find("\r\n\r\n");
    if (pos == std::string::npos)
        return ;
    
    std::string header = req.substr(0, pos);
    this->_body = req.substr(pos + 4);

    std::vector < std::string > strings;
    customSplit(std::string(header), strings);
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
    i++;
    
    // std::cout << "Method: " << this->_method << std::endl;
    // std::cout << "Path: " << this->_path << std::endl;
    // std::cout << "Procol ver: " << this->_protocol_ver << std::endl;
    // std::cout << "Path :" << this->_path << std::endl;
    // std::map<std::string, std::string>::iterator it = _header.begin();
    // while (it != _header.end())
    // {
    //     std::cout << it->first << ": " << it->second << std::endl;
    //     ++it;
    // }
    // std::cout << "Body :" << std::endl;
    // std::cout << this->_body << std::endl;

}