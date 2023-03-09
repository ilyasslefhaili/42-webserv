
#ifndef CONFIG_HPP


# define CONFIG_HPP

# include <vector>
# include <string>
# include <cstring>
# include <vector>
# include <iostream>
# include <fstream>
# include <sstream>
# include <stdexcept>

# include "Server.hpp"
# include "ServerConfig.hpp"


// this class will parse the config file and create a config class for every server

class Server;

class Config {
    private:
        std::vector<ServerConfig>	_configs;
        std::string					_file_path;
		bool						_parsing_location;
		bool						_server_bracket_open;
		bool						_location_bracket_open;
		Location					_current_location;
		std::string					_last_key;
		std::string 				_key;
		ServerConfig				_current;

		void						parse_server( std::istringstream &ss );
		void						parse_open_accolade( std::istringstream &ss );
		void						parse_close_accolade( std::istringstream &ss );
		void						parse_listen( std::istringstream &ss );
		void						parse_host( std::istringstream &ss );
		void						parse_server_name( std::istringstream &ss );
		void						parse_autoindex( std::istringstream &ss );
		void						parse_error_page( std::istringstream &ss );
		void						parse_client_max_body_size( std::istringstream &ss );
		void						parse_root( std::istringstream &ss );
		void						parse_index( std::istringstream &ss );
		void						parse_location( std::istringstream &ss );
		void						parse_return( std::istringstream &ss );
		void						parse_methods( std::istringstream &ss );
		void						parse_cgi_path( std::istringstream &ss );
		void						parse_cgi_ext( std::istringstream &ss );
		void						parse_upload( std::istringstream &ss );
		void						parse_upload_dir( std::istringstream &ss );




    public:
        // Config();
        Config(const std::string &file_path);
        ~Config();
        Config(const Config & src);
        Config &operator=(const Config & rhs);

        void    					parse();
        void    					print();
        bool    					is_port_valid(const std::string &port);
        void 						generate_servers(std::vector<Server> &servers);
        void 						init_if_not_set();
        std::vector<ServerConfig>	&get_configs();
		std::string const			&get_key() const;


        class ConfigFileException : public std::exception
        {
			public:
				virtual const char * what() const throw();
        };


};

#endif