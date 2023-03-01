#include <iostream>
#include <fstream>
#include "Response.hpp"
int main()
{
    std::string e = "95409549:000";
    std::vector<std::string> a = split_host_port(e);
    std::cout<< a[0] << "    "<< a[1]<<std::endl;

    
}
