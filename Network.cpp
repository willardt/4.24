#include "network.h"

#include <iostream>
#include <iomanip>

#define ENDL '\n'
#define MAX_MSG_LEN 50

void fmt_out() {}

template<typename _First, typename ..._Rest>
void fmt_out(_First&& first, _Rest&& ...rest) {
	std::cout << std::setw(20) << std::setfill('.') << std::forward<_First>(first);
	fmt_out(std::forward<_Rest>(rest)...);
}

sockaddr_in get_socket_info(SOCKET s) {
	sockaddr_in socket_info;
	ZeroMemory(&socket_info, sizeof(socket_info));
	int socket_info_len = sizeof(socket_info);
	getpeername(s, (sockaddr*)&socket_info, &socket_info_len);
	return socket_info;
}

std::string get_socket_addr(SOCKET s, sockaddr_in socket_addr) {
	int addr_size = sizeof(socket_addr);
	getpeername(s, (sockaddr*)&socket_addr, &addr_size);

	char addr[24];
	inet_ntop(socket_addr.sin_family, (const void*)&socket_addr.sin_addr, addr, sizeof(addr));

	return std::string(addr);
}

void print_host_address(const char* host_name) {
	addrinfo* result;
	getaddrinfo(host_name, nullptr, nullptr, &result);

	char addr[24];
	inet_ntop(result->ai_family, result->ai_addr, addr, sizeof(addr));
	fmt_out("Address", addr, ENDL);

	freeaddrinfo(result);
}

Socket::Socket() :
	_af(-1),
	_socket(INVALID_SOCKET)
{}

Socket::Socket(const int af, const int type, const int protocol) :
	_af(af),
	_socket(socket(af, type, protocol))
{
	net_set_nonblocking();
}

Socket::~Socket() {
	closesocket(_socket);
}

void Socket::net_socket(const int af, const int type, const int protocol) {
	_af = af;
	_socket = socket(af, type, protocol);
	net_set_nonblocking();
}

void Socket::net_connect(const std::string_view addr_name, const int port) const {
	if (_socket == INVALID_SOCKET) {
		std::cout << "INVALID SOCKET" << std::endl;
		return;
	}

	sockaddr_in server;
	ZeroMemory(&server, sizeof(server));
	inet_pton(_af, addr_name.data(), &server.sin_addr);
	server.sin_family = _af;
	server.sin_port = port;
	if (const int result = connect(_socket, (const sockaddr*)&server, sizeof(server))) {
		std::cout << "Socket Connect Error Code -- " << result << '\n';
	}
	std::cout << "Connected to " << addr_name << '\n';
}

void Socket::net_bind(const int port) const {
	if (_socket == INVALID_SOCKET) {
		std::cout << "INVALID SOCKET" << std::endl;
		return;
	}

	sockaddr_in server_addr;
	ZeroMemory(&server_addr, sizeof(server_addr));
	server_addr.sin_family = _af;
	server_addr.sin_port = port;
	if (const int result = bind(_socket, (const sockaddr*)&server_addr, sizeof(server_addr))) {
		std::cout << "Socket Bind Error Code -- " << result << '\n';
	}
}

void Socket::net_listen(const int backlog) const {
	if (_socket == INVALID_SOCKET) {
		std::cout << "INVALID SOCKET" << std::endl;
		return;
	}

	if (const int result = listen(_socket, backlog)) {
		std::cout << "Socket Listen Error Code -- " << result << '\n';
	}
}

void Socket::net_accept(Socket& socket, sockaddr* addr, int* addrlen) {
	_socket = accept(socket.get(), addr, addrlen);
}

void Socket::net_send(const std::string_view data) const {
	if (_socket == INVALID_SOCKET) {
		std::cout << "INVALID SOCKET" << std::endl;
		return;
	}

	if (send(_socket, data.data(), data.size() + 1, 0) != data.size() + 1) {
		std::cout << "Socket Send Didn't Match Data -- " << data << '\n';
	}
}

void Socket::net_set_nonblocking() {
	unsigned long block = 0;
	ioctlsocket(_socket, FIONBIO, &block);
}

SOCKET Socket::get() const {
	return _socket;
}

Server::Server(const int af, const int type, const int protocol, const int port) :
	_shutdown		( false ),
	_pause			( false ),
	_port			( port )
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

Server::~Server() {
	WSACleanup();
}

void Server::net_send(const char* data, SOCKET s) {
	if (s == INVALID_SOCKET) {
		std::cout << "INVALID SOCKET" << std::endl;
		return;
	}

	send(s, data, strlen(data) + 1, 0);
}

// Puts server in loop till another socket calls net_connect on it
// Needs to run in seperate thread
void Server::net_accept() {
	while (!_shutdown) {
		std::cout << "Watiing for client..." << '\n';

		std::shared_ptr<Client_Socket> client = std::make_shared<Client_Socket>();
		client->socket.net_accept(_server);

		_mutex.lock();
		_clients.push_back(client);
		_mutex.unlock();

		client->thread = std::thread(&Server::net_recieve, this, client);
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

void Server::net_recieve(std::shared_ptr<Client_Socket> client) {
	int result;
	do {
		char buf[MAX_MSG_LEN];
		result = recv(client->socket.get(), buf, MAX_MSG_LEN, 0);
		if(result > 0) {
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
				if(*it == client) {
					client->thread.detach();
					_clients.erase(it);
					break;
				}
			}
			
		}
	} while (result > 0);
}

void Server::shutdown() {
	_shutdown = true;
}

Client::Client(const int af, const int type, const int protocol, const std::string_view server_addr, const int port) :
	_port(port)
{
	if (WSAStartup(MAKEWORD(2, 2), &_wsa_data)) {
		throw "Couldn't Init Server with WSAStartup";
	}

	_client.net_socket(af, type, protocol);
	_client.net_connect(server_addr.data(), port);
}

Client::~Client() {
	WSACleanup();
}

std::string Client::net_recieve() {
	char buf[MAX_MSG_LEN];
	const int result = recv(_client.get(), buf, MAX_MSG_LEN, 0);
	std::cout << result << '\n';
	return buf;
}

void Client::net_send(const std::string_view data) {
	send(_client.get(), data.data(), data.size() + 1, 0);
}