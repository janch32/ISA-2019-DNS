#pragma once
#include <string>
#include <arpa/inet.h>
#include "dns_utils.h"

using namespace std;

namespace Dns{
	///
	///
	/// @see https://tools.ietf.org/html/rfc1035 (section 4.1.3, pages 29-30)
	class Resource{
	public:
		string Name;
		Dns::Type Type;
		Dns::Class Class;
		int TTL;
		Bytes Data;

		string ToString();
		void ToBytes(Bytes *byte);
		static Resource ParseBytes(Bytes *byteptr, uint *index);
	};
}
