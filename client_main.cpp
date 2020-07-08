#include <iostream>

#include <winsock2.h>
#include <ws2tcpip.h>

#include <string>
#include <thread>

#pragma comment(lib, "Ws2_32.lib")

#include "Network.h"
#include "TCP_Client.h"

#define SERVER_NAME "192.168.1.4"

int main() {
	/*
	WSADATA wsa_data;

	std::cout << "WSAStartup -- " << WSAStartup(MAKEWORD(2, 2), &wsa_data) << '\n';

	SOCKET connect_socket;

	if ((connect_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		std::cout << "connect_socket -- INVALID" << '\n';
	}

	sockaddr_in server;
	inet_pton(AF_INET, SERVER_NAME, &server.sin_addr);
	server.sin_family = AF_INET;
	server.sin_port = htons(DEFAULT_PORT);

	std::cout << "connect -- " << connect(connect_socket, (const sockaddr*)&server, sizeof(server)) << '\n';

	std::string in = "";
	do {
		std::cout << "write: ";
		std::cin >> in;
		std::cout << "send -- " << send(connect_socket, in.c_str(), in.size() + 1, 0) << '\n';
	} while (in != "exit");

	*/

	
	//UDP_Client client(AF_INET, 0, SERVER_NAME);

	//sockaddr_in addr;
	//ZeroMemory(&addr, sizeof(addr));
	//addr.sin_family = AF_INET;
	//addr.sin_port = htons(80);
	//inet_pton(AF_INET, "192.168.1.4", &addr.sin_addr);

	TCP_Client client(AF_INET, 0, SERVER_NAME);

	std::string in;
	do {
		std::cout << "Enter:";
		std::cin >> in;
		client.net_send(in);

		std::cout << "Recieve -- " << client.net_recieve() << '\n';
		std::cout << WSAGetLastError() << "\n";
	} while (in != "exit");

	std::cout << "done" << '\n';
	system("PAUSE");

	return 0;
}