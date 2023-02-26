/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilefhail <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/26 00:49:50 by ilefhail          #+#    #+#             */
/*   Updated: 2023/02/26 00:54:47 by ilefhail         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <iostream>
#include "../models/Request.hpp"
#include <fstream>

class Response
{
    private:
        std::string     _content_type;
        int             _status;
        //size_t          _cl;
        std::fstream    _file;
        std::string     _body;
    public:
        std::string get_body();
        void set_body(std::string body);
        std::string get_content_type();
        void set_content_type(std::string type);
        void fill_attributes(Request& re_st);
        void set_status(int status);
        int get_status();
        Response(/* args */);
        ~Response();
        
};

Response& get_response_object(Request& re_st);
std::string content_from_path(std::string& path);
std::string get_response(Request& re_st);
std::string create_status_line(int status);
std::string get_content_lenght(Response &a);
std::string get_response(Request& re_st);
Response& get_response_object(Request& re_st);



#endif
