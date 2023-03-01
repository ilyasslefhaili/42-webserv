

#ifndef LOCATION_HPP


# define LOCATION_HPP

# include <string>
# include <string>


class Location {
    public:
        std::string                 _path; 
        std::string                 _root;
        std::string                 _index;
        bool                        _autoindex;
        std::string                 _ret;
        // char                        _allowed_methds[4];
        std::vector<std::string>    _allowed_methods;
        std::vector<std::string>    _cgi_path;
        std::vector<std::string>    _cgi_ext;


};

#endif