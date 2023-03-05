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

void    Response::get_index(){
    if (this->_configs._index.size() > 0){
        this->_file.open(this->_path + _configs._index[0]);
        size_t i = 0;
        while (i < this->_configs._index.size()&& this->_file.fail()){
            this->_file.open(this->_path + _configs._index[i]);
        }
        if (!this->_file.fail())
            this->_file.close();
        this->_path += _configs._index[i];
    }
}

std::string& Response::get_path(){
    return (_path);
}

void check_the_file_permissions(std::string& path, int *status){
    int reslt = access(path.c_str(), F_OK);
    if (reslt == 0){
        reslt = access(path.c_str(), R_OK);
        std::cout<<"--------"<<std::endl;
        if (reslt != 0){
            *status = 401;
        }
    }
    else
        throw (std::exception());
}

void Response::fill_attributes(Request& re_st){
    if (this->_dir_or_file)
        this->get_index();
    try{
        check_the_file_permissions(this->_path, &this->_status);
    }
    catch(const std::exception& e){
        std::cout<<"file not found"<<std::endl;
        this->_status = 404;
        return ;
    }
    this->_file.open(this->_path);
    std::string str;
    while (!this->_file.fail() && !this->_file.eof()){
        std::getline(this->_file, str);
        this->_status = 200;
        this->_body += str;
        if (!this->_file.eof())
            this->_body += "\n";
    }
}

void Response::get_error_page(){
   this->_error_page = this->_configs._root;
   this->_error_page += this->_configs._error_pages[std::to_string(this->_status)];
   std::fstream error;
   error.open(this->_error_page);
   if (error.is_open())
   {
        std::string str;
        while (!error.eof())
        {
            std::getline(error, str);
            this->_body += str;
            if (!error.eof())
                this->_body += "\n";
        }
   }
}

int     compare_str(std::string a, std::string b){
    int i = 0;
    while (a[i] == b[i] && a[i] && b[i])
        i++;
    return (i);
}

void    Response::get_location(){
    int a;
    int b;

    b = 0;
    for (size_t i = 0; i < _configs._locations.size(); i++){
        a = compare_str(_request._path, _configs._locations[i]._path);
        if (a > b)
        {
            this->_location = _configs._locations[i];
            b = a;
        }
    }
}

void    Response::get_the_absolute_path(){
    if (isDirectory(this->_path)){
        if (this->_path[_path.size() - 1] != '/'){
            this->_path += "/";
            this->_status = 301;
            this->_dir_or_file = true;
            return ;
        }
        this->_dir_or_file = true;
    }
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
    _path += this->_location._root;
    _path += re_st._path;
}

void   Response::set_config(ServerConfig& conf){
    this->_configs = conf;
}

ServerConfig& Response::get_config(){
    return (this->_configs);
}
Response::Response(Request& re_st) : _request(re_st){
}

Response::~Response()
{
}
