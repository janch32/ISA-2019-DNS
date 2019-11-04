#include "dns_utils.h"

void Dns::AppendNameToBytes(string name, Bytes *byte){
	int len = name.length();
	Bytes conv(1, 0);

	int start = 0;
	for (int i = 0; i <= len; i++){
		if(name[i] == '.' || name[i] == 0){
			conv[start] = i - start;
			if(i - start == 0) break;
			start = i + 1;
		}
		conv.push_back(name[i]);
	}

	// Komprese
	if(conv.size() > 1 && byte->size() > conv.size()){
		int it = distance(byte->begin(), search(byte->begin(), byte->end(), conv.begin(), conv.end()));
		conv.resize(sizeof(uint8_t) * 2);
		*(uint16_t *)&conv[0] = htons(it | 0xC000);
	}

	byte->insert(byte->end(), conv.begin(), conv.end());
	return;
}

string Dns::GetNameFromBytes(Bytes *byteptr, unsigned int *index, Bytes *rbyteptr){
	Bytes byte = *byteptr;
	unsigned int i = *index;
	string out = "";
	
	for(int len = -1; i < byte.size(); i++){
		if(len <= 0){
			if(len == 0) out += '.';
			
			// Dekomprese
			if((byte[i] & 0xC0) == 0xC0){
				unsigned int ptrindex = ntohs(*(uint16_t *)&byte[i]) & 0x3FFF;
				out += GetNameFromBytes(rbyteptr != NULL ? rbyteptr : byteptr, &ptrindex);
				i++;
			}else{
				len = byte[i];
			}

			if(len <= 0) break;
		}else{
			out += byte[i];
			len--;
		}
	}

	*index = i + 1;
	return out;
}

string Dns::AddressToRevLookup(string address){
	stringstream out;

	struct in_addr inaddr = {};
	if(inet_pton(AF_INET, address.c_str(), &inaddr) == 1){
		for(int i = sizeof(inaddr.s_addr) - 1; i >= 0; i--)
			out << to_string((inaddr.s_addr >> i*8) & 0xFF) << ".";

		return out.str() + "in-addr.arpa.";
	}
	
	struct in6_addr in6addr = {};
	if(inet_pton(AF_INET6, address.c_str(), &in6addr) != 1){
		throw std::invalid_argument("Failed to convert address - \"" + address + "\" is not a valid IPv4 or IPv6 address.");
	}
	
	out << hex;
	for(int i = sizeof(in6addr.__in6_u.__u6_addr8) / sizeof(in6addr.__in6_u.__u6_addr8[0]) - 1; i >= 0; i--){
		out << (in6addr.__in6_u.__u6_addr8[i] & 0xF) << ".";
		out << ((in6addr.__in6_u.__u6_addr8[i] >> 4) & 0xF) << ".";
	}
	
	return out.str() + "ip6.arpa.";
}

string Dns::ClassToString(Class value){
	switch(value){
		case CLASS_IN:	return "IN";
		case CLASS_CH:	return "CHAOS";
		case CLASS_HS:	return "HS";
		default:		return "N/A (" + to_string((uint16_t)value) + ")";
	}
}

string Dns::TypeToString(Type value){
	switch (value){
		case TYPE_A:		return "A";
		case TYPE_NS:		return "NS";
		case TYPE_CNAME:	return "CNAME";
		case TYPE_SOA:		return "SOA";
		case TYPE_WKS:		return "WKS";
		case TYPE_PTR:		return "PTR";
		case TYPE_MX:		return "MX";
		case TYPE_TXT:		return "TXT";
		case TYPE_AAAA:		return "AAAA";
		default:			return "N/A (" + to_string((uint16_t)value) + ")";
	}
}

string Dns::RcodeToString(Rcode value){
	switch(value){
		case Rcode::SUCCESS:			return "Success";
		case Rcode::FORMAT_ERROR:		return "Format error - The server was unable to interpret the query";
		case Rcode::SERVER_FAILTURE:	return "Server failture - The name server was unable to process this query due to a problem with the name server";
		case Rcode::NAME_ERROR:			return "Name error - Domain name referenced in the query does not exist";
		case Rcode::NOT_IMPLEMENTED:	return "Not Implemented - The name server does not support th erequested kind of query";
		case Rcode::REFUSED:			return "Refused - The name server refused to preform the specified operation";
		default:						return "Unknown - Unknown error occured";
	}
}

string Dns::OpcodeToString(Opcode value){
	switch(value){
		case Opcode::QUERY:		return "QUERY";
		case Opcode::IQUERY:	return "IQUERY";
		case Opcode::STATUS:	return "STATUS";
		default:				return "N/A";
	}
}
