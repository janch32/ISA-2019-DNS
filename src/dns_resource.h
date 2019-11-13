#pragma once
#include <string>
#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include "dns_utils.h"

using namespace std;

namespace Dns{
	/**
	 * TODO
	 * 
	 * @see https://tools.ietf.org/html/rfc1035 (section 4.1.3)
	 */
	class Resource{
	private:
		/** Bajty, ze kterých byl záznam parsován */
		Bytes ParsedFrom;
	public:
		/** Doménové jméno */
		string Name = "";
		
		/** Typ záznamu */
		Dns::Type Type = Dns::TYPE_A;
		
		/** Třída, do které záznam patří */
		Dns::Class Class = Dns::CLASS_IN;
		
		/** Time to live - doba, po kterou má být záznam zachován v cache */
		int TTL = 0;
		
		/** Data záznamu */
		Bytes Data;

		/**
		 * @param parsedFrom Ukazatel na bajty, ze kterého byl záznam parsován
		 */
		Resource(Bytes *parsedFrom = NULL);

		/**
		 * Převede záznam na uživatelsky čitelný řetězec
		 * 
		 * @return Uživatelsky čitelná data 
		 */
		string ToString();

		/**
		 * Převede pole dat na čitelná data
		 * 
		 * @return Obsah dat
		 */
		string ParseData();
		
		/**
		 * Převede záznam na bajty.
		 * Data záznamu vloží na konec pole a alokuje pro ně místo
		 * 
		 * @param byte ukazatel na inicializované pole bajtů, kam se přidají data
		 */
		void ToBytes(Bytes *byte);
		
		/**
		 * Převede záznam z bajtů.
		 * 
		 * @see https://tools.ietf.org/html/rfc1035 (section 3.3)
		 * 
		 * @param byteptr Vektor bajtů obsahující data záznamu
		 * @param index Ukazatel v poli na začátek dat záznamu
		 * @return DNS záznam 
		 */
		static Resource ParseBytes(Bytes *byteptr, unsigned int *index);
	};
}
