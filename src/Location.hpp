

#ifndef LOCATION_HPP


# define LOCATION_HPP

# include <string>
# include <vector>
# include <map>

struct Location {
	std::string							_path; 
	std::string							_root;
	std::vector<std::string>			_index;
	bool								_autoindex;
	bool								_is_autoindex_set;
	std::pair<int, std::string>			_ret;
	std::vector<std::string>			_allowed_methods;
	std::string							_cgi_path;
	bool								_upload;
	std::string							_upload_dir;
	ssize_t								_max_body;
	std::map<int, std::string>			_error_pages;


	Location()
	{
		_ret.first = -1;
		_upload = false;
		_is_autoindex_set = false;
		_autoindex = false;
	}
};

#endif