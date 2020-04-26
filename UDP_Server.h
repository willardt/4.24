#ifndef UDP_SERVER_H
#define UDP_SERVER_H

#include "Network.h"
#include "Socket.h"

#include <memory>
#include <vector>
#include <mutex>

class UDP_Server {
public:
	UDP_Server(const int af, const int type, const int protocol, const int port = DEFAULT_PORT);
	~UDP_Server();

	void net_send(const char* data, SOCKET s);
	void net_accept();
	void net_recieve();
	void net_respond();
	void shutdown();
private:
	std::mutex _mutex;
	bool _shutdown;
	bool _pause;

	int _port;
	std::vector<std::shared_ptr<Client_Socket>> _clients;
	Socket _server;
	WSAData _wsa_data;
};

#endif