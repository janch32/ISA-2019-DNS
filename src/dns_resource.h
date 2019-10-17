#pragma once
#include <cstdint>
#include <string>
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

		char DataLength;
		uint8_t *Data;

		void ToBytes(Bytes *byte);
		static Resource ParseBytes(const uint8_t *bytes, int length);
	};
}
