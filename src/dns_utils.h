#pragma once
#include <string>
#include <vector>
#include <cstdint>

using namespace std;

namespace Dns{
	using Bytes = vector<uint8_t>;
	
	enum Opcode{
		QUERY	= 0,
		IQUERY	= 1,
		STATUS	= 2
	};

	enum Rcode{
		SUCCESS			= 0,
		FORMAT_ERROR	= 1,
		SERVER_FAILTURE	= 2,
		NAME_ERROR		= 3,
		NOT_IMPLEMENTED	= 4,
		REFUSED			= 5
	};

	enum Type{
		TYPE_A		= 1,
		TYPE_NS		= 2,
		TYPE_CNAME	= 5,
		TYPE_MX		= 15,
		TYPE_TXT	= 16
	};

	enum Class{
		CLASS_IN	= 1,
		CLASS_CS	= 2,
		CLASS_CH	= 3,
		CLASS_HS	= 4
	};
	
	void AppendNameToByte(string name, Bytes *byte);
}

