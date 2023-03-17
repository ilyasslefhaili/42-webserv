

#include "Request.hpp"

Request::Request(ClientInfo &client)	: _client(client)
{

}


Request::Request(const Request &src) : _client(src._client)
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

Request::Request(const char *request, size_t length, ClientInfo &client)
	: _client(client), _raw(std::string(request, length))
{
	_header_only = false;
	_client.chunk_finished = true;
	_client.first_time = true;
	_client.chunk_size = 0;
	// _client.body;
	// _client.new_body;
    parse_request(request, length);
}

Request::~Request() {
	// std::cout << this->_path  << " " << this->_client.socket << std::endl;
	// std::cout << "request is destructed " << std::endl;
}

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
	// PIPE_BUF
    std::cout << "Method: " << this->_method << std::endl;
    std::cout << "Path: " << this->_path << std::endl;
    std::cout << "Procol ver: " << this->_protocol_ver << std::endl;
    std::map<std::string, std::string>::const_iterator it = _header.begin();
    while (it != _header.end())
    {
        std::cout << it->first << ": " << it->second << std::endl;
        ++it;
    }
    // std::cout << "Body :" << std::endl;
    // std::cout << this->_body << std::endl;
}

// GET /fish.png HTTP/1.1
// User-Agent: PostmanRuntime/7.29.2
// Accept: */*
// Postman-Token: f55e3a00-adae-4f5d-8564-f483c52e994c
// Host: 127.0.0.1:8080
// Accept-Encoding: gzip, deflate, br
// Connection: keep-alive


void    Request::parse_request(const char *request, size_t length)
{
    std::string req(request, length);
    int pos = req.find("\r\n\r\n");
    std::string header = req.substr(0, pos + 1);

	// const char *s = strnstr(request, "\r\n\r\n", length);
	// _body = (char *) (s + 4);
	// std::cout << "check req size " << req.size() << std::endl;
	// std::cout << "check _body size " << _body.size() << std::endl;

    std::vector < std::string > strings;
    customSplit(std::string(header), strings, '\n');

    std::cout << "currently parsing the request" << std::endl;
    get_method_and_path(strings[0]);
    for (int i = 1; i < strings.size(); i++)
    {
        int pos = strings[i].find(": ");
        if (pos != std::string::npos)
        {
            std::string value = strings[i].substr(pos + 2);
            _header[strings[i].substr(0, pos)] = value.substr(0, value.size() - 1); // to remove the \r
        }
    }

	// std::string str_body(_body, _body_len);
	if (_header["Transfer-Encoding"] == "Chunked")
	{
		// std::string new_body = "";
		// std::cout << "body length " << _body.length() << std::endl;
		// int pos = _body.find("\r\n");
		// while (pos != std::string::npos)
		// {
		// 	std::string len_str = _body.substr(0, pos);
		// 	unsigned int len;
		// 	// std::cout << "len_str " << len_str << std::endl;
		// 	std::stringstream ss(len_str);
		// 	ss << std::hex; ss >> len;
		// 	// std::cout << "len_str in decimal " << len << std::endl;
		// 	if (len == 0)
		// 		break ;
		// 	_body = _body.substr(len_str.length() + 2); // skip both length and \r\n
		// 	new_body += _body.substr(0, len);
		// 	_body = _body.substr(len + 2);
		// 	pos = _body.find("\r\n");
		// }
		// new_body += "\r\n";
		_body = _client.new_body;
		_body_len = _body.size();
		_client.new_body.clear();
		_client.body.clear();
		_client.chunk_finished = true;
		_client.first_time = true;
		_client.chunk_size = 0;

		// std::cout  << _body << std::endl;
	}
	else
	{
		_body = req.substr(pos + 4, length);
		_body_len = atoi(_header["Content-Length"].c_str());
	}
	// print_request();
	// if (_body.find("------WebKitFormBoundary") != std::string::npos)
	// {

	// }5107845
	//5107830
}

