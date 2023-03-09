/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorchi <mkorchi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 00:54:52 by ilefhail          #+#    #+#             */
/*   Updated: 2023/03/09 17:00:00 by mkorchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"


void Response::fill_directive(){
    _root = _configs._root;
    _root = _location._root.size() > 0 ? _location._root : _configs._root;
    _index = _configs._index;
    _index = _location._index.size() > 0 ? _location._index : _configs._index;
    _allowed_methods = _location._allowed_methods;
    _ret = _location._ret;
    _autoindex = _configs._auto_index;
    _autoindex = _location._autoindex;
    _upload    = _location._upload;
    _upload_dir = _location._upload_dir;
    _content_type = _request._header["Content-Type"];
}

void  Response::get_files_in_dir(){
    DIR*            dir;
    struct dirent*  to_incriment;

    dir = opendir(this->_path.c_str());
    to_incriment = readdir(dir);
    _body = "<html>\n";
    _body += "<head>Index ";
    _body += this->_path;
    while (to_incriment != NULL){
        _body += "<a href= \"";
        _body += _request._path;
        _body += "/";
        _body += to_incriment->d_name;
        _body += "\"> ";
        _body += to_incriment->d_name;
        _body += "</a>";
        to_incriment = readdir(dir);
        _body += "\n";
    }
    if (this->_status == 0)
        this->_status = 200;
    _body += "</html>\n";
}

void    Response::get_index(){
    if (this->_index.size() > 0){
        this->_file.open(this->_path + this->_index[0]);
        size_t i = 0;
        while (i < this->_index.size()&& this->_file.fail()){
            this->_file.open(this->_path + this->_index[i]);
        }
        if (!this->_file.fail())
            this->_file.close();
        else 
            this->_status = 404;
        this->_path += this->_index[i];
    }
    else if (this->_autoindex)
    {
        this->get_files_in_dir();
        this->_path += "i.html";
        throw (std::exception());
    }
    else
        this->_status = 404;
}

std::string& Response::get_path(){
    return (_path);
}

void check_the_file_permissions(std::string& path, int *status){
    int reslt = access(path.c_str(), F_OK);
    if (reslt == 0){
        reslt = access(path.c_str(), R_OK);
        if (reslt != 0){
            *status = 401;
        }
    }
    else
        throw (std::exception());
}

void Response::file_body(){
    this->_file.open(this->_path);
    std::string str;
    while (!this->_file.fail() && !this->_file.eof()){
     std::getline(this->_file, str);
     this->_body += str;
     if (!this->_file.eof())
         this->_body += "\n";
    }
}

void Response::in_case_of_return(){
    if (this->_ret.first != -1 || this->_ret.second != ""){
        if (this->_ret.first != -1){
            this->_status = this->_ret.first;
            if (this->_status < 400 && this->_status >= 300 && this->_ret.second != "")
            {
                this->_path = this->_ret.second;
                try
                {
                    check_the_file_permissions(this->_path, &this->_status);
                }
                catch(const std::exception& e)
                {
                    this->_status = 404;
                }
                this->set_content_type(content_from_path(this->_path));  
                this->file_body();    
            }
            else{
                this->_status = this->_ret.first;
                this->_body = this->_ret.second;
            }
        }
        else
            this->_body = this->_ret.second;
        throw(std::exception());
    }
}

void Response::fill_attributes(Request& re_st){
    if (this->_dir_or_file)
    {
        try{
            this->get_index();
        }catch(std::exception& e){
            return ;
        }
    }
    try{
        check_the_file_permissions(this->_path, &this->_status);
    }
    catch(const std::exception& e){
        std::cout<<"file not found"<<std::endl;
        this->_status = 404;
        return ;
    }
    if (this->_status == 0)
        this->_status = 200;
    this->file_body();
}

void    Response::get_index_in_post(){
    if (this->_index.size() > 0){
        this->_file.open(this->_path + this->_index[0]);
        size_t i = 0;
        while (i < this->_index.size()&& this->_file.fail()){
            this->_file.open(this->_path + this->_index[i]);
        }
        if (!this->_file.fail())
            this->_file.close();
        else 
            this->_status = 404;
        this->_path += this->_index[i];
    }
    else
        this->_status = 404;
}

void    Response::post_method(){
    if (!this->_upload)
    {
        if (this->_dir_or_file)
            this->get_index_in_post();
        try{
            check_the_file_permissions(this->_path, &this->_status);
        }catch(std::exception& e){
            std::cout<<"file not found"<<std::endl;
            this->_status = 404;
            return ;
        }
    }
    else{
        std::string Upload_file = this->_upload_dir;
        Upload_file += "/upload.";
        Upload_file += this->types.get_extention(this->_content_type);
        // std::ofstream to_upload;
        // to_upload.open(Upload_file);
        // to_upload<<this->_request._body;
        int fd = open(Upload_file.c_str(), O_CREAT);
        write(fd, this->_request._body.c_str(), atoi(this->_request._header["Content-Length"].c_str()));
        this->_status = 201;
    }
}

void Response::get_error_page(){
   this->_error_page = _root;
   this->_error_page += this->_configs._error_pages[this->_status];
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
    if (b[i] != '\0')
        i = 0;
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
            this->_check_location = true;
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
    _path = this->_root;
    _path += re_st._path;
}

void   Response::set_config(ServerConfig& conf){
    this->_configs = conf;
}

ServerConfig& Response::get_config(){
    return (this->_configs);
}
Response::Response(Request& re_st) : _request(re_st){
    this->_status = 0;
    this->_check_location = false;
}

Response::~Response()
{
}
