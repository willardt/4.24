#include "UDP_Client.h"

#include <iostream>

UDP_Client::UDP_Client(const int af, const int type, const int protocol, const std::string_view server_addr, const int port) :
	_port(port)
{
	if (WSAStartup(MAKEWORD(2, 2), &_wsa_data)) {
		throw "Couldn't Init UDP_Client with WSAStartup";
	}

	_client.net_socket(af, type, protocol);
	_client.net_bind(port);

	char broadcast = '1';
	std::cout << setsockopt(_client.get(), SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) << std::endl;
	std::cout << WSAGetLastError() << std::endl;
}

UDP_Client::~UDP_Client() {
	WSACleanup();
}

std::string UDP_Client::net_recieve() {
	char buf[MAX_MSG_LEN];
	const int result = recvfrom(_client.get(), buf, MAX_MSG_LEN, 0, nullptr, nullptr);
	std::cout << result << '\n';
	return buf;
}

void UDP_Client::net_send(const std::string_view data, const sockaddr_in& addr) {
	sendto(_client.get(), data.data(), data.size() + 1, 0, (const sockaddr*)&addr, sizeof(addr));
}