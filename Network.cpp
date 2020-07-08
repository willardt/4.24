#include "Network.h"

constexpr const char* get_family_name(int af) {
	switch (af) {
	case 1:
		return "AF_UNSPEC";
	case 2:
		return "AF_INET, IPv4";
	case 6:
		return "AF_IPX";
	case 16:
		return "AF_APPLETALK";
	case 17:
		return "AF_NETBIOS";
	case 23:
		return "AF_INET6, IPv6";
	case 26:
		return "AF_IRDA";
	case 32:
		return "AF_BTH";
	default:
		return "UNKOWN";
	};
}

constexpr const char* get_type_name(int type) {
	switch(type) {
	case 1:
		return "SOCK_STREAM, TCP";
	case 2:
		return "SOCK_DGRAM, UDP";
	case 3:
		return "SOCK_RAW";
	case 4:
		return "SOCK_RDM";
	case 5:
		return "SOCK_SEQPACKET";
	default:
		return "UNKOWN";
	}
}

constexpr const char* get_protocol_name(int protocol) {
	switch(protocol) {
	case 1:
		return "IPPROTO_ICMP";
	case 2:
		return "IPPROTO_IGMP";
	case 3:
		return "BTHPROTO_RFCOMM";
	case 6:
		return "IPPROTO_TCP";
	case 17:
		return "IPPROTO_UDP";
	case 58:
		return "IPPROTO_ICMPV6";
	case 113:
		return "IPPROTO_RM";
	default:
		return "UNKNOWN";
	}
}

sockaddr_in get_socket_info(SOCKET s) {
	sockaddr_in socket_info;
	ZeroMemory(&socket_info, sizeof(socket_info));
	int socket_info_len = sizeof(socket_info);
	getpeername(s, (sockaddr*)&socket_info, &socket_info_len);
	return socket_info;
}

std::string get_socket_addr(SOCKET s, sockaddr_in socket_addr) {
	int addr_size = sizeof(socket_addr);
	getpeername(s, (sockaddr*)&socket_addr, &addr_size);

	char addr[24];
	inet_ntop(socket_addr.sin_family, (const void*)&socket_addr.sin_addr, addr, sizeof(addr));

	return std::string(addr);
}

std::string get_socket_addr(const sockaddr_in& socket_addr) {
	int addr_size = sizeof(socket_addr);

	char addr[24];
	inet_ntop(socket_addr.sin_family, (const void*)&socket_addr.sin_addr, addr, sizeof(addr));

	return std::string(addr);
}

void print_host_address(const char* host_name) {
	addrinfo* result;
	getaddrinfo(host_name, nullptr, nullptr, &result);

	char addr[24];
	inet_ntop(result->ai_family, result->ai_addr, addr, sizeof(addr));
	fmt_out("Address", addr);

	freeaddrinfo(result);
}

void print_server(SOCKET s, int af, int type, int protocol, int port) {
	char name[24];
	gethostname(name, strlen(name));
	const auto socket_info = get_socket_info(s);
	const auto socket_addr = get_socket_addr(s, socket_info);

	std::cout << "*Starting Server*" << '\n';
	fmt_out("Host Name", name);
	print_host_address(name);
	fmt_out("Family", get_family_name(af));
	fmt_out("Type", get_type_name(type));
	fmt_out("Protocol", get_protocol_name(protocol));
	fmt_out("Port", port);
}