#include "main.h"

#define BUFSIZE 512

using namespace std;

int main(int argc, char *const *argv){
	int client_socket, bytestx, bytesrx;
	socklen_t serverlen;
	struct hostent *server;
	struct sockaddr_in server_address;
	unsigned char buf[BUFSIZE];

	Options opt;

	try{
		opt = Options::Parse(argc, argv);
	}catch(std::exception &ex){
		cout << ex.what() << endl;
		return 1;
	}

	Dns::Question requestQ;
	requestQ.Name = opt.LookupAddress;
	requestQ.Class = Dns::CLASS_IN;
	requestQ.Type = Dns::TYPE_A;
	
	Dns::Message request;
	request.RecursionDesired = opt.RecursionDesired;
	request.Question.push_front(requestQ);
	
	Dns::Bytes bytes = request.ToBytes();

	printf("Request: \n");
	for (uint i = 0; i < bytes.size(); i++)
	{
		printf("%02x ", bytes[i]);
	}
	printf("\n");
	//exit(0);
	
	/* 2. ziskani adresy serveru pomoci DNS */
	if ((server = gethostbyname(opt.DnsServerHost.c_str())) == NULL) {
		fprintf(stderr,"ERROR: no such host as %s\n", opt.DnsServerHost.c_str());
		exit(EXIT_FAILURE);
	}
	
	/* 3. nalezeni IP adresy serveru a inicializace struktury server_address */
	bzero((char *) &server_address, sizeof(server_address));
	server_address.sin_family = AF_INET;
	bcopy((char *)server->h_addr_list[0], (char *)&server_address.sin_addr.s_addr, server->h_length);
	server_address.sin_port = htons(opt.DnsServerPort);
	
	/* Vytvoreni soketu */
	if ((client_socket = socket(AF_INET, SOCK_DGRAM, 0)) <= 0)
	{
		perror("ERROR: socket");
		exit(EXIT_FAILURE);
	}

	/* odeslani zpravy na server */
	serverlen = sizeof(server_address);
	bytestx = sendto(client_socket, bytes.data(), bytes.size(), 0, (struct sockaddr *) &server_address, serverlen);
	if (bytestx < 0) 
		perror("ERROR: sendto");
	
	/* prijeti odpovedi a jeji vypsani */
	bytesrx = recvfrom(client_socket, buf, BUFSIZE, 0, (struct sockaddr *) &server_address, &serverlen);
	if (bytesrx < 0) 
		perror("ERROR: recvfrom");
	
	
	
	printf("Response: \n");
	for (int i = 0; i < BUFSIZE; i++)
	{
		printf("%02x ", buf[i]);
	}
	printf("\n");

	return 0;
}
