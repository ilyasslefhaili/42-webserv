/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorchi <mkorchi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 00:49:50 by ilefhail          #+#    #+#             */
/*   Updated: 2023/03/13 16:12:11 by mkorchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <iostream>
#include "../src/Server.hpp"
#include "../src/Request.hpp"
#include <fstream>
#include <unistd.h>
#include <fcntl.h>
#include "../src/Location.hpp"
#include "../src/ServerConfig.hpp"
#include "../src/Config.hpp"
#include <dirent.h>
#include <sys/types.h>
#include "Mimetypes.hpp"
#include <sys/stat.h>
#include <cstdlib>


class Response
{
    private:
        int                                                 _status;
        // char**                                              _env;
        ssize_t                                             _max_body_size;                
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
        std::string                                          _content_type;
        std::vector<std::string>                            _allowed_methods;
        std::string                                         _cgi_path;
        std::string                                         _cgi_header;
        bool                                                _upload;
        std::string                                         _upload_dir;
        std::pair<int, std::string>                 		_ret;
		static size_t										_change_name;
    public:
        MimeTypes   types;
        std::string get_body();
        void fill_body();
        void in_case_of_return();
        std::string &get_path();
        void    fill_directive();
        void    get_index();
        void set_body(std::string body);
        void    post_method();
        void    get_index_in_post();
        void get_files_in_dir();
        std::string get_content_type();
        void set_content_type(std::string type);
        void fill_attributes(Request& re_st);
        void link_root_path(Request& re_st);
        void get_error_page();
        void check_status_code(std::string& str);
        void set_status(int status);
        int get_status();
        void set_config(ServerConfig& conf);
        ServerConfig& get_config();
        void    get_the_absolute_path();
        void    get_location();
        std::vector<std::string>     set_env();
        void    delete_response();
        std::string cgi_execute(std::string cgi_path, std::string file, char **env);
        Response(Request& re_st);
        ~Response();
        
};

bool    delete_folder(std::string path);
ServerConfig& get_server(Request& re_st,  std::vector<ServerConfig> &configs);
// std::string content_from_path(std::string& path);
std::string get_response(Request& re_st);
std::string create_status_line(int status);
std::string get_content_lenght(Response &a);
std::string get_response(Request& re_st, std::vector<ServerConfig> &configs);
Response* get_response_object(Request& re_st, std::vector<ServerConfig> &configs);
std::vector<std::string> split_host_port(std::string host_port);
bool isDirectory(const std::string& path);
void    give_error_page(Request& re_st, std::vector<ServerConfig> &configs, int status);


#endif
