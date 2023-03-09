/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response_utils.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorchi <mkorchi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 17:10:05 by ilefhail          #+#    #+#             */
/*   Updated: 2023/03/09 18:45:14 by mkorchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Response.hpp"

bool isDirectory(std::string& path){
    DIR*  dir = opendir(path.c_str());
    if (dir != NULL){
        closedir(dir);
        return true;
    }
    return false;
}

std::string create_status_line(int status, Request&re_st){
    if (status == 404)
        return (re_st._protocol_ver + " 404 Not Found\r\n");
    else if (status == 200)
        return (re_st._protocol_ver + " 200 OK\r\n");
    else if (status == 301)
        return (re_st._protocol_ver + " 301 Moved Permanently\r\n");
    else if (status == 401)
        return (re_st._protocol_ver + " 401 Unauthorized\r\n");
    return "";
}

std::string get_content_lenght(Response &a){
    std::string str = "content-length: " + std::to_string(a.get_body().size());
    return (str + "\r\n");
}

std::string get_response(Request& re_st, std::vector<ServerConfig> &configs){
    std::string response;
    Response &a = get_response_object(re_st, configs);

    response += create_status_line(a.get_status(), re_st);
    response += content_from_path(a.get_path());
    response += get_content_lenght(a);
    response += "\r\n";
    response += a.get_body();
    // std::cout<<response<<std::endl;
    return response;
}

Response& get_response_object(Request& re_st, std::vector<ServerConfig> &configs){
    Response *a = new Response(re_st);

    if (re_st._method == "GET"){
        a->set_config(get_server(re_st, configs));
        a->get_location();
        a->fill_directive();
        try
        {
            a->in_case_of_return();
        }
        catch(const std::exception& e)
        {
            return (*a);
        }
        a->link_root_path(re_st);
        a->get_the_absolute_path();
        a->set_content_type(content_from_path(re_st._path));  
        a->fill_attributes(re_st);
    } 
    return *a;
}
//get content type 
std::string content_from_path(std::string& path){
    size_t pos = path.rfind('.');
    if (pos != std::string::npos) {
        std::string last_dot = path.substr(pos, path.size() - pos);
        if (last_dot == ".css")
            return "Content-Type: text/css\r\n";
        else if (last_dot == ".csv")
            return "Content-Type: text/csv\r\n";
        else if (last_dot == ".gif")
            return "Content-Type: image/gif\r\n";
        else if (last_dot == ".htm")
            return "Content-Type: text/html\r\n";
        else if (last_dot == ".html")
            return "Content-Type: text/html\r\n";
        else if (last_dot ==  ".ico")
            return "Content-Type: image/x-icon\r\n";
        else if (last_dot == ".jpeg")
            return "Content-Type: image/jpeg\r\n";
        else if (last_dot == ".jpg")
            return ("Content-Type: image/jpeg\r\n");
        else if (last_dot == ".js")
            return "Content-Type: application/javascript\r\n";
        else if (last_dot == ".json")
            return "Content-Type: application/json\r\n";
        else if (last_dot == ".png")
             return "Content-Type: image/png\r\n";
        else if (last_dot == ".pdf")
            return "Content-Type: application/pdf\r\n";
        else if (last_dot == ".svg")
            return "Content-Type: image/svg+xml\r\n";
        else if (last_dot == ".txt")
            return "Content-Type: text/plain\r\n";
    }
    return "Content-Type: application/octet-stream\r\n";
}

//split the host and the port
std::vector<std::string> split_host_port(std::string host_port){
    size_t pos = 0;
    std::vector<std::string> to_ret;
    while ((pos = host_port.find(":")) != std::string::npos) {
        to_ret.push_back(host_port.substr(0, pos));
        host_port.erase(0, pos + 1);
    }
    to_ret.push_back(host_port);
    return (to_ret);
}
//find the matching config
ServerConfig& get_server(Request& re_st,  std::vector<ServerConfig> &configs){
    std::vector<std::string> host_vec = split_host_port(re_st._header["Host"]);
    for (size_t i = 0; i < configs.size();i++){
        if (host_vec.at(0) == configs[i]._server_name)
            return (configs[i]);
    }
    return (configs[0]);
}
