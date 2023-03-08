/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorchi <mkorchi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 00:49:50 by ilefhail          #+#    #+#             */
/*   Updated: 2023/03/08 15:48:23 by mkorchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <iostream>
#include "../src/Server.hpp"
#include "../src/Request.hpp"
#include <fstream>
#include "../src/Location.hpp"
#include "../src/ServerConfig.hpp"
#include "../src/Config.hpp"
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>


class Response
{
    private:
        std::string                                         _content_type;
        int                                                 _status;
        //size_t          _cl;                      
        bool                                                _dir_or_file;
        ServerConfig                                        _configs;
        std::fstream                                        _file;
        bool                                                _check_location;
        std::string                                         _path;
        std::string                                         _body;
        Request&                                            _request;
        Location                                            _location;
        std::string                                         _error_page;
        std::string                                         _root;
        std::vector<std::string>                             _index;
        bool                                                 _autoindex;
        std::vector<std::string>                            _allowed_methods;
        std::vector<std::string>                            _cgi_path;
        std::vector<std::string>                            _cgi_ext;
        std::pair<std::string, std::string>                 _ret;

    public:
        std::string get_body();
        void file_body();
        void in_case_of_return();
        std::string &get_path();
        void    fill_directive();
        void    get_index();
        void set_body(std::string body);
        void get_files_in_dir();
        std::string get_content_type();
        void set_content_type(std::string type);
        void fill_attributes(Request& re_st);
        void link_root_path(Request& re_st);
        void get_error_page();
        void set_status(int status);
        int get_status();
        void set_config(ServerConfig& conf);
        ServerConfig& get_config();
        void    get_the_absolute_path();
        void    get_location();
        Response(Request& re_st);
        ~Response();
        
};

ServerConfig& get_server(Request& re_st,  std::vector<ServerConfig> &configs);
std::string content_from_path(std::string& path);
std::string get_response(Request& re_st);
std::string create_status_line(int status);
std::string get_content_lenght(Response &a);
std::string get_response(Request& re_st, std::vector<ServerConfig> &configs);
Response&   get_response_object(Request& re_st, std::vector<ServerConfig> &configs);
std::vector<std::string> split_host_port(std::string host_port);
bool        isDirectory(std::string& path);



#endif
