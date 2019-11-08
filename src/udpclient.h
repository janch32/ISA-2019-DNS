#pragma once
#include <cstdint>
#include <string>
#include <cstring>
#include <stdexcept>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <errno.h>

using namespace std;

#define UDP_TIMEOUT_SECONDS 5

class UdpClient{
public:
	/**
	 * Odešle pomocí protokolu UDP data na zadanou adresu a port
	 * a uloží odpověď do bufferu
	 * 
	 * Pro komunikaci je nastavený timeout UDP_TIMEOUT_SECONDS
	 * @see http://man7.org/linux/man-pages/man3/getaddrinfo.3.html
	 * 
	 * @param destination Adresa dotazovaného serveru
	 * @param port Port dotazovaného serveru
	 * @param data Data pro odeslání
	 * @param datalen Velikost pole dat pro odeslání
	 * @param buffer Buffer pro uložení odpovědi
	 * @param bufferlen Velikost bufferu
	 */
	static void SendRequest(string destination, string port, uint8_t *data, int datalen, uint8_t *buffer, int bufferlen);
};
