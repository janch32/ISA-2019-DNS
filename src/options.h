#pragma once
#include <stdlib.h> 
#include <stdbool.h>

#define UNKNOWN_ARGUMENT_MSG "Unknown argument \"%s\". Run this program with --help to show available arguments."

typedef struct ProgramOptions{
	bool recursionDesired;
	bool reverseLookup;
	bool ipv6;
	char* dnsServerHost;
	int dnsServerPort;
	char* lookupAddress;
} programOptions;


programOptions parseArguments(int argc, char const *argv[]);

void printHelp();