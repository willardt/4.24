#ifndef NETWORK_H
#define NETWORK_H

#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

#include <iostream>
#include <iomanip>

#define MAX_CONNECTIONS 20
#define DEFAULT_PORT 80

#define MAX_MSG_LEN 1000

inline void fmt_out() {}

template<typename _First, typename _Last>
inline void fmt_out(_First&& first, _Last&& last) {
	std::cout << std::left << std::setw(20) << std::setfill('.') << std::forward<_First>(first)
		<< std::setw(40) << std::forward<_Last>(last) << std::endl;
}

constexpr const char* get_family_name(int af);
constexpr const char* get_type_name(int type);
constexpr const char* get_protocol_name(int protocol);

sockaddr_in get_socket_info(SOCKET s);

std::string get_socket_addr(SOCKET s, sockaddr_in socket_addr);

std::string get_socket_addr(const sockaddr_in& socket_addr);

void print_host_address(const char* host_name);

void print_server(SOCKET s, int af, int type, int protocol, int port);

#endif