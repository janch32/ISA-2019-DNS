#include <cstdint>
#include <string>
#include <cstring>
#include <stdexcept>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <errno.h>
#include <cstring>

using namespace std;

#define UDP_BUFFER_LENGTH 512

class UdpClient{
public:
	static void SendRequest(string destination, short port, uint8_t *data, int datalen, uint8_t *buffer, int bufferlen);
};
