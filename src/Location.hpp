

#ifndef LOCATION_HPP


# define LOCATION_HPP

# include <string>
# include <vector>

struct Location {
	std::string							_path; 
	std::string							_root;
	std::vector<std::string>			_index;
	bool								_autoindex;
	bool								_is_autoindex_set;
	std::pair<int, std::string>			_ret;
	std::vector<std::string>			_allowed_methods;
	std::string							_cgi_path;
	std::string							_cgi_ext;
	bool								_upload;
	std::string							_upload_dir;

	Location()
	{
		_ret.first = -1;
		_upload = false;
		_is_autoindex_set = false;
		_autoindex = false;
	}
};

#endif