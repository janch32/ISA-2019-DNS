#include "udpclient.h"

void UdpClient::SendRequest(string destination, string port, uint8_t *data, int datalen, uint8_t *buffer, int bufferlen){
	// Získat adresu ze stringu
	// Chceme IPv4 i IPv6
	// Nastaven flag že chceme v6 jen pokud ji klient umí poslat
	struct addrinfo *dest, hints = {};
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = 0;
	hints.ai_flags = AI_ADDRCONFIG;
	
	int result = getaddrinfo(destination.c_str(), port.c_str(), &hints, &dest);

	if (result != 0){
		throw std::invalid_argument("Failed to parse server address - " + string(gai_strerror(result)));
	}
	
	if(dest == NULL)
		throw std::invalid_argument("Unknown server address - " + destination);
	
	int client;
	if ((client = socket(dest->ai_family, dest->ai_socktype, dest->ai_protocol)) <= 0)
		throw std::runtime_error("Error creating socket - (" + to_string(errno) + ") " + string(strerror(errno)));

	// Nastavit timeout přenosu na 5s 
	struct timeval timeout = {};
	timeout.tv_sec = 5;
	if (setsockopt (client, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout)) < 0)
        throw std::runtime_error("Cannot set socket send timeout - (" + to_string(errno) + ") " + string(strerror(errno)));

	if (setsockopt (client, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0)
        throw std::runtime_error("Cannost set socket receive timeout - (" + to_string(errno) + ") " + string(strerror(errno)));

	// Odeslat požadavek na server
	int bytestx = sendto(client, data, datalen, 0, dest->ai_addr, dest->ai_addrlen);
	if (bytestx < 0) 
		throw std::runtime_error("Error sending data - (" + to_string(errno) + ") " + string(strerror(errno)));
	
	// Přijmout odpoveď ze serveru
	int bytesrx = recvfrom(client, buffer, bufferlen, 0, dest->ai_addr, &dest->ai_addrlen);
	if (bytesrx < 0) 
		throw std::runtime_error("Error receiving data - (" + to_string(errno) + ") " + string(strerror(errno)));
	
	freeaddrinfo(dest);
}
