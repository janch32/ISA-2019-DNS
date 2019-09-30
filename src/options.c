#include "options.h"

programOptions parseArguments(int argc, char const *argv[]){
	struct ProgramOptions opt;

	opt.dnsServerPort = 53;
	opt.recursionDesired = false;
	opt.reverseLookup = false;
	opt.ipv6 = false;
	opt.dnsServerHost = NULL;
	opt.lookupAddress = NULL;

	return opt;
}

void printHelp(){
	printf("DNS Project TODO");
}