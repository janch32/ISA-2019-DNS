#include <cstdint>
#include <string>
#include <cstring>
#include <stdexcept>
#include <netdb.h>

using namespace std;

#define UDP_BUFFER_LENGTH 512

class UdpClient{
public:
	static void SendRequest(string destination, short port, uint8_t *data, int datalen, uint8_t *buffer, int bufferlen);
};
