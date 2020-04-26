#include "socket.h"

#include <iostream>

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
		std::cout << "Socket Connect Error Code -- " << WSAGetLastError() << '\n';
	}
	else {
		std::cout << "Connected to " << addr_name << '\n';
	}
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
		std::cout << "Socket Bind Error Code -- " << WSAGetLastError() << '\n';
	}
}

void Socket::net_listen(const int backlog) const {
	if (_socket == INVALID_SOCKET) {
		std::cout << "INVALID SOCKET" << std::endl;
		return;
	}

	if (const int result = listen(_socket, backlog)) {
		std::cout << "Socket Listen Error Code -- " << WSAGetLastError() << '\n';
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