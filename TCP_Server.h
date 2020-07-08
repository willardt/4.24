#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include "Network.h"
#include "Socket.h"

#include <memory>
#include <vector>
#include <mutex>

class TCP_Server {
public:
	TCP_Server(const int af, const int protocol, const int port = DEFAULT_PORT);
	~TCP_Server();

	void net_send_all(const char* data);
	void net_send(const char* data, SOCKET s);
	void net_accept();
	void net_recieve(std::shared_ptr<Client_Socket> client);
	void net_respond();
	void shutdown();
private:
	bool _shutdown;
	int _port;

	std::mutex _mutex;

	std::vector<std::shared_ptr<Client_Socket>> _clients;
	Socket _server;

	WSAData _wsa_data;
};

#endif