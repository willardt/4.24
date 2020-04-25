#include <iostream>

/*
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")
*/

#include "Network.h"
#include <thread>

#define DEFAULT_PORT 1024
#define DEFAULT_BUFLEN 512

int main() {
	/*
	WSADATA wsa_data;

	std::cout << "WSAStartup -- " << WSAStartup(MAKEWORD(2, 2), &wsa_data) << '\n';

	SOCKET listen_socket = INVALID_SOCKET;
	listen_socket = socket(AF_INET, SOCK_STREAM, 0);

	if(listen_socket == INVALID_SOCKET) {
		std::cout << "listen_socket -- INVALID" << '\n';
	}

	sockaddr_in addr;
	ZeroMemory(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(DEFAULT_PORT);

	std::cout << "bind -- "<< bind(listen_socket, (const sockaddr*)&addr, sizeof(addr)) << '\n';

	std::cout << "listen -- " << listen(listen_socket, SOMAXCONN) << '\n';

	std::cout << "Waiting..." << '\n';

	SOCKET client_socket = INVALID_SOCKET;
	client_socket = accept(listen_socket, NULL, NULL);
	if (client_socket == INVALID_SOCKET) {
		std::cout << "client_socket -- INVALID" << '\n';
	}

	std::cout << "connected" << '\n';

	char recvbuf[DEFAULT_BUFLEN];
	int r_result, s_result;
	int recvbuflen = DEFAULT_BUFLEN;

	do {
		if (r_result = recv(client_socket, recvbuf, recvbuflen, 0)) {
			std::cout << "recieved -- " << r_result << recvbuf << '\n';
			

			s_result = send(client_socket, recvbuf, r_result, 0);
			if(s_result == SOCKET_ERROR) {
				std::cout << "send failed" << std::endl;
			}

			std::cout << "sent -- " << s_result << '\n';
		} 
		std::cout << "r_result -- " << r_result << '\n';
	} while (r_result > 0);

	*/
	Server server(AF_INET, SOCK_STREAM, 0);

	std::thread server_connections(&Server::net_accept, &server);

	std::cout << "done" << '\n';
	system("PAUSE");

	server_connections.detach();
	return 0;
}