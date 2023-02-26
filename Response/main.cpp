#include <iostream>
#include <fstream>
#include "Response.hpp"
int main()
{
    Request l;

    l._path = "./response.cpp";
    l._method = "GET";
    std::cout<<get_response(l)<<std::endl;
}
