#ifndef UDP_CLIENT_H
#define UDP_CLIENT_H
#include "Network.h"
#include "Socket.h"

#include <string_view>

class UDP_Client {
public:
	UDP_Client(const int af, const int type, const int protocol, const std::string_view server_addr, const int port = DEFAULT_PORT);
	~UDP_Client();

	std::string net_recieve();
	void net_send(const std::string_view data, const sockaddr_in& addr);
private:
	int _port;
	Socket _client;
	WSADATA _wsa_data;
};

#endif