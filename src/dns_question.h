#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <arpa/inet.h>
#include "dns_utils.h"

using namespace std;

namespace Dns{
	///
	///
	/// @see https://tools.ietf.org/html/rfc1035 (section 4.1.2, pages 28-29)
	class Question{
	public:
		string Name;
		Dns::Type Type;
		Dns::Class Class;

		void ToBytes(Bytes *byte);
		static Question ParseBytes(const uint8_t *bytes, int length);
	};
}
