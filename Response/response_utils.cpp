/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response_utils.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkorchi <mkorchi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 17:10:05 by ilefhail          #+#    #+#             */
/*   Updated: 2023/03/15 12:26:12 by mkorchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Response.hpp"

bool isDirectory(std::string& path){
	// std::cout << path << std::endl;
    struct stat statbuf;
    stat(path.c_str(), &statbuf);
    if (S_ISDIR(statbuf.st_mode))
        return true;
    return false;
}

std::string create_status_line(int status, Request&re_st){
    if (status == 404)
        return (re_st._protocol_ver + " 404 Not Found\r\n");
    else if (status == 201)
        return (re_st._protocol_ver + " 201 Created\r\n");
    else if (status == 200)
        return (re_st._protocol_ver + " 200 OK\r\n");
    else if (status == 301)
        return (re_st._protocol_ver + " 301 Moved Permanently\r\n");
    else if (status == 401)
        return (re_st._protocol_ver + " 401 Unauthorized\r\n");
    else if (status == 502)
        return (re_st._protocol_ver + " 502 Bad Gateway\r\n");
    else if (status == 403)
        return (re_st._protocol_ver + " 403 Forbidden\r\n");
    else if (status == 204)
        return (re_st._protocol_ver + " 204 No Content\r\n");
    else if (status == 500)
        return (re_st._protocol_ver + " 500 Internal Server Error\r\n");
    else if (status == 409)
        return (re_st._protocol_ver + " 409 Conflict\r\n");
    else if (status == 400)
        return (re_st._protocol_ver + " 400 Bad Request\r\n");
    else if (status == 413)
        return (re_st._protocol_ver + " 413 Content Too Large\r\n");
    return "";
}

std::string get_content_lenght(Response &a, Request &re_st){
    std::string str = "Content-Length: ";
	if (re_st._client.is_reading)
		str += std::to_string(re_st._client.file_size);
	else
    	str += std::to_string(a.get_body().size());
    str += "\r\n";
    return (str);
}

std::string get_response(Request& re_st, std::vector<ServerConfig> &configs){
    std::string response;
    std::cout<<"get_response"<<std::endl;
    Response *a = get_response_object(re_st, configs);
    a->get_error_page();
    response += create_status_line(a->get_status(), re_st);
    response += a->get_content_type();
    response += get_content_lenght(*a, re_st);
    response += "\r\n";
    response += a->get_body();
    delete a;
    return response;
}


void    give_error_page(Request& re_st, std::vector<ServerConfig> &configs, int status){
    Response respo(re_st);

    respo.set_config(get_server(re_st, configs));
    respo.get_location();
    respo.set_status(status);
    respo.get_error_page();
}

Response* get_response_object(Request& re_st, std::vector<ServerConfig> &configs){
    Response *a = new Response(re_st);

    
    a->set_config(get_server(re_st, configs));
    a->get_location();
    a->fill_directive();
    a->link_root_path(re_st);
    a->get_the_absolute_path();
    if (re_st._path.size() > 100)
        a->set_status(400);
	else if (re_st._path.find("..") != std::string::npos)
		a->set_status(404);
	else{
        if (re_st._method == "GET"){
            a->fill_attributes(re_st);
        }
        else if (re_st._method == "POST"){
            std::cout<<"POST"<<std::endl;
            a->post_method();
        }else if (re_st._method == "DELETE"){
            std::cout<<"DELETE"<<std::endl;
            a->delete_response();
        }
        else
            a->set_status(400);
    }
    try
    {
        a->in_case_of_return();
    }
    catch(const std::exception& e)
    {
        return (a);
    }
    return a;
}

std::vector<std::string> Response::set_env(){
    std::string a = "SCRIPT_FILENAME=:QUERY_STRING=:REQUEST_METHOD=:CONTENT_TYPE=:CONTENT_LENGTH=:REDIRECT_STATUS=";
    std::vector<std::string> vec_str = split_host_port(a);
    
    vec_str[0] += this->_path;
    vec_str[2] += this->_request._method;
    vec_str[3] += this->_content_type;
    vec_str[4] += std::to_string(this->_request._body.size());
    vec_str[5] += std::to_string(200);
 
    return vec_str;
}

std::string Response::cgi_execute(std::string cgi_path, std::string file, char **env){

	std::cout << "CGI EXECUTING  :"<< file << std::endl;
    int fd[2];
    int fd_r[2];
    int for_k;
    std::vector<std::string> vec_str = set_env();
    char *envp[7];

    for (int i = 0;i < vec_str.size(); i++)
        envp[i] = (char *)vec_str[i].c_str();
    for (int i = 0;i < 6; i++)
        std::cout<<envp[i]<<std::endl;
    envp[6] = NULL;
    std::string buff;
    if (access(file.c_str(), F_OK) != -1)
    {
        char *argv[3] = {(char*)cgi_path.c_str(), (char*)file.c_str(), NULL};
        pipe(fd);
        //pipe(fd_r);
        for_k = fork(); 
        int i = 0;
        // FILE* temp = tmpfile();
        int f_w = open("l", O_CREAT | O_WRONLY | O_TRUNC);
        int f_r = open("l", O_RDONLY);
        if (for_k == 0){
            write(f_w, this->_request._body.c_str(),this->_request._body.size());//w - f
            dup2(f_r, 0);// d - f
            close(f_r);// c -f
            close(f_w);
            dup2(fd[1], 1);
            close(fd[1]);
            close(fd[0]);
            // close(fd_r[0]);
            execve(cgi_path.c_str(), argv, envp);
            write(2, "cgi fail()\n", 12);
            exit(1);
        }
        wait(NULL); 
        char c[2];
        int r = 1;
        close(fd[1]);
        while (r != 0){
            r = read(fd[0], c, 1);
            if (r <= 0)
                break ;
            c[1] = '\0';
            buff += c;
        }
        close(fd[0]);
        // close(fd_r[0]);
        // close(f);
        // close(fd_r[1]);
    }
    std::cout<<"that is the buffer :"<<buff<<std::endl;
    return buff;
}
//get content type 
// std::string content_from_path(std::string& path){
//     size_t pos = path.rfind('.');
//     if (pos != std::string::npos) {
//         std::string last_dot = path.substr(pos + 1, path.size() - pos);
//         if (last_dot == ".css")
//             return "Content-Type: text/css\r\n";
//         else if (last_dot == ".csv")
//             return "Content-Type: text/csv\r\n";
//         else if (last_dot == ".gif")
//             return "Content-Type: image/gif\r\n";
//         else if (last_dot == ".htm")
//             return "Content-Type: text/html\r\n";
//         else if (last_dot == ".html")
//             return "Content-Type: text/html\r\n";
//         else if (last_dot ==  ".ico")
//             return "Content-Type: image/x-icon\r\n";
//         else if (last_dot == ".jpeg")
//             return "Content-Type: image/jpeg\r\n";
//         else if (last_dot == ".jpg")
//             return ("Content-Type: image/jpeg\r\n");
//         else if (last_dot == ".js")
//             return "Content-Type: application/javascript\r\n";
//         else if (last_dot == ".json")
//             return "Content-Type: application/json\r\n";
//         else if (last_dot == ".png")
//              return "Content-Type: image/png\r\n";
//         else if (last_dot == ".pdf")
//             return "Content-Type: application/pdf\r\n";
//         else if (last_dot == ".svg")
//             return "Content-Type: image/svg+xml\r\n";
//         else if (last_dot == ".txt")
//             return "Content-Type: text/plain\r\n";
//     }
//     return "Content-Type: application/octet-stream\r\n";
// }

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

