/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response_utils.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilefhail <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 17:10:05 by ilefhail          #+#    #+#             */
/*   Updated: 2023/02/25 17:10:07 by ilefhail         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Response.hpp"

std::string create_status_line(int status){
    if (status == 404)
        return ("HTTP/1.1 404 Not Found\r\n");
    else if (status == 200)
        return ("HTTP/1.1 200 OK\r\n");
    return "";
}

std::string get_content_lenght(Response &a){
    std::string str = "content-length: " + std::to_string(a.get_body().size());
    return (str + "\r\n");
}

std::string get_response(Request& re_st){
    Response &a = get_response_object(re_st);
    std::string response;

    response += create_status_line(a.get_status());
    response += content_from_path(re_st._path);
    response += get_content_lenght(a);
    response += "\r\n";
    response += a.get_body();
    return response;
}

Response& get_response_object(Request& re_st){
    Response *a = new Response();

    a->set_content_type(content_from_path(re_st._path));
    a->fill_attributes(re_st);
    return *a;
}

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