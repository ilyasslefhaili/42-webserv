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