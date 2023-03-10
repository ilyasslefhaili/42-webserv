#include <iostream>
#include <fstream>
#include "Response.hpp"
#include "Mimetypes.hpp"
int main(int ac, char **av, char **env)
{  
    MimeTypes l;

    (void)av;
    ac = 0;
    std::cout<<cgi_execute("../php-cgi", "../i.php", env)<<std::endl;
}
