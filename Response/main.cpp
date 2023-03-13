#include <iostream>
#include <fstream>
#include "Response.hpp"
#include "Mimetypes.hpp"
int main(int ac, char **av, char **env)
{  

    (void)av;
    (void)env;

    std::string str = "./a.out.dSYM";
    // std::cout<<isDirectory(str)<<std::endl;
    delete_folder(str);
    ac = 0;
}
