#ifndef UDP_SERVER_H
#define UDP_SERVER_H

#include "Network.h"
#include "Socket.h"

#include <memory>
#include <vector>
#include <mutex>

class UDP_Server {
public:
	UDP_Server(const int af, const int protocol, const int port = DEFAULT_PORT);
	~UDP_Server();

	// send to all sockets connected
	void net_sendall(const char* data);
	// send to specific socket
	void net_send(const char* data, const sockaddr_in& addr);
	void net_accept();
	void net_recieve();
	void net_respond();
	void shutdown();
private:
	bool _shutdown;
	int _port;

	std::mutex _mutex;

	std::vector<Client_Addr> _clients;
	sockaddr_in _broadcast_addr;
	Socket _server;

	WSAData _wsa_data;
};

#endif