Request::Request( ClientInfo &client, const char *header, size_t length) :_client(client)
{
	_header_only = true;
	std::string req(header, length);

	std::vector < std::string > strings;
    customSplit(std::string(header), strings, '\n');

    std::cout << "currently parsing the request" << std::endl;
    get_method_and_path(strings[0]);
    for (int i = 1; i < strings.size(); i++)
    {
        int pos = strings[i].find(": ");
        if (pos != std::string::npos)
        {
            std::string value = strings[i].substr(pos + 2);
            _header[strings[i].substr(0, pos)] = value.substr(0, value.size() - 1); // to remove the \r
        }
    }
	// std::cout << "req " << req << std::endl;
	// print_request();
	// std::cout << "length " << length << std::endl;
	// std::cout << "map content length " << _header["Content-Length"] << std::endl;
	// std::cout << "content length " << atoi(_header["Content-Length"].c_str()) << std::endl;
	_body_len = atoi(_header["Content-Length"].c_str());
	buffer_size = length + 4 + _body_len;
	std::cout << "buffer_size " << buffer_size << std::endl;


}


bool Request::request_is_complete(const char* buffer, size_t length, int added_length, ClientInfo &client)
{
	
	const char* end = (const char *) memmem(buffer, length,  "\r\n\r\n", 4); // double CRLF sequence that marks the end of the header
	if (!client.header_reached)
	{
		if (end != NULL)
		{
			std::cout << "reached" << std::endl;
			if (client.request_obj != nullptr)
				delete client.request_obj;
			client.request_obj = new Request(client, buffer, end - buffer);
			client.header_reached = true;
		}
	}
    
    void* chunked = memmem(buffer, length,  "Transfer-Encoding: Chunked", 26);
	if (chunked != NULL) // find away to collect while collencting ghh
	{
		size_t body_start = end - buffer + 4;
		
		if (client.first_time)
		{
			// if (length - body_start == end + 4)
			client.body = std::string(end + 4, length - body_start);
			client.first_time = false;
		}
		else
		{
			client.body += std::string(buffer + length - added_length, added_length);
			// std::cout << std::string(buffer + length - added_length, added_length) << "." << std::endl;
		}

		if (client.body.size() >= client.chunk_size && client.chunk_size != 0)
			{
				client.new_body += client.body.substr(0, client.chunk_size);
				client.body = client.body.substr(client.chunk_size + 2);
				client.chunk_size = 0;
			}
		// if (client.chunk_finished)
		// {
			client.chunk_finished = false;
			int pos = client.body.find("\r\n");
			while (pos != std::string::npos)
			{
				std::string len_str = client.body.substr(0, pos);
				unsigned int len;
				// std::cout << "len_str " << len_str << std::endl;
				std::stringstream ss(len_str);
				ss << std::hex; ss >> len;
				if (len == 0)
				{
					// std::cout << "check this out "<< len_str << std::endl;
					return true ;
				}
				client.chunk_size = len;
				client.body = client.body.substr(len_str.length() + 2);
				if (client.body.size() >= client.chunk_size)
				{
					client.new_body += client.body.substr(0, len);
					client.body = client.body.substr(len + 2);
					client.chunk_size = 0;
					// client.chunk_finished = true;
					pos = client.body.find("\r\n");
				}
				else
					return false;
			}
			// else
			// {
				return false ;
			// }
		// }
		// else
		// {
		// 	if (client.body.size() >= client.chunk_size)
		// 	{
		// 		client.new_body += client.body.substr(0, client.chunk_size);
		// 		client.body = client.body.substr(client.chunk_size + 2);
		// 		client.chunk_size = 0;
		// 		client.chunk_finished = true;
		// 		std::cout << "LULE " << std::endl;
		// 		// int pos = client.body.find("\r\n");
		// 		// if (pos == std::string::npos)
		// 		// 	return false;
		// 		// std::string len_str = client.body.substr(0, pos);
		// 		// unsigned int len;
		// 		// std::stringstream ss(len_str);
		// 		// ss << std::hex; ss >> len;
		// 		// if (len == 0)
		// 		// {
		// 		// 	std::cout << "LULE" << std::endl;
		// 		// 	return true ;

		// 		// }
		// 		// client.chunk_size = len;
		// 		// client.body = client.body.substr(len_str.length() + 2);
		// 	}
				// client.chunk_finished = true;
			return false ;
		}


    // Check if the content-length is specified
    const char* content_length_str = strnstr(buffer, "Content-Length:", length);
    if (content_length_str != NULL) {
        content_length_str += strlen("Content-Length:");
        int content_length = atoi(content_length_str);
        if (length < (end - buffer) + 4 + content_length) {
            return false; // Not enough data received
        }
    }

    return true; // Request is complete
}

void		Request::set_request(const char *request, size_t length)
{
    parse_request(request, length);
}
