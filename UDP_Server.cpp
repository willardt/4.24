#include "UDP_Server.h"

#include <iostream>

UDP_Server::UDP_Server(const int af, const int type, const int protocol, const int port) :
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

UDP_Server::~UDP_Server() {
	WSACleanup();
}

void UDP_Server::net_send(const char* data, SOCKET s) {
	if (s == INVALID_SOCKET) {
		std::cout << "INVALID SOCKET" << '\n';
		return;
	}

	send(s, data, strlen(data) + 1, 0);
}

void UDP_Server::net_recieve() {
	char buf[MAX_MSG_LEN];
	sockaddr_in from_addr;
	ZeroMemory(&from_addr, sizeof(from_addr));
	int from_len = sizeof(from_addr);
	while(!_shutdown) {
		const int result = recvfrom(_server.get(), buf, sizeof(buf), 0, (sockaddr*)&from_addr, &from_len);
		if(result > 0) {

			fmt_out("From", get_socket_addr(from_addr), ENDL);
			fmt_out("Data", buf, ENDL);
		}
	}
}

void UDP_Server::shutdown() {
	_shutdown = true;
}

