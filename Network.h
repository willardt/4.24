#ifndef NETWORK_H
#define NETWORK_H

#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

#include <string_view>
#include <array>
#include <vector>
#include <memory>
#include <mutex>

#define MAX_CONNECTIONS 20
#define DEFAULT_PORT 80

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

struct Client_Socket {
	size_t id;
	Socket socket;
	std::thread thread;
};

class Server {
public:
	Server(const int af, const int type, const int protocol, const int port = DEFAULT_PORT);
	~Server();

	void net_send(const char* data, SOCKET s);
	void net_accept();
	void net_recieve(std::shared_ptr<Client_Socket> client);
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

class Client {
public:
	Client(const int af, const int type, const int protocol, const std::string_view server_addr, const int port = DEFAULT_PORT);
	~Client();

	std::string net_recieve();
	void net_send(const std::string_view data);
private:
	int _port;
	Socket _client;
	WSADATA _wsa_data;
};


#endif