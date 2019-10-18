#include "udpclient.h"

void UdpClient::SendRequest(string destination, short port, uint8_t *data, int datalen, uint8_t *buffer, int bufferlen){
	
	struct hostent *server;
	
	if ((server = gethostbyname(destination.c_str())) == NULL) {
		throw std::invalid_argument("Unknown host " + destination);
	}
	 
	struct sockaddr_in address = {};
	address.sin_family = AF_INET;
	memcpy((char *)&address.sin_addr.s_addr, (char *)server->h_addr_list[0], server->h_length);
	address.sin_port = htons(port);
	
	int client;
	if ((client = socket(AF_INET, SOCK_DGRAM, 0)) <= 0)
	{
		throw std::runtime_error("Error creating socket " + errno);
	}

	// odeslani zpravy na server
	socklen_t serverlen = sizeof(address);
	int bytestx = sendto(client, data, datalen, 0, (struct sockaddr *) &address, serverlen);
	if (bytestx < 0) 
		throw std::runtime_error("Error sending UDP data " + errno);
	
	//prijeti odpovedi a jeji vypsani
	int bytesrx = recvfrom(client, buffer, bufferlen, 0, (struct sockaddr *) &address, &serverlen);
	if (bytesrx < 0) 
		throw std::runtime_error("Error receiving UDP data " + errno);
	
}
