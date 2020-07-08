#include "UDP_Server.h"

#include <iostream>

UDP_Server::UDP_Server(const int af, const int protocol, const int port) :
	_shutdown		( false ),
	_port			( port )
{
	if (WSAStartup(MAKEWORD(2, 2), &_wsa_data)) {
		throw "Couldn't Init Server with WSAStartup";
	}
	_server.net_socket(af, SOCK_DGRAM, protocol);
	_server.net_bind(port);

	ZeroMemory(&_broadcast_addr, sizeof(_broadcast_addr));
	_broadcast_addr.sin_family = af;
	_broadcast_addr.sin_port = htons(port);
	//_broadcast_addr.sin_addr.S_un.S_addr = INADDR_BROADCAST;
	inet_pton(af, "192.168.1.255", &_broadcast_addr.sin_addr);

	//bind(_server.get(), (const sockaddr*)&_broadcast_addr, sizeof(_broadcast_addr));

	char broadcast = '1';
	std::cout << setsockopt(_server.get(), SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) << std::endl;
	std::cout << WSAGetLastError() << std::endl;

	print_server(_server.get(), af, SOCK_DGRAM, protocol, port);
}

UDP_Server::~UDP_Server() {
	WSACleanup();
}

void UDP_Server::net_sendall(const char* data) {
	sendto(_server.get(), data, strlen(data) + 1, 0, (const sockaddr*)&_broadcast_addr, sizeof(_broadcast_addr));
	
	char addr[100];
	inet_ntop(AF_INET, &_broadcast_addr.sin_addr, addr, 100);
	std::cout << "send all -- " << addr << std::endl;
}

void UDP_Server::net_send(const char* data, const sockaddr_in& addr) {
	sendto(_server.get(), data, strlen(data) + 1, 0, (const sockaddr*)&addr, sizeof(addr));
}

void UDP_Server::net_recieve() {
	char buf[MAX_MSG_LEN];
	sockaddr_in from_addr;
	ZeroMemory(&from_addr, sizeof(from_addr));
	int from_len = sizeof(from_addr);
	while(!_shutdown) {
		//net_sendall("Hello");
		
		const int result = recvfrom(_server.get(), buf, sizeof(buf), 0, (sockaddr*)&from_addr, &from_len);
		if(result > 0) {
			fmt_out("From", get_socket_addr(from_addr));
			fmt_out("Data", buf);
			// handle msg and respond

			if (strcmp(buf, "Connect") == 0) {
				static size_t id = 0;
				_clients.push_back({ id++, from_addr });
				fmt_out("New Connection", get_socket_addr(from_addr));
			}

			//net_send(buf, from_addr);
			//sendto(_server.get(), buf, result, 0, &from_addr, from_len);
			net_sendall(buf);
		}
		
	}
}

void UDP_Server::shutdown() {
	_shutdown = true;
}

