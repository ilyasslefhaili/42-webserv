#include <iostream>
#include <fstream>
#include "Response.hpp"
#include "Mimetypes.hpp"
int main()
{  
    MimeTypes l;

    std::cout<<l.get_extention("image/jpeg");
}
