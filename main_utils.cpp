
# include "main.hpp"

void	init_client(ClientInfo &client)
{
	bzero(&client, sizeof(ClientInfo));
	client.address_length = sizeof(client.address);
	client.request = (char *) calloc(BASE_REQUEST_SIZE, sizeof(char));
	client.capacity = BASE_REQUEST_SIZE;
	client.is_receiving = false;
	client.is_saving = false;
	client.is_reading = false;
	client.fd = -1;
	client.received = 0;
}

void    check_incoming_connections(std::pair<fd_set, fd_set> &fds, std::vector<Server> &servers)
{
    fds = Server::wait_on_clients(servers);
	// std::cout << "select again" << std::endl;
    std::vector<Server>::iterator s = servers.begin();
    std::vector<Server>::iterator ite = servers.end();
    while (s != ite)
    {
        if (FD_ISSET(s->get_socket(), &fds.first)) // will return true if file descriptor was flagged by select
        {
            ClientInfo client;
			init_client(client);
            client.socket = accept(s->get_socket(),
                (struct sockaddr*) &(client.address),
                &(client.address_length));
			fcntl(client.socket, F_SETFL, O_NONBLOCK);
            client.last_received = time(NULL);
			s->clients.push_back(client);
			if (client.socket < 0) {
				std::cerr << "accept() failed. (" << errno << ") " << strerror(errno) << std::endl;
				exit(1);
			}
			std::cout << "######################################################" << std::endl;
			std::cout << "New connection from " << s->get_client_address(client)
				<< " : " << s->get_port() << " using socket " << client.socket << std::endl;
			std::cout << "####################################################" << std::endl;
        }
        s++;
    }
}


bool	read_chunk_file(std::vector<ClientInfo>::iterator &it)
{
	if (it->response.empty())
	{
		std::cout << "reading file" << std::endl;
		ssize_t  r;
		size_t byte_to_read = it->file_size - it->total_bytes_read;
		if (byte_to_read > CHUNK_SIZE_SEND)
			byte_to_read = CHUNK_SIZE_SEND;
		char s[byte_to_read];
		r = read(it->fd, s, byte_to_read);
		if (r < 0)
		{
			if (it->fd != -1)
				close(it->fd);
			it->fd = -1;
			it->is_reading = false;
			it->is_receiving = true;
			it->force_drop_connection = true;
			return true ;
		}
		it->total_bytes_read += r;
		// std::cout << "total bytes read: " << r << std::endl;
		it->response = std::string(s, r);
		if (it->total_bytes_read == it->file_size)
		{
			// std::cout << "finished reading " << std::endl;
			it->is_reading = false;
			close(it->fd);
			it->fd = -1;
			it->total_bytes_read = 0;
		}
	}
	it->is_receiving = true;
	return false ;
}

bool	save_chunk_file(std::vector<ClientInfo>::iterator &it)
{
	// std::cout << "savin file" << std::endl;
	ssize_t r;
	bool failed = false;
	size_t bytes_to_write = it->request_obj->_body_len - it->total_bytes_saved;
	if (bytes_to_write > CHUNK_SIZE_SAVE)
		bytes_to_write = CHUNK_SIZE_SAVE;
	// std::cout << it->fd << std::endl;
	r = write(it->fd, it->request_obj->_body.c_str() + it->total_bytes_saved,
		bytes_to_write);
	if (r < 0)
	{
		std::cout << strerror(errno) << " " << errno << std::endl;
		if (it->fd != -1)
			close(it->fd);
		it->fd = -1;
		unlink(it->file_name.c_str());
		it->force_drop_connection = true;
		it->is_receiving = true;
		it->is_saving = false;
		return true ;
	}
	it->total_bytes_saved += r;
	std::cout << "bytes were saved: " << r << std::endl;
	if (it->total_bytes_saved == it->request_obj->_body_len)
	{
		it->is_saving = false;
		close(it->fd);
		it->fd = -1;
		it->total_bytes_saved = 0;
		it->is_receiving = true;
	}
	return false ;
}

void    check_incoming_requests(std::pair<fd_set, fd_set> &fds, std::vector<Server> &servers, char** env)
{
    std::vector<Server>::iterator server = servers.begin();
    std::vector<Server>::iterator end = servers.end();
    while (server != end)
    {
        std::vector<ClientInfo>::iterator it = server->clients.begin();
        std::vector<ClientInfo>::iterator e = server->clients.end();
        while (it != e)
        {
			if (!it->is_receiving && it->is_reading && FD_ISSET(it->fd, &fds.first))
			{
				int r = read_chunk_file(it);
				if (r)
				{
					server->send_500(*it);
					++it;
					continue ;
				}
			}
			else if (it->is_receiving && FD_ISSET(it->socket, &fds.second))
			{
				std::cout << "retrying receiving "<< std::endl;
				bool r = server->send_data(*it); // if true keep connection
				if (!r)
				{
						std::cout << "dropping client" << std::endl;
					if (it->fd != -1)
						close(it->fd);
				    it = server->drop_client(*it);
					e = server->clients.end();
                    continue ;
				}
			}
			else if (it->is_saving && FD_ISSET(it->fd, &fds.second))
			{
				bool r = save_chunk_file(it);
				if (r)
				{
					server->send_500(*it);
					++it;
					continue ;
				}
			}
			else if (FD_ISSET(it->socket, &fds.first))
            {
                bool r = server->receive_request(it, env, fds); // return whether should close connection or not
				if (r)
                {
                    e = server->clients.end();
                    continue ;
                }
            }
			else
			{
                if (time(NULL) - it->last_received > TIMEOUT)
                {
                    std::cout << "timeout; socket fd :" << it->socket << std::endl;
                    it = server->drop_client(*it);
                    e = server->clients.end();
                    continue ;
                }
            }
            ++it;
        }
        ++server;
    }
}
