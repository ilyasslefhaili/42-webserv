/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorchi <mkorchi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 00:54:52 by ilefhail          #+#    #+#             */
/*   Updated: 2023/03/17 19:21:52 by mkorchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

size_t Response::_change_name = 0;

void Response::fill_directive(){
    _root = _configs._root;
    _root = _location._root.size() > 0 ? _location._root : _configs._root;
    _index = _configs._index;
    _index = _location._index.size() > 0 ? _location._index : _configs._index;
    _allowed_methods = _configs._allowed_methods;
    _allowed_methods =_location._allowed_methods.size() > 0 ? _location._allowed_methods : _configs._allowed_methods;
    _ret = _location._ret;
    _autoindex = _configs._auto_index;
    if (this->_check_location)
        _autoindex = _location._autoindex;
    _upload    = _location._upload;
    _upload_dir = _location._upload_dir;
    _content_type = _request._header["Content-Type"];
	_cgi_path = _configs._cgi_path;
    _cgi_path = _location._cgi_path.size() > 0 ? _location._cgi_path : _configs._cgi_path;
    _max_body_size = _configs._max_body;
    _max_body_size =_location._max_body !=  -1 ? _location._max_body : _configs._max_body;
}

void  Response::get_files_in_dir(){
    DIR*            dir;
    struct dirent*  to_incriment;

	if (this->_path_autoindex[_path_autoindex.size() - 1] != '/')
		this->_path_autoindex += "/";
    dir = opendir(this->_path.c_str()); 
    to_incriment = readdir(dir);
    _body = "<html>\n";
    _body += "<head>Index :  ";
    _body += this->_path;
    _body += "</head>";
    _body += "\n";
    while (to_incriment != NULL){
        if (std::string(to_incriment->d_name) == "." || std::string(to_incriment->d_name) == ".."){
            to_incriment = readdir(dir);
            continue;
        }
        _body += "<a href= \"";
		// std::cout << "request " << this->_path_autoindex << std::endl;
		// std::cout << "d_name " << to_incriment->d_name << std::endl;
        _body += this->_path_autoindex;
        _body +=  to_incriment->d_name;
        _body += "\"> ";
        _body += to_incriment->d_name;
        _body += "</a>";
        to_incriment = readdir(dir);
        _body += "\n";
    }
    if (this->_status == 0)
        this->_status = 200;
    _body += "</html>\n";
    // std::cout<<_body<<std::endl;
    closedir(dir);
}

void    Response::get_index(){
   
    if (this->_index.size() > 0){
        this->_file.open(this->_path + this->_index[0]);
        size_t i = 0;
        while (i < this->_index.size() && this->_file.fail()){
            this->_file.open(this->_path + this->_index[i]);
            i++;
        }
        if (!this->_file.fail()){
            this->_path += this->_index[i];
            this->_file.close();
        }
        else 
            this->_status = 404;
    }
    else if (this->_autoindex)
    {
        this->get_files_in_dir();
        this->_path += "i.html";
        throw (std::exception());
    }
    else{
        this->_status = 403;
        throw (std::exception());
    }
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

void Response::check_status_code(std::string& str){
    if (str.compare(0, 6,"Status") == 0){
        char s[4];
        int i = 0;
        for (i = 0; i < 3;i++)
            s[i] = str[i + 8];
        s[i] = '\0';
        this->_status = atoi(s);
    }
}

void Response::fill_body(){
    if(this->_cgi_path.size() > 0 && (this->_path.find(".php") == this->_path.size() - 4 || this->_path.find(".pl") == this->_path.size() - 3)){
       
            if (access(this->_cgi_path.c_str(), F_OK) != -1 && access(this->_path.c_str(), F_OK) != -1){
                if (access(this->_cgi_path.c_str(), X_OK) != -1){
                    std::string str = cgi_execute(_cgi_path, this->_path, this->_request._env);
                    this->check_status_code(str);
					if (this->_status == 0)
						this->_status = 200;
					if (_cgi_path.find("php-cgi") == _cgi_path.size() - 7)
						this->is_cgi_response = true;
					size_t pos = str.find("\r\n");
					if (pos != std::string::npos)
						str.erase(0, pos + 2);
                    this->_body = str;
                }
                else
                    this->_status = 502;
            }
            else
                this->_status = 502;
    }
    else {
        if (this->_request._client.fd != -1)
            close(this->_request._client.fd);
        this->_request._client.fd = open(this->_path.c_str(), O_RDONLY | O_NONBLOCK);
		if (_request._client.fd < 0)
		{
            this->_status = 500;
			return ;
		}
		_request._client.file_name = _path;
		struct stat buf;
		fstat(_request._client.fd, &buf);
		_request._client.file_size = buf.st_size;
        this->_request._client.is_reading = true;
    }
}


void Response::in_case_of_return(){
    // std::cout<<this->_ret.first<<std::endl;
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
                catch(const std::exception& e){
                    this->_status = 404;
                }
                this->fill_body();    
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
    (void)re_st;
    if (this->_allowed_methods.size() > 0){
        size_t i = 0; 
        for (;i< _allowed_methods.size(); i++){
            if (_allowed_methods[i] == "GET")
                break;
        }
        if (i == _allowed_methods.size()){
            this->_status = 405;
            return ;
        }
    }
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
        this->_status = 404;
        return ;
    }
    if (this->_status == 0)
        this->_status = 200;
    this->fill_body();
}

void    Response::get_index_in_post(){
    if (this->_index.size() > 0){
        size_t i = 0;
        while (i < this->_index.size()){
            if (access((this->_path + this->_index[0]).c_str(), F_OK) == -1)
                    this->_status = 404;
            else{
                this->_path += this->_index[i];
                return;
            }
            i++;
        } 
        throw(std::exception());
    }
    else{
        this->_status = 403;
        throw(std::exception());
    }
}

// if (this->_status == 301 || this->_status == 200){
//                         size_t pos = str.find("\n");
//                         this->_content_type = str.substr(0, pos + 1);
//                         str.erase(0, pos + 1);
//                         if (strncmp(this->_content_type.c_str(), "Content-Type:", 13) != 0){
//                             size_t pos = str.find("\n");
//                             this->_content_type = str.substr(0, pos + 1);
//                             str.erase(0, pos + 1);
//                         }
//                         this->_body = str;
//                     }
void    Response::post_method(){
    if (this->_allowed_methods.size() > 0){
        size_t i = 0; 
        for (;i< _allowed_methods.size(); i++){
            if (_allowed_methods[i] == "POST")
                break;
        }
        if (i == _allowed_methods.size()){
            this->_status = 405;
            return ;
        }
    }
    if (this->_request._body.size() <= _max_body_size){
        if (this->_cgi_path.size() > 0 && (this->_path.find(".php") == this->_path.size() - 4 || this->_path.find(".pl") == this->_path.size() - 3))
        {
            if (this->_dir_or_file)
            {
                try{
                    this->get_index_in_post();
                }catch(std::exception& e){
                    return ;
                }   
            }
            if (access(this->_cgi_path.c_str(), F_OK) != -1){
                if (access(this->_cgi_path.c_str(), X_OK) != -1){
                    std::string str = cgi_execute(_cgi_path, this->_path, this->_request._env);
                    this->check_status_code(str);
                    if (this->_status == 0)
					{
						if (_cgi_path.find("php-cgi") == _cgi_path.size() - 7)
							this->is_cgi_response = true;
                        this->_status = 200;
					}
					else{
						size_t pos = str.find("\r\n");
						if (pos != std::string::npos)
							str.erase(0, pos + 2);
					}
                    this->_body = str;
                }
                else
                    this->_status = 502;
            }
            else
                this->_status = 404;  
        }
        else if (this->_upload){
            if (access(this->_upload_dir.c_str(), F_OK) != -1)
            {
                if (access(this->_upload_dir.c_str(), W_OK) != -1)
	    		{
                    std::string Upload_file = this->_upload_dir;
                    if (isDirectory(this->_path) && this->_path.find(_upload_dir) == 0){
                        Upload_file += "/upload" + std::to_string(_change_name);
                        Upload_file += this->types.get_extention(this->_content_type);
                        _change_name++;
                    }
                    else if (this->_path.find(_upload_dir) == 0)
                        Upload_file = this->_path;
                    else{
                        this->_status = 403;
                        return ;
                    }
	    	   		int fd = open(Upload_file.c_str(), O_CREAT | O_WRONLY | O_NONBLOCK, 0666);
					if (fd < 0)
					{
						this->_status = 500;
					}
					else
					{
						_request._client.file_name = Upload_file;
						// fcntl(fd, F_SETFL, O_NONBLOCK);
						// std::cout << "adsfadsf   " << fd << std::endl;
						_request._client.fd = fd;
						_request._client.is_saving = true;
						_request._client.total_bytes_saved = 0;
						
       					if (this->_status == 0)
       					    this->_status = 201;
					}
                }
                else
                	this->_status = 403;
            }
            else
                this->_status = 404;
        }
        else 
            this->_status = 403;
    }
    else
        this->_status = 413;
    this->_content_type = "";
}

    //    if (this->_request._client.fd != -1)
    //         close(this->_request._client.fd);
    //     this->_request._client.fd = open(this->_path.c_str(), O_RDONLY | O_NONBLOCK);
	// 	if (_request._client.fd < 0)
	// 	{
	// 		return ;
	// 	}
	// 	// fcntl(_request._client.fd, F_SETFL, O_NONBLOCK);
	// 	_request._client.file_name = _path;
	// 	struct stat buf;
	// 	fstat(_request._client.fd, &buf);
	// 	_request._client.file_size = buf.st_size;
    //     this->_request._client.is_reading = true;

void Response::get_error_page(){
    // std::cout<<"status    " <<this->_status<<std::endl;
   this->_error_page = this->_configs._error_pages[this->_status];
	
	
   if (!_error_page.empty())
   {
		if (this->_request._client.fd != -1)
	    	close(this->_request._client.fd);
		this->_request._client.fd = open(this->_error_page.c_str(), O_RDONLY | O_NONBLOCK);
		if (_request._client.fd < 0)
			_status = 404;
		else
		{
			// fcntl(_request._client.fd, F_SETFL, O_NONBLOCK);
			_request._client.file_name = this->_path;
			struct stat buf;
			fstat(_request._client.fd, &buf);
			_request._client.file_size = buf.st_size;
			// std::cout<<"len   = "<<buf.st_size<<std::endl;
			_request._client.is_reading = true;
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
	this->_path_autoindex = this->_request._path;
    this->_request._path.erase(0, this->_location._path.size());
}

void    Response::get_the_absolute_path(){
    if (isDirectory(this->_path)){
        if (this->_path[_path.size() - 1] != '/' && this->_request._method != "DELETE"){
            this->_path += "/";
            this->_location._path += "/";
            this->_status = 301;
            this->_dir_or_file = true;
            return ;
        }
        this->_dir_or_file = true;
    }
	else
        this->_dir_or_file = false;
}

std::string Response::get_body(){
    if (this->_status >= 400 && this->_body.size() == 0 && this->_error_page.empty())
        return std::to_string(this->_status);
    return _body;
}
void Response::set_body(std::string body){
    this->_body = body;
}
std::string Response::get_content_type(){
    if (this->_status >= 400)
        return ("Content-Type: text/html\r\n");
    else if (this->_request._method == "POST")
        return ("");
    else if (this->_request._method == "DELETE")
        return ("");
    else if (this->_cgi_path.size() > 0&& (_path.find(".php") == _path.size() - 4 || _path.find(".pl") == _path.size() - 3))
        return (this->_content_type);
    return (this->types.get_type(this->_path));
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
	this->_path = this->_root;
    if (this->_root.find("/") == this->_root.size() - 1 && re_st._path.find("/") == 0)
        re_st._path.erase(0, 1);
	else if (this->_root.find("/") == std::string::npos && re_st._path.find("/") == std::string::npos)
		_path += "/";
	size_t pos = re_st._path.find("?");
	if (pos != std::string::npos)
	{
		_path += re_st._path.substr(0, pos);
		_query = re_st._path.substr(pos + 1);
		re_st._path = _path;
	} else {
		_path += re_st._path;
	}
	
}

void   Response::set_config(ServerConfig& conf){
    this->_configs = conf;
}

ServerConfig& Response::get_config(){
    return (this->_configs);
}
Response::Response(Request& re_st) : _request(re_st){
	this->_status = 0;
	this->is_cgi_response = false;
    this->_check_location = false;
}

Response::~Response()
{
}

bool check_empty_dir(std::string path){
    struct stat dir_info;

    stat(path.c_str(), &dir_info);
    if (dir_info.st_size == 68)
        return true;
    return false;
}

bool delete_folder(std::string path){
    DIR*  dir = opendir(path.c_str());

    struct  dirent *rdd;
    while ((rdd = readdir(dir))){    
        if (strcmp(rdd->d_name, ".") != 0 && strcmp(rdd->d_name, "..") != 0){
            std::string str = path + "/";
            str += rdd->d_name;
            if (isDirectory(str)){
                if (!check_empty_dir(str)){
                    if (delete_folder(str) == false)
                        return false;
                    if (rmdir(str.c_str()) == -1)
                        return false;
                }
                else{
                    if (rmdir(str.c_str()) == -1)
                        return false;
                    return true;
                }
            }else{
                if (unlink(str.c_str()) == -1)
                    return (false);
            }
        }
    }
    return true;
}

void Response::delete_response(){
    if (this->_allowed_methods.size() > 0){
        size_t i = 0; 
        for (;i< _allowed_methods.size(); i++){
            if (_allowed_methods[i] == "DELETE")
                break;
        }
        if (i == _allowed_methods.size()){
            this->_status = 405;
            return ;
        }
    }
    else{
        this->_status = 405;
        return ;
    }
    if (access(this->_path.c_str(), F_OK) == 0){
        if (isDirectory(this->_path)){
            if (this->_path[this->_path.size() - 1]!= '/'){
                this->_status = 409;
            }
            else if (access(this->_path.c_str(), W_OK) != 0){
                this->_status = 403;
            }
            else{
                if (!delete_folder(this->_path))
                    this->_status = 500;
                else
                    this->_status = 204;
            }
        }else{
            unlink(this->_path.c_str());
            this->_status = 204;
        }
    }
    else 
        this->_status = 404;
}