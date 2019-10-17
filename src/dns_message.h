#pragma once

#include "dns_utils.h"
#include "dns_question.h"
#include "dns_resource.h"
#include <list>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <arpa/inet.h>

using namespace std;

namespace Dns{
	///
	///
	/// @see https://tools.ietf.org/html/rfc1035 (pages 25-28)
	class Message{
	private:
		uint16_t GenerateId();
	public:
		uint16_t ID;
		bool IsResponse;
		bool AuthoritativeAnswer;
		bool Truncated;
		bool RecursionDesired;
		bool RecursionAvailable;

		/// Specifikace typu požadavku
		Dns::Opcode Opcode;
		Rcode ResponseCode;

		list<Dns::Question> Question;
		list<Resource> Answer;
		list<Resource> Authority;
		list<Resource> Additional;

		Message();
		Bytes ToBytes();
		static Resource ParseBytes(const uint8_t *bytes, int length);
	};
} 

//void createRequestMessage(pDnsMessage message, uint8_t *host, bool recursion, bool reverse, uint16_t type);
