#include "dns.h"

uint16_t generateId(){
	static bool seedSet = false;
	
	if(!seedSet){
		srand(time(NULL));
		seedSet = 1;
	}

	return (uint16_t)rand();
}

void cleanMessage(pDnsMessage message){
	if(message == NULL) return;

	free(message->question);
	free(message->answer);
	free(message->authority);
	free(message->additional);

	memset(message, 0, sizeof(struct DnsMessage));
}

void convNameToMsg(const char *name, uint8_t **conv){
	int len = strlen(name);
	uint8_t *dest = malloc(sizeof(uint8_t) * (len + 2));
	*conv = dest;

	int start = 0;
	for (int i = 0; i <= len; i++){
		dest[i+1] = name[i];
		if(name[i] == '.' || name[i] == 0){
			dest[start] = i - start;
			start = i+1;
		}
	}
}

void createRequestMessage(pDnsMessage message, uint8_t *host, bool recursion, bool reverse, uint16_t type){
	if(message == NULL) return;
	cleanMessage(message);

	message->id = generateId();
	message->rd = recursion ? 1 : 0;
	message->opcode = reverse ? 1 : 0;
	message->qdcount = 1;

	message->question = malloc(sizeof(struct DnsMessageQuestion));
	message->question->qtype = type;
	message->question->qname = host;
	message->question->qclass = CLASS_IN;
}



void dnsParseResponse(uint8_t const *byte, pDnsMessage *res){

}

int dnsRequestToBytes(pDnsMessage req, uint8_t** byteptr){
	if (req == NULL || byteptr == NULL) return -1;
	int size = 12;
	uint8_t *byte = malloc(sizeof(uint8_t) * size);

	*(uint16_t *)&byte[0] = htons(req->id);
	byte[2] = (req->qr << 7) + (req->opcode << 3) + (req->aa << 2) + (req->tc << 1) + req->rd;
	byte[3] = (req->z << 4) + req->rcode;
	*(uint16_t *)&byte[4] = htons(req->qdcount);
	*(uint16_t *)&byte[6] = 0;
	*(uint16_t *)&byte[8] = 0;
	*(uint16_t *)&byte[10] = 0;

	for (int i = 0; i < req->qdcount; i++){
		int namelen = strlen((char *)req->question[i].qname) + 1;
		byte = realloc(byte, sizeof(uint8_t) * (size + namelen + 4));
		memcpy(&byte[size], req->question[i].qname, sizeof(uint8_t) * namelen);
		*(uint16_t *)&byte[size + namelen] = htons(req->question[i].qtype);
		*(uint16_t *)&byte[size + namelen + 2] = htons(req->question[i].qclass);
		size += namelen + 4;
	}

	*byteptr = byte;
	return size;
}
