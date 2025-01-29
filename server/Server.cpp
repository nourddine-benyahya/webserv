#include "Server.hpp"
#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>

#define PORT 8080
#define BUFFER_SIZE 1024

Server::Server() {
	initServer();
}

Server::~Server() {
	close(server_fd);
}

void Server::initServer() {
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == 0) {
		std::cerr << "Socket creation error" << std::endl;
		exit(EXIT_FAILURE);
	}

	int opt = 1;
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
		std::cerr << "Setsockopt error" << std::endl;
		exit(EXIT_FAILURE);
	}

	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
		std::cerr << "Bind error" << std::endl;
		exit(EXIT_FAILURE);
	}

	if (listen(server_fd, 3) < 0) {
		std::cerr << "Listen error" << std::endl;
		exit(EXIT_FAILURE);
	}
}

void Server::run() {
	fd_set master_set, working_set;
	FD_ZERO(&master_set);
	FD_SET(server_fd, &master_set);
	int max_sd = server_fd;

	while (true) {
		memcpy(&working_set, &master_set, sizeof(master_set));
		if (select(max_sd + 1, &working_set, NULL, NULL, NULL) < 0) {
			std::cerr << "Select error" << std::endl;
			exit(EXIT_FAILURE);
		}

		for (int i = 0; i <= max_sd; ++i) {
			if (FD_ISSET(i, &working_set)) {
				if (i == server_fd) {
					int new_socket = accept(server_fd, NULL, NULL);
					if (new_socket < 0) {
						std::cerr << "Accept error" << std::endl;
						exit(EXIT_FAILURE);
					}
					FD_SET(new_socket, &master_set);
					if (new_socket > max_sd) {
						max_sd = new_socket;
					}
				} else {
					char buffer[BUFFER_SIZE];
					int bytes_read = read(i, buffer, sizeof(buffer));
					if (bytes_read <= 0) {
						close(i);
						FD_CLR(i, &master_set);
					} else {
						buffer[bytes_read] = '\0';
						std::cout << "Received: " << buffer << std::endl;
						std::string response = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, world!";
						send(i, response.c_str(), response.size(), 0);
					}
				}
			}
		}
	}
}