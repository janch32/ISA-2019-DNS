/**
 * Dotaz DNS knihovny
 * 
 * @file dns_question.h
 * @author Jan Chaloupka (xchalo16)
 */

#pragma once
#include <list>
#include <stdexcept>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <arpa/inet.h>
#include "dns_utils.hpp"
#include "dns_question.hpp"
#include "dns_resource.hpp"

using namespace std;

namespace Dns{
	/**
	 * TODO
	 * 
	 * @see https://tools.ietf.org/html/rfc1035 (pages 25-28)
	 */
	class Message{
	private:
		/**
		 * Náhodně vygenerované ID zprávy
		 * Zavolá se v konstruktoru při vytvoření objektu
		 * 
		 * @return ID DNS zprávy
		 */
		uint16_t GenerateId();
	public:
		/** Identifikační číslo zprávy, odpověď má mít stejné ID jako ID dotazu */
		uint16_t ID;

		/** Zpráva je dopoveď od serveru */
		bool IsResponse = false;
		
		/** Odpoveď je od autoritativního serveru pro tuto doménu */
		bool AuthoritativeAnswer = false;
		
		/** Zpráva se nevešla do maximální velikosti DNS zprávy (512B) */
		bool Truncated = false; 
		
		/** Je požadována rekurze ze strany klienta */
		bool RecursionDesired = false;
		
		/** Je rekurze k dispozici */
		bool RecursionAvailable = false;

		/** Typ zprávy */
		Dns::Opcode Opcode = Dns::QUERY;
		
		/** Stavový kód */
		Rcode ResponseCode = Dns::Rcode::SUCCESS;

		/** Seznam dotazů ve zprávě */
		list<Dns::Question> Question;
		
		/** Seznam odpovědí ve zprávě */
		list<Resource> Answer;
		
		/** Seznam autorit ve zprávě */
		list<Resource> Authority;

		/** Seznam dalších záznamů ve zprávě */
		list<Resource> Additional;

		Message();

		/**
		 * Převede zprávu na uživatelsky čitelný řetězec
		 * 
		 * @return Uživatelsky čitelná data 
		 */
		string ToString();
		
		/**
		 * Převede zprávu na bajty.
		 * Data zprávy vloží na konec pole a alokuje pro ně místo
		 * 
		 * @param byte ukazatel na inicializované pole bajtů, kam se přidají data
		 */
		Bytes ToBytes();

		/**
		 * Převede zprávu z bajtů.
		 * 
		 * @param byteptr Vektor bajtů obsahující data zprávy
		 * @param index Ukazatel v poli na začátek dat zprávy
		 * @return DNS zpráva
		 */
		static Message ParseBytes(Bytes *byte);
	};
} 
