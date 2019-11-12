#pragma once
#include "dns_utils.h"
#include "dns_question.h"
#include "dns_resource.h"
#include <list>
#include <stdexcept>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <arpa/inet.h>

using namespace std;

namespace Dns{
	/**
	 * TODO
	 * 
	 * @see https://tools.ietf.org/html/rfc1035 (pages 25-28)
	 */
	class Message{
	private:
		/**
		 * 
		 * 
		 * @return ID DNS zprávy
		 */
		uint16_t GenerateId();
	public:
		/**  */
		uint16_t ID;
		bool IsResponse = false;
		bool AuthoritativeAnswer = false;
		bool Truncated = false;
		bool RecursionDesired = false;
		bool RecursionAvailable = false;

		/// Specifikace typu požadavku
		Dns::Opcode Opcode;
		Rcode ResponseCode;

		list<Dns::Question> Question;
		list<Resource> Answer;
		list<Resource> Authority;
		list<Resource> Additional;

		Message();
		string ToString();
		Bytes ToBytes();
		static Message ParseBytes(Bytes *byte);
	};
} 
