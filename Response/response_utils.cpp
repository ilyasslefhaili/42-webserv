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

std::string content_from_path(std::string& path){
    size_t pos = path.rfind('.');
    if (pos != std::string::npos) {
        std::string last_dot = path.substr(pos, path.size() - pos);
        if (last_dot == ".css")
            return "Content-Type: text/css\n\r";
        else if (last_dot == ".csv")
            return "Content-Type: text/csv\n\r";
        else if (last_dot == ".gif")
            return "Content-Type: image/gif\n\r";
        else if (last_dot == ".htm")
            return "Content-Type: text/html\n\r";
        else if (last_dot == ".html")
            return "Content-Type: text/html\n\r";
        else if (last_dot ==  ".ico")
            return "Content-Type: image/x-icon\n\r";
        else if (last_dot == ".jpeg")
            return "Content-Type: image/jpeg\n\r";
        else if (last_dot == ".jpg")
            return ("Content-Type: image/jpeg\n\r");
        else if (last_dot == ".js")
            return "Content-Type: application/javascript\n\r";
        else if (last_dot == ".json")
            return "Content-Type: application/json\n\r";
        else if (last_dot == ".png")
             return "Content-Type: image/png\n\r";
        else if (last_dot == ".pdf")
            return "Content-Type: application/pdf\n\r";
        else if (last_dot == ".svg")
            return "Content-Type: image/svg+xml\n\r";
        else if (last_dot == ".txt")
            return "Content-Type: text/plain\n\r";
    }
    return "Content-Type: application/octet-stream\n\r";
}