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
	if(byte->size() > conv.size()){
		int it = distance(byte->begin(), search(byte->begin(), byte->end(), conv.begin(), conv.end()));
		conv.resize(sizeof(uint8_t) * 2);
		*(uint16_t *)&conv[0] = htons(it | 0xC000);
	}

	byte->insert(byte->end(), conv.begin(), conv.end());
	return;
}

string Dns::GetNameFromBytes(Bytes *byteptr, uint *index){
	Bytes byte = *byteptr;
	uint i = *index;

	string out = "";
	
	// Dekomprese
	if((byte[i] & 0xC0) == 0xC0)
		i = ntohs(*(uint16_t *)&byte[i]) & 0x3FFF;

	for(int len = -1; i < byte.size(); i++){
		if(len <= 0){
			if(len == 0) out += '.';
			len = byte[i];
			if(len == 0) break;
		}else{
			out += byte[i];
			len--;
		}
	}

	*index = (i <= *index) ? *index + 2 : i + 1;
	return out;
}

string Dns::ClassToString(Class value){
	switch(value){
		case CLASS_IN:	return "IN";
		case CLASS_CH:	return "CHAOS";
		case CLASS_HS:	return "HS";
		default:		return "N/A";
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
		default:			return "N/A";
	}
}

string Dns::RcodeToString(Rcode value){
	switch(value){
		case Rcode::SUCCESS:			return "Success";
		case Rcode::FORMAT_ERROR:		return "Format error";
		case Rcode::SERVER_FAILTURE:	return "Server failture";
		case Rcode::NAME_ERROR:			return "Name error";
		case Rcode::NOT_IMPLEMENTED:	return "Not Implemented";
		case Rcode::REFUSED:			return "Refused";
		default:						return "Unknown";
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
