/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorchi <mkorchi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 00:49:50 by ilefhail          #+#    #+#             */
/*   Updated: 2023/02/27 20:08:08 by mkorchi          ###   ########.fr       */
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
class Response
{
    private:
        std::string     _content_type;
        int             _status;
        //size_t          _cl;
        ServerConfig    _configs;
        std::fstream    _file;
        std::string     _path;
        std::string     _body;
    public:
        std::string get_body();
        void set_body(std::string body);
        std::string get_content_type();
        void set_content_type(std::string type);
        void fill_attributes(Request& re_st);
        void link_root_path(Request& re_st);
        void get_error_page();
        void set_status(int status);
        int get_status();
        void set_config(ServerConfig& conf);
        ServerConfig& get_config();
        Response(/* args */);
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



#endif
