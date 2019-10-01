#include "options.h"

int parseArguments(int argc, char *const *argv, programOptions opt){
	opt->dnsServerPort = 53;
	opt->recursionDesired = false;
	opt->reverseLookup = false;
	opt->ipv6 = false;
	opt->dnsServerHost = NULL;
	opt->lookupAddress = NULL;

	int c;
	while ((c = getopt(argc, argv, "-hrx6p:s:")) != -1){
		switch (c)
		{
			case 'h':
				printHelp();
				return 2;
				break;
			case 'r':
				opt->recursionDesired = true;
				break;
			case 'x':
				opt->reverseLookup = true;
				break;
			case '6':
				opt->ipv6 = true;
				break;
			case 'p':
				if((opt->dnsServerPort = parsePort(optarg)) < 0){
					printerr("Specified port is out of range - valid range is 0-65535 inclusive%s", ERROR_HELP_MSG);
					return 1;
				}
				break;
			case 's':
				opt->dnsServerHost = optarg;
				break;
			case 1: 
				// Hodnota mimo přepínač (v našem případě hledaná adresa)
				if(opt->lookupAddress != NULL){
					printerr("Unknown argument \"%s\"%s", optarg, ERROR_HELP_MSG);
					return 1;
				}
				opt->lookupAddress = optarg;
				break;
			case ':': 
				// Chybí parametr argumentu
				printerr("Missing argument for option -%c%s", optopt, ERROR_HELP_MSG);
				return 1;
				break;
			case '?': 
				// Neznámý přepínač
				printerr("Unknown option -%c%s", optopt, ERROR_HELP_MSG);
				return 1;
				break;
		}
	}

	if(opt->dnsServerHost == NULL || opt->lookupAddress == NULL){
		printerr("Missing required arguments%s", ERROR_HELP_MSG);
		return 1;
	}

	return 0;
}

void printHelp(){
	printf("DNS Project TODO\n");
}

int parsePort(char* port){
	char *parseEnd;
	long parsed = strtol(port, &parseEnd, 10);
	
	if(strlen(parseEnd) != 0 || parsed < 0 || parsed > 65535)
		return -1;

	return parsed;
}
