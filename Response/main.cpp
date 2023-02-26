#include <iostream>
#include <fstream>
#include "Response.hpp"
int main()
{
    Request l;

    l._path = "a.html";
    l._method = "POST";
    l._body = "hello world";
    std::cout<<get_response(l)<<std::endl;
}
