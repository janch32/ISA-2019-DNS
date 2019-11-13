/**
 * Dotaz ve zprávě DNS knihovny
 * 
 * @file dns_question.h
 * @author Jan Chaloupka (xchalo16)
 */

#pragma once
#include <string>
#include <arpa/inet.h>
#include "dns_utils.h"

using namespace std;

namespace Dns{
	/**
	 * TODO
	 * 
	 * @see https://tools.ietf.org/html/rfc1035 (section 4.1.2, pages 28-29)
	 */
	class Question{
	public:
		/** Doménové jméno */
		string Name = "";
		
		/** Typ dotazu */
		Dns::Type Type = Dns::TYPE_A;

		/** Třída dotazu */
		Dns::Class Class = Dns::CLASS_IN;

		/**
		 * Převede dotaz na uživatelsky čitelný řetězec
		 * 
		 * @return Uživatelsky čitelná data 
		 */
		string ToString();

		/**
		 * Převede dotaz na bajty.
		 * Data dotazu vloží na konec pole a alokuje pro ně místo
		 * 
		 * @param byte ukazatel na inicializované pole bajtů, kam se přidají data
		 */
		void ToBytes(Bytes *byte);

		/**
		 * Převede dotaz z bajtů.
		 * 
		 * @param byteptr Vektor bajtů obsahující data dotazu
		 * @param index Ukazatel v poli na začátek dat dotazu
		 * @return DNS dotaz
		 */
		static Question ParseBytes(Bytes *byteptr, unsigned int *index);
	};
}
