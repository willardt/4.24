#include "TCP_Server.h"

#include <iostream>

TCP_Server::TCP_Server(const int af, const int type, const int protocol, const int port) :
	_shutdown(false),
	_pause(false),
	_port(port)
{
	if (WSAStartup(MAKEWORD(2, 2), &_wsa_data)) {
		throw "Couldn't Init Server with WSAStartup";
	}
	std::cout << "*Starting Server*" << '\n';
	_server.net_socket(af, type, protocol);
	_server.net_bind(port);
	_server.net_listen();

	char name[24];
	gethostname(name, strlen(name));
	const auto socket_info = get_socket_info(_server.get());
	const auto socket_addr = get_socket_addr(_server.get(), socket_info);
	fmt_out("Host Name", name, ENDL);
	print_host_address(name);
	fmt_out("Protocol", protocol, ENDL);
	fmt_out("Type", type, ENDL);
	fmt_out("Port", port, ENDL);
	fmt_out("Family", af, ENDL);
}

TCP_Server::~TCP_Server() {
	WSACleanup();
}

void TCP_Server::net_send(const char* data, SOCKET s) {
	if (s == INVALID_SOCKET) {
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

		const auto socket_info = get_socket_info(client->socket.get());
		const auto socket_addr = get_socket_addr(client->socket.get(), socket_info);

		std::cout << "*New Client Connected*" << '\n';
		fmt_out("id", id, ENDL);
		fmt_out("Address", socket_addr, ENDL);
		fmt_out("Port", socket_info.sin_port, ENDL);
		fmt_out("Family", socket_info.sin_family, ENDL);
	}
}

void TCP_Server::net_recieve(std::shared_ptr<Client_Socket> client) {
	int result;
	do {
		char buf[MAX_MSG_LEN];
		result = recv(client->socket.get(), buf, MAX_MSG_LEN, 0);
		if (result > 0) {
			const auto socket_info = get_socket_info(client->socket.get());
			const auto socket_addr = get_socket_addr(client->socket.get(), socket_info);
			fmt_out("Client", socket_addr, ENDL);
			fmt_out("Recieved", result, ENDL);
			std::cout << buf << '\n';
			// handle msg and respond 

			net_send(buf, client->socket.get());
		}
		else {
			const auto socket_info = get_socket_info(client->socket.get());
			const auto socket_addr = get_socket_addr(client->socket.get(), socket_info);
			fmt_out("Client Disconnected", socket_addr, ENDL);
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