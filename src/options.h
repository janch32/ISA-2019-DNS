#pragma once
#include <stdlib.h> 
#include <stdio.h> 
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#define printerr(s, ...) fprintf(stderr, s, __VA_ARGS__)

#define ERROR_HELP_MSG ". Run this program with -h to show available options.\n"


/**
 * Struktura obsahující aktuální konfiguraci spuštěného programu
 */
typedef struct ProgramOptions{
	/** Požadována rekurze (Recursion Desired = 1), jinak bez rekurze */
	bool recursionDesired;
	/** Reverzní dotaz místo přímého */
	bool reverseLookup;
	/** Dotaz typu AAAA místo výchozího A */
	bool ipv6;
	/** Číslo portu, na který se má poslat dotaz */
	int dnsServerPort;
	/** IP adresa nebo doménové jméno serveru, kam se má zaslat dotaz */
	char* dnsServerHost;
	/** Dotazovaná adresa */
	char* lookupAddress;
} *programOptions;


/**
 * Analyzuje argumenty spuštění programu a vloží je do struktury opt
 * 
 * @param argc Počet argumentů z příkazové řádky
 * @param argv Pole argumentů zpříkazové řádky
 * @param opt Odkaz na strukturu pro ukládání možností spuštění
 * @return int Pokud analýza proběhne úspěšně, vrátí funkce hodnotu 0. 
 * 	V opačném případě vrátí nenulové číslo
 */
int parseOptions(int argc, char *const *argv, programOptions opt);


/**
 * Vypíše nápovědu a informace o dostupných přepínačích na standardní výstup
 * a ukončí program s návratovým kódem 0
 */
void printHelpAndExit();


/**
 * Převede číslo portu z řetězce na číslo včetně validace
 * 
 * @param port Řetězec obsahující číslo portu
 * @return int Převedený port
 * Pokud se převod nezdaří, z důvodu nepovolených znaků v řetězci nebo
 * čísla mimo intervalu je vrácená hodnota -1
 */
int parsePort(char *port);
