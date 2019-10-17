#pragma once
#include <string>
#include <stdexcept>
#include <unistd.h>

using namespace std;

#define ERROR_HELP_MSG string(". Run this program with -h to show available options")

class Options{
	private:
 		/// Převede číslo portu z řetězce na číslo včetně validace
 		///
 		/// @param port Řetězec obsahující číslo portu
 		/// @return Převedený port
 		/// @throw std::out_of_range Číslo je mimo interval 0-65535
		/// @throw std::invalid_argument Řetězec není reprezentace celého čísla v desítkové soustavě
		static int ParsePort(string port);
		
		/// Vypíše nápovědu a informace o dostupných přepínačích na standardní výstup
		/// a ukončí program s návratovým kódem 0
		static void PrintHelpAndExit();
	public:
		/// Požadována rekurze (Recursion Desired = 1), jinak bez rekurze
		bool RecursionDesired;
		
		/// Reverzní dotaz místo přímého
		bool ReverseLookup;
		
		/// Dotaz typu AAAA místo výchozího A
		bool IPv6;
		
		/// Číslo portu, na který se má poslat dotaz
		int DnsServerPort;
		
		/// IP adresa nebo doménové jméno serveru, kam se má zaslat dotaz
		string DnsServerHost;
		
		/// Dotazovaná adresa
		string LookupAddress;

		/// Analyzuje argumenty spuštění programu a vloží je do struktury opt
		///
		/// @param argc Počet argumentů z příkazové řádky
		/// @param argv Pole argumentů zpříkazové řádky
		/// @return Třída options s analyzovanými hodnotami 
		/// @throw std::out_of_range Číslo portu není validní
		/// @throw std::invalid_argument Chyba při analyzování argumentů (např. neznámý argument)
		static Options Parse(int argc, char *const *argv);
};
