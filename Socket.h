#ifndef SOCKET_H
#define SOCKET_H

#include "Network.h"

#include <string_view>
#include <thread>

class Socket {
public:
	Socket();
	Socket(const int af, const int type, const int protocol);
	~Socket();

	void net_socket(const int af, const int type, const int protocol);
	void net_connect(const std::string_view addr_name, const int port) const;
	void net_bind(const int port) const;
	void net_listen(const int backlog = SOMAXCONN) const;
	void net_accept(Socket& socket, sockaddr* addr = nullptr, int* addrlen = nullptr);
	void net_send(const std::string_view data) const;
	void net_set_nonblocking();

	SOCKET get() const;
private:
	int _af;
	SOCKET _socket;
};

// TCP connections
struct Client_Socket {
	size_t id = 0;
	Socket socket;
	std::thread thread;
};

// UDP connections
struct Client_Addr {
	size_t id = 0;
	sockaddr_in addr;
};


#endif