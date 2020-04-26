#ifndef NETWORK_H
#define NETWORK_H

#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

#include <iostream>
#include <iomanip>

#define MAX_CONNECTIONS 20
#define DEFAULT_PORT 80

#define ENDL '\n'
#define MAX_MSG_LEN 50

inline void fmt_out() {}

template<typename _First, typename ..._Rest>
inline void fmt_out(_First&& first, _Rest&& ...rest) {
	std::cout << std::setw(20) << std::setfill('.') << std::forward<_First>(first);
	fmt_out(std::forward<_Rest>(rest)...);
}

sockaddr_in get_socket_info(SOCKET s);

std::string get_socket_addr(SOCKET s, sockaddr_in socket_addr);

std::string get_socket_addr(const sockaddr_in& socket_addr);

void print_host_address(const char* host_name);

#endif