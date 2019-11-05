#pragma once
#include <string>
#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include "dns_utils.h"

using namespace std;

namespace Dns{
	///
	///
	/// @see https://tools.ietf.org/html/rfc1035 (section 4.1.3, pages 29-30)
	class Resource{
	private:
		Bytes ParsedFrom;
	public:
		string Name;
		Dns::Type Type;
		Dns::Class Class;
		int TTL;
		Bytes Data;

		Resource(Bytes *parsedFrom = NULL);
		string ToString();
		string ParseData();
		void ToBytes(Bytes *byte);
		static Resource ParseBytes(Bytes *byteptr, unsigned int *index);
	};
}
