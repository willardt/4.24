#include "Network.h"

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

std::string get_socket_addr(const sockaddr_in& socket_addr) {
	int addr_size = sizeof(socket_addr);

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