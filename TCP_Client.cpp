#include "TCP_Client.h"

#include <iostream>

TCP_Client::TCP_Client(const int af, const int type, const int protocol, const std::string_view server_addr, const int port) :
	_port(port)
{
	if (WSAStartup(MAKEWORD(2, 2), &_wsa_data)) {
		throw "Couldn't Init Server with WSAStartup";
	}

	_client.net_socket(af, type, protocol);
	_client.net_connect(server_addr.data(), port);
}

TCP_Client::~TCP_Client() {
	WSACleanup();
}

std::string TCP_Client::net_recieve() {
	char buf[MAX_MSG_LEN];
	const int result = recv(_client.get(), buf, MAX_MSG_LEN, 0);
	std::cout << result << '\n';
	return buf;
}

void TCP_Client::net_send(const std::string_view data) {
	send(_client.get(), data.data(), data.size() + 1, 0);
}