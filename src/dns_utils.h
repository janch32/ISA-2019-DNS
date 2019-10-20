#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <arpa/inet.h>

using namespace std;

namespace Dns{
	const uint BUFFER_SIZE = 512;

	using Bytes = vector<uint8_t>;
	
	enum Opcode : uint8_t{
		QUERY	= 0,
		IQUERY	= 1,
		STATUS	= 2
	};

	enum Rcode : uint8_t{
		SUCCESS			= 0,
		FORMAT_ERROR	= 1,
		SERVER_FAILTURE	= 2,
		NAME_ERROR		= 3,
		NOT_IMPLEMENTED	= 4,
		REFUSED			= 5
	};

	enum Type: uint16_t{
		TYPE_A		= 1,
		TYPE_NS		= 2,
		TYPE_CNAME	= 5,
		TYPE_SOA	= 6,
		TYPE_WKS	= 11,
		TYPE_PTR	= 12,
		TYPE_MX		= 15,
		TYPE_TXT	= 16,
		TYPE_AAAA	= 28
	};

	enum Class: uint16_t{
		CLASS_IN	= 1,
		CLASS_CH	= 3,
		CLASS_HS	= 4
	};
	
	void AppendNameToBytes(string name, Bytes *byte);
	
	string GetNameFromBytes(Bytes *byteptr, uint *index, Bytes *rbyteptr = NULL);
	
	string AddressToRevLookup(string address);

	string ClassToString(Class value);
	string TypeToString(Type value);
	string RcodeToString(Rcode value);
	string OpcodeToString(Opcode value);
}

