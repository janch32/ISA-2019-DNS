#include "main.h"

#define BUFSIZE 512


int main(int argc, char *const *argv){
	int client_socket, port_number, bytestx, bytesrx;
	socklen_t serverlen;
	const char *server_hostname;
	struct hostent *server;
	struct sockaddr_in server_address;
	unsigned char buf[BUFSIZE];
	
	struct ProgramOptions opt;
	
	if(parseOptions(argc, argv, &opt) != 0)
		return 1;

	printf("Test\n");
	unsigned char exampleDnsRequest[] = {
		// Header
		0x12, 0x34, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		// Query
		0x03, 'w', 'w', 'w', 0x04, 'i', 'e', 't', 'f', 0x03, 'o', 'r', 'g', 0x00, 0x00, 0x01, 0x00, 0x01
	};

	server_hostname = "8.8.8.8";
	port_number = 53;
	
	/* 2. ziskani adresy serveru pomoci DNS */
	
	if ((server = gethostbyname(server_hostname)) == NULL) {
		fprintf(stderr,"ERROR: no such host as %s\n", server_hostname);
		exit(EXIT_FAILURE);
	}
	
	/* 3. nalezeni IP adresy serveru a inicializace struktury server_address */
	bzero((char *) &server_address, sizeof(server_address));
	server_address.sin_family = AF_INET;
	bcopy((char *)server->h_addr_list[0], (char *)&server_address.sin_addr.s_addr, server->h_length);
	server_address.sin_port = htons(port_number);
   
	/* tiskne informace o vzdalenem soketu */ 
	printf("INFO: Server socket: %s : %d \n", inet_ntoa(server_address.sin_addr), ntohs(server_address.sin_port));
	
	/* Vytvoreni soketu */
	if ((client_socket = socket(AF_INET, SOCK_DGRAM, 0)) <= 0)
	{
		perror("ERROR: socket");
		exit(EXIT_FAILURE);
	}

	/* odeslani zpravy na server */
	serverlen = sizeof(server_address);
	bytestx = sendto(client_socket, exampleDnsRequest, 30, 0, (struct sockaddr *) &server_address, serverlen);
	if (bytestx < 0) 
		perror("ERROR: sendto");
	
	/* prijeti odpovedi a jeji vypsani */
	bytesrx = recvfrom(client_socket, buf, BUFSIZE, 0, (struct sockaddr *) &server_address, &serverlen);
	if (bytesrx < 0) 
		perror("ERROR: recvfrom");
	
	printf("Request: \n");
	for (int i = 0; i < 30; i++)
	{
		printf("%02x ", exampleDnsRequest[i]);
	}
	printf("\n");
	
	printf("Response: \n");
	for (int i = 0; i < BUFSIZE; i++)
	{
		printf("%02x ", buf[i]);
	}
	printf("\n");

	return 0;
}
