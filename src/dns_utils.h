#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

using namespace std;

namespace Dns{
	/** Velikost bufferu pro odpověď serveru */
	const unsigned int BUFFER_SIZE = 512;

	using Bytes = vector<uint8_t>;
	
	/** Specifikace typu dns dotazu */
	enum Opcode : uint8_t{
		QUERY	= 0,
		IQUERY	= 1,
		STATUS	= 2
	};

	/* Návratový kód odpovědi dns zprávy */
	enum Rcode : uint8_t{
		SUCCESS			= 0,
		FORMAT_ERROR	= 1,
		SERVER_FAILTURE	= 2,
		NAME_ERROR		= 3,
		NOT_IMPLEMENTED	= 4,
		REFUSED			= 5
	};

	/** Typy záznamů */
	enum Type: uint16_t{
		TYPE_A		= 1,
		TYPE_NS		= 2,
		TYPE_CNAME	= 5,
		TYPE_SOA	= 6,
		TYPE_WKS	= 11,
		TYPE_PTR	= 12,
		TYPE_MX		= 15,
		TYPE_TXT	= 16,
		TYPE_AAAA	= 28
	};

	/** Třída záznamu */
	enum Class: uint16_t{
		CLASS_IN	= 1,
		CLASS_CH	= 3,
		CLASS_HS	= 4
	};
	
	/**
	 * Převede doménové jméno na bajty včetně komprese
	 * 
	 * @param name Doménové jméno v neupraveném tvaru (např. "www.vutbr.cz")
	 * @param byte Vektor bytů, automaticky rozšíří a přidá jméno na konec
	 */
	void AppendNameToBytes(string name, Bytes *byte);
	
	/**
	 * Převede doménové jméno z bajtů podle zadaného indexu
	 * 
	 * @param byteptr Ukazatel na pole bajtů obsahující jméno
	 * @param index Index v poli, kde název začíná
	 * @param rbyteptr Pokud je specifikováno, použije se pro dekomrimaci
	 * @return Převedené jméno 
	 */
	string GetNameFromBytes(Bytes *byteptr, unsigned int *index, Bytes *rbyteptr = NULL);
	
	/**
	 * Převede IPv4 nebo IPv6 adresu na adresu reverzního dotazu
	 * 
	 * @param address Standardní zápis IPv4 nebo IPv6 adresy
	 * @return string Reverzní název
	 */
	string AddressToRevLookup(string address);

	/** Převedení výčtu Dns::Class na čitelný řetězec */
	string ClassToString(Class value);
	
	/** Převedení výčtu Dns::Type na čitelný řetězec */
	string TypeToString(Type value);
	
	/** Převedení výčtu Dns::Rcode na čitelný řetězec */
	string RcodeToString(Rcode value);
	
	/** Převedení výčtu Dns::Opcode na čitelný řetězec */
	string OpcodeToString(Opcode value);
}

