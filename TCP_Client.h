#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include "Network.h"
#include "Socket.h"

#include <string_view>

class TCP_Client {
public:
	TCP_Client(const int af, const int type, const int protocol, const std::string_view server_addr, const int port = DEFAULT_PORT);
	~TCP_Client();

	std::string net_recieve();
	void net_send(const std::string_view data);
private:
	int _port;
	Socket _client;
	WSADATA _wsa_data;
};

#endif