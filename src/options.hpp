/**
 * Parsování argumentů příkazové řádky
 * 
 * @file options.h
 * @author Jan Chaloupka (xchalo16)
 */

#pragma once
#include <iostream>
#include <string>
#include <stdexcept>
#include <unistd.h>
#include "dns_utils.hpp"

using namespace std;

#define ERROR_HELP_MSG string(". Run this program with -h to show help")

class Options{
	private:
		/**
		 * Vypíše nápovědu a informace o dostupných přepínačích na standardní výstup
		 * a ukončí program s návratovým kódem 0
		 */
		static void PrintHelpAndExit();
	public:
		/** Požadována rekurze (Recursion Desired = 1), jinak bez rekurze */
		bool RecursionDesired = false;
		
		/** Typ dotazu */
		Dns::Type RequestType = Dns::TYPE_A;

		/** Číslo portu, na který se má poslat dotaz */
		string DnsServerPort = "53";
		
		/** IP adresa nebo doménové jméno serveru, kam se má zaslat dotaz */
		string DnsServerHost = "";
		
		/** Dotazovaná adresa */
		string LookupAddress = "";

		/**
		 * Analyzuje argumenty spuštění programu a vloží je do struktury opt
		 * 
		 * @param argc Počet argumentů z příkazové řádky
		 * @param argv Pole argumentů zpříkazové řádky
		 * @return Třída options s analyzovanými hodnotami
		 * @throw std::invalid_argument Chyba při analyzování argumentů (např. neznámý argument)
		 */
		static Options Parse(int argc, char *const *argv);
};
