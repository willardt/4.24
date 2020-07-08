#include "TCP_Server.h"

#include <iostream>

void print_new_connection(SOCKET s, int id) {
	const auto socket_info = get_socket_info(s);
	const auto socket_addr = get_socket_addr(s, socket_info);

	std::cout << "*New Client Connected*" << '\n';
	fmt_out("id", id);
	fmt_out("Address", socket_addr);
	fmt_out("Port", socket_info.sin_port);
	fmt_out("Family", socket_info.sin_family);
}

TCP_Server::TCP_Server(const int af, const int protocol, const int port) :
	_shutdown		( false ),
	_port			( port )
{
	if (WSAStartup(MAKEWORD(2, 2), &_wsa_data)) {
		throw "Couldn't Init Server with WSAStartup";
	}

	_server.net_socket(af, SOCK_STREAM, protocol);
	_server.net_bind(port);
	_server.net_listen();
	print_server(_server.get(), af, SOCK_STREAM, protocol, port);
}

TCP_Server::~TCP_Server() {
	WSACleanup();
}

void TCP_Server::net_send_all(const char* data) {
	_mutex.lock();
	for(auto c : _clients) {
		net_send(data, c->socket.get());
	}
	_mutex.unlock();
}

void TCP_Server::net_send(const char* data, SOCKET s) {
	if (s == INVALID_SOCKET) {
		std::cout << "Failed to send " << data << '\n';
		std::cout << "INVALID SOCKET" << '\n';
		return;
	}

	send(s, data, strlen(data) + 1, 0);
}

// Puts server in loop till another socket calls net_connect on it
// Needs to run in seperate thread
void TCP_Server::net_accept() {
	while (!_shutdown) {
		std::cout << "Watiing for client..." << '\n';

		std::shared_ptr<Client_Socket> client = std::make_shared<Client_Socket>();
		client->socket.net_accept(_server);

		_mutex.lock();
		_clients.push_back(client);
		_mutex.unlock();

		client->thread = std::thread(&TCP_Server::net_recieve, this, client);
		static size_t id = 0;
		client->id = id++;

		print_new_connection(client->socket.get(), id);
	}
}

void TCP_Server::net_recieve(std::shared_ptr<Client_Socket> client) {
	const auto socket_info = get_socket_info(client->socket.get());
	const auto socket_addr = get_socket_addr(client->socket.get(), socket_info);
	int result;
	do {
		char buf[MAX_MSG_LEN];
		result = recv(client->socket.get(), buf, MAX_MSG_LEN, 0);
		if (result > 0) {
			fmt_out("Client", socket_addr);
			fmt_out("Recieved", result);

			net_send_all(buf);

			std::cout << buf << '\n';
			// handle msg and respond 

			net_send(buf, client->socket.get());
		}
		else {
			fmt_out("Client Disconnected", socket_addr);
			std::lock_guard<std::mutex> lock_guard(_mutex);
			for (auto it = _clients.begin(); it != _clients.end(); ++it) {
				if (*it == client) {
					client->thread.detach();
					_clients.erase(it);
					break;
				}
			}

		}
	} while (result > 0);
}

void TCP_Server::shutdown() {
	_shutdown = true;
}