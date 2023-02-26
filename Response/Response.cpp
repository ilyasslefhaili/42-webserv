/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilefhail <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 00:54:52 by ilefhail          #+#    #+#             */
/*   Updated: 2023/02/26 00:54:53 by ilefhail         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

std::string create_status_line(int status){
    if (status == 404)
        return ("HTTP/1.1 404 Not Found\n\r");
    else if (status == 200)
        return ("HTTP/1.1 200 OK\n\r");
    return "";
}

std::string get_content_lenght(Response &a){
    std::string str = "content-length: " + std::to_string(a.get_body().size());
    return (str + "\n\r");
}

std::string get_response(Request& re_st){
    Response &a = get_response_object(re_st);
    std::string response;

    response += create_status_line(a.get_status());
    response += content_from_path(re_st._path);
    response += get_content_lenght(a);
    response += "\n\r";
    response += a.get_body();
    return response;
}

Response& get_response_object(Request& re_st){
    Response *a = new Response();

    a->set_content_type(content_from_path(re_st._path));
    if (re_st._method == "GET")
    {
        if (a->file_opening(re_st._path, re_st._method))
            return (*a);
    }
    return *a;
}

int Response::file_opening(std::string path, std::string method){
    if (method == "GET")
    {
        this->_file.open(path);
        if (!(this->_file.is_open()))
        {
            this->_status = 404;
            return (404);
        }
        else
        {
            this->_status = 200;
            std::string str;
            while (!this->_file.eof()){
                std::getline(this->_file, str);
                this->_body += str;
                if (!this->_file.eof())
                    this->_body += "\n";
            }
        }
    }
    return (0);
}

std::string Response::get_body(){
    return _body;
}
void Response::set_body(std::string body){
    this->_body = body;
}
std::string Response::get_content_type(){
    return (_content_type);
}
void Response::set_content_type(std::string type){
    this->_content_type = type;
}
void Response::set_status(int status){
    _status = status;
}

int Response::get_status(){
    return _status;
}

Response::Response(/* args */)
{
}

Response::~Response()
{
}