#include "dns.h"

uint16_t generateId(){
	return (uint16_t)time();
}

void createRequest(pDnsMessage message, uint8_t *host, bool recursion, uint16_t type){
	message->id = generateId();
	message->rd = recursion ? 1 : 0;
	message->qdcount = 1;

	message->question = malloc(sizeof(struct DnsMessageQuestion));
	message->question->qtype = type;
	message->question->qname = host;
	message->question->qclass = CLASS_IN;
}

void 
