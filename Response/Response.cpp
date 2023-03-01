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

void Response::fill_attributes(Request& re_st){
    if (re_st._method == "GET"){
        this->_file.open(this->_path);
        if (!(this->_file.is_open()))
        {
            this->_status = 404;
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
    else if (re_st._method == "POST"){
        this->_status = 200;
        this->_file.open(re_st._path, std::ios::out);
        this->_file<<re_st._body;
        this->_file.close();
    }
}
void Response::get_error_page(){
    
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

void Response::link_root_path(Request& re_st){
    _path = this->_configs._root;
    _path += re_st._path;
    std::cout<<"                    "<<_path<<std::endl;
}

void   Response::set_config(ServerConfig& conf){
    this->_configs = conf;
}

ServerConfig& Response::get_config(){
    return (this->_configs);
}

Response::Response(/* args */)
{
}

Response::~Response()
{
}
