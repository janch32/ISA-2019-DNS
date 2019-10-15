#include "main.h"

#define BUFSIZE 512


int main(int argc, char *const *argv){
	int client_socket, bytestx, bytesrx;
	socklen_t serverlen;
	struct hostent *server;
	struct sockaddr_in server_address;
	unsigned char buf[BUFSIZE];
	
	struct ProgramOptions opt;
	
	if(parseOptions(argc, argv, &opt) != 0)
		return 1;

	uint8_t *lookup;
	convNameToMsg(opt.lookupAddress, &lookup);

	struct DnsMessage req = {0};
	createRequestMessage(&req, lookup, opt.recursionDesired, opt.reverseLookup, TYPE_A);
	
	uint8_t *bytes;
	int bytes_length = dnsRequestToBytes(&req, &bytes);
	
	/* 2. ziskani adresy serveru pomoci DNS */
	if ((server = gethostbyname(opt.dnsServerHost)) == NULL) {
		fprintf(stderr,"ERROR: no such host as %s\n", opt.dnsServerHost);
		exit(EXIT_FAILURE);
	}
	
	/* 3. nalezeni IP adresy serveru a inicializace struktury server_address */
	bzero((char *) &server_address, sizeof(server_address));
	server_address.sin_family = AF_INET;
	bcopy((char *)server->h_addr_list[0], (char *)&server_address.sin_addr.s_addr, server->h_length);
	server_address.sin_port = htons(opt.dnsServerPort);
	
	/* Vytvoreni soketu */
	if ((client_socket = socket(AF_INET, SOCK_DGRAM, 0)) <= 0)
	{
		perror("ERROR: socket");
		exit(EXIT_FAILURE);
	}

	/* odeslani zpravy na server */
	serverlen = sizeof(server_address);
	bytestx = sendto(client_socket, bytes, bytes_length, 0, (struct sockaddr *) &server_address, serverlen);
	if (bytestx < 0) 
		perror("ERROR: sendto");
	
	/* prijeti odpovedi a jeji vypsani */
	bytesrx = recvfrom(client_socket, buf, BUFSIZE, 0, (struct sockaddr *) &server_address, &serverlen);
	if (bytesrx < 0) 
		perror("ERROR: recvfrom");
	
	printf("Request: \n");
	for (int i = 0; i < bytes_length; i++)
	{
		printf("%02x ", bytes[i]);
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
