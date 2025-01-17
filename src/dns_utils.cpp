#include "dns_utils.hpp"

void Dns::AppendNameToBytes(string name, Bytes *byte){
	int len = name.length();
	// Vložíme první bajt, který se poté přepíše délkou labelu
	Bytes conv(1, 0);

	// Start je počátek labelu, když se dojde na tečku, tak aktuální pozicí a
	// hodnotou ve start zjistíme délku labelu a tu zapíšeme na pozici start
	int start = 0;
	for (int i = 0; i <= len; i++){
		if(name[i] == '.' || name[i] == 0){
			if(i - start > 63) throw std::invalid_argument(
				"Cannot convert domain name to bytes. Maximum length of label exceeeded (max: 63, got: "
				+ to_string(i - start) + ")");
			
			conv[start] = i - start;
			// Délka nula, konec jména
			if(i - start == 0) break;
			start = i + 1;
		}
		conv.push_back(name[i]);
	}

	if(conv.size() > 255) throw std::invalid_argument(
		"Cannot convert domain name to bytes. Maximum length of name exceeeded (max: 255, got: "
		+ to_string(conv.size()) + ")");

	// Komprese
	// Komprese nepodporuje částečnou kompresi, hledá pouze celé doménové jméno.
	// V našem projektu toto není problém, ale pokud by se mělo jedat o obecnou
	// DNS knihovnu, je potřeba toto chování doddělat
	if(conv.size() > 1 && byte->size() > conv.size()){
		// Pokud je jméno nalezeno, vrátí iterátor na začátek místa, jinak vrátí iterátor na konec listu
		Bytes::iterator searchIterator = search(byte->begin(), byte->end(), conv.begin(), conv.end());
		if(searchIterator != byte->end()){
			int it = distance(byte->begin(), searchIterator); // Převedení adresy na index
			conv.resize(sizeof(uint8_t) * 2);
			*(uint16_t *)&conv[0] = htons(it | 0xC000);
		}
	}

	byte->insert(byte->end(), conv.begin(), conv.end());
	return;
}

string Dns::GetNameFromBytes(Bytes *byteptr, unsigned int *index, Bytes *rbyteptr){
	Bytes byte = *byteptr;
	unsigned int i = *index;
	string out = "";
	
	// Cyklí se po bajtu a každým cyklem se snižuje délka labelu.
	// Když je délka labelu na nule, tak se přečetl celý label, do stringu se dá tečka a aktuální
	// bajt značí, jakou délku má další label (to se nastaví jako len) nebo může značit začátek
	// komprese - v tom případě se tato funkce volá rekurzivně s daným počátečním indexem.
	// 
	// Když bude délka labelu nula, tak je konec (přečetli jsme všechno) a inkrementuje se index
	// takže naný ukazuje na první bajt za jménem
	for(int len = -1; i < byte.size(); i++){
		if(len <= 0){
			// Len začíná s -1, aby se na začátek nepřidala tečka
			if(len == 0) out += '.';
			
			// Dekomprese
			if((byte[i] & 0xC0) == 0xC0){
				// Zavolat rekurzivně s počátečním indexem
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

	// Parsování IPv4 adresy
	struct in_addr inaddr = {};
	if(inet_pton(AF_INET, address.c_str(), &inaddr) == 1){
		// Uložení jednotlivých oktetů pozpátku do stringu
		for(int i = sizeof(inaddr.s_addr) - 1; i >= 0; i--)
			out << to_string((inaddr.s_addr >> i*8) & 0xFF) << ".";

		return out.str() + "in-addr.arpa.";
	}
	
	// Pokud se napodařilo parsovat IPv4, zkusit parsování jako IPv6
	struct in6_addr in6addr = {};
	if(inet_pton(AF_INET6, address.c_str(), &in6addr) != 1){
		throw std::invalid_argument("Failed to convert address - \"" + address + "\" is not a valid IPv4 or IPv6 address.");
	}
	
	out << hex;
	// Vypsat po 4 bitech hexadecimálně (pozpátku)
	for(int i = sizeof(in6addr.s6_addr) / sizeof(in6addr.s6_addr[0]) - 1; i >= 0; i--){
		out << (in6addr.s6_addr[i] & 0xF) << ".";
		out << ((in6addr.s6_addr[i] >> 4) & 0xF) << ".";
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
		case Rcode::NOT_IMPLEMENTED:	return "Not Implemented - The name server does not support the requested kind of query";
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
