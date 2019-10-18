#include "dns_resource.h"

using namespace Dns;

string Resource::ToString(){
	string out = this->Name + ", ";
	out += TypeToString(this->Type) + ", ";
	out += ClassToString(this->Class) + ", ";
	out += to_string(this->TTL) + ", ";
	out += "Data TODO\n";

	return out;
}

void Resource::ToBytes(Bytes *byte){
	AppendNameToBytes(this->Name, byte);
	
	int index = byte->size();
	byte->resize(sizeof(uint8_t) * (index + 10));
	*(uint16_t *)&(*byte)[index] = htons(this->Type);
	*(uint16_t *)&(*byte)[index + 2] = htons(this->Class);
	*(uint32_t *)&(*byte)[index + 4] = htonl(this->TTL);
	*(uint16_t *)&(*byte)[index + 8] = htons(this->Data.size());
	byte->insert(byte->end(), this->Data.begin(), this->Data.end());
}

Resource Resource::ParseBytes(Bytes *byteptr, uint *index){
	Bytes byte = *byteptr;
	Resource res = {};

	res.Name = GetNameFromBytes(byteptr, index);
	int i = *index;
	res.Type = (Dns::Type)ntohs(*(uint16_t *)&byte[i]);
	res.Class = (Dns::Class)ntohs(*(uint16_t *)&byte[i+2]);
	res.TTL = (Dns::Class)ntohl(*(uint32_t *)&byte[i+4]);
	short datalen = ntohs(*(uint16_t *)&byte[i+8]);
	res.Data.insert(res.Data.begin(), &byte[i+10], &byte[i+10+datalen]);

	*index = i + 10 + datalen;
	return res;
}
