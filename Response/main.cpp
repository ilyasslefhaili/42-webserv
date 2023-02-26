#include <iostream>
#include <fstream>
#include "Response.hpp"
int main()
{
    Request l;

    l._path = "./l";
    l._method = "GET";
    std::cout<<get_response(l)<<std::endl;
}