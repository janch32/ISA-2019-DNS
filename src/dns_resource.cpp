#include "dns_resource.hpp"

using namespace Dns;

Resource::Resource(Bytes *parsedFrom){
	this->ParsedFrom.insert(this->ParsedFrom.begin(), parsedFrom->begin(), parsedFrom->end());
}

string Resource::ToString(){
	string out = this->Name + ", ";
	out += TypeToString(this->Type) + ", ";
	out += ClassToString(this->Class) + ", ";
	out += to_string(this->TTL) + ", ";
	out += this->ParseData() + "\n";

	return out;
}

string Resource::ParseData(){
	unsigned int index = 0;
	string out;
	string email;

	switch(this->Type){
		case TYPE_NS: // Parsování dat NS záznamu (jméno)
		case TYPE_CNAME: // Parsování dat CNAME záznamu (jméno)
		case TYPE_PTR: // Parsování dat PTR záznamu (jméno)
			return GetNameFromBytes(&this->Data, &index, &this->ParsedFrom);
		case TYPE_A: // Parsování dat A záznamu (IPv4 adresy)
			if(sizeof(struct in_addr) > this->Data.size())
				throw std::out_of_range("Error parsing IPv4 address from DNS message - rdata is too short.");
			char addr[INET_ADDRSTRLEN];
			inet_ntop(AF_INET, this->Data.data(), addr, INET_ADDRSTRLEN);
			return string(addr);
		case TYPE_AAAA: // Parsování dat AAA záznamu (IPv6 adresy)
			if(sizeof(struct in6_addr) > this->Data.size())
				throw std::out_of_range("Error parsing IPv6 address from DNS message - rdata is too short.");
			char addr6[INET6_ADDRSTRLEN];
			inet_ntop(AF_INET6, this->Data.data(), addr6, INET6_ADDRSTRLEN);
			return string(addr6);
		case TYPE_TXT: // Parsování dat TXT záznamu (string)
			return string((char *)this->Data.data());
		case TYPE_MX: // Parsování dat MX záznamu (short + jméno)
			index = 2;
			out = to_string(ntohs(*(uint16_t *)&this->Data[0]));
			out += " " + GetNameFromBytes(&this->Data, &index, &this->ParsedFrom);
			return out;
		case TYPE_SOA: // Parsování dat SOA záznamu
			out = GetNameFromBytes(&this->Data, &index, &this->ParsedFrom);
			email = GetNameFromBytes(&this->Data, &index, &this->ParsedFrom);
			out += " " + email.replace(email.find('.'), 1, "@");

			if(index + 20 > this->Data.size())
				throw std::out_of_range("Error parsing SOA data - rdata is too short.");
			
			for(int i = 0; i < 5; i++)
				out += " " + to_string(ntohl(*(uint32_t *)&this->Data[index + 4*i]));
			
			return out;
		default:
			return "(unknown data)";
	}
}

void Resource::ToBytes(Bytes *byte){
	AppendNameToBytes(this->Name, byte);
	
	unsigned int index = byte->size();
	byte->resize(sizeof(uint8_t) * (index + 10));
	*(uint16_t *)&(*byte)[index] = htons(this->Type);
	*(uint16_t *)&(*byte)[index + 2] = htons(this->Class);
	*(uint32_t *)&(*byte)[index + 4] = htonl(this->TTL);
	*(uint16_t *)&(*byte)[index + 8] = htons(this->Data.size());
	byte->insert(byte->end(), this->Data.begin(), this->Data.end());
}

Resource Resource::ParseBytes(Bytes *byteptr, unsigned int *index){
	Bytes byte = *byteptr;
	Resource res(byteptr);

	res.Name = GetNameFromBytes(byteptr, index);
	
	unsigned int i = *index;
	checkListLength(byteptr->size(), i + 10);
	
	res.Type = (Dns::Type)ntohs(*(uint16_t *)&byte[i]);
	res.Class = (Dns::Class)ntohs(*(uint16_t *)&byte[i+2]);
	res.TTL = (int)ntohl(*(uint32_t *)&byte[i+4]);
	short datalen = ntohs(*(uint16_t *)&byte[i+8]);

	checkListLength(byteptr->size(), i + 10 + datalen);
	
	res.Data.insert(res.Data.begin(), &byte[i+10], &byte[i+10+datalen]);

	*index = i + 10 + datalen;
	return res;
}
