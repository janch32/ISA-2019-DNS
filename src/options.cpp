#include "options.h"

Options Options::Parse(int argc, char *const *argv){
	Options opt;

	int c;
	// Kvůli skvělýmu POSIX který neumý parsovat non-options v getopt 
	// se to musí dělat takhle blbě přes dva cykly
	while(optind < argc){
		while ((c = getopt(argc, argv, ":hrx6tmcp:s:")) != -1){
			switch (c)
			{
				case 'h':
					Options::PrintHelpAndExit();
					break;
				case 'r':
					opt.RecursionDesired = true;
					break;
				case 'x':
					if(opt.RequestType != Dns::TYPE_A)
						throw std::invalid_argument("Multiple request types specified" + ERROR_HELP_MSG);
					opt.RequestType = Dns::TYPE_PTR;
					break;
				case '6':
					if(opt.RequestType != Dns::TYPE_A)
						throw std::invalid_argument("Multiple request types specified" + ERROR_HELP_MSG);
					opt.RequestType = Dns::TYPE_AAAA;
					break;
				case 'c':
					if(opt.RequestType != Dns::TYPE_A)
						throw std::invalid_argument("Multiple request types specified" + ERROR_HELP_MSG);
					opt.RequestType = Dns::TYPE_CNAME;
					break;
				case 'm':
					if(opt.RequestType != Dns::TYPE_A)
						throw std::invalid_argument("Multiple request types specified" + ERROR_HELP_MSG);
					opt.RequestType = Dns::TYPE_MX;
					break;
				case 't':
					if(opt.RequestType != Dns::TYPE_A)
						throw std::invalid_argument("Multiple request types specified" + ERROR_HELP_MSG);
					opt.RequestType = Dns::TYPE_TXT;
					break;
				case 'p':
					opt.DnsServerPort = optarg;
					break;
				case 's':
					opt.DnsServerHost = optarg;
					break;
				case ':': 
					// Chybí parametr argumentu
					throw std::invalid_argument("Missing argument for option -" + to_string(optopt) + ERROR_HELP_MSG);
				case '?': 
					// Neznámý přepínač
					throw std::invalid_argument("Unknown option -" + string((char*)&optopt) + ERROR_HELP_MSG);
			}

		}

		// Hodnota mimo přepínač (v našem případě hledaná adresa)
		if(optind < argc){
			if(!opt.LookupAddress.empty())
				throw std::invalid_argument("Unknown argument " + string(argv[optind]) + ERROR_HELP_MSG);
			
			opt.LookupAddress = argv[optind];
		}
		
		optind++;
	}

	if(opt.DnsServerHost.empty() || opt.LookupAddress.empty())
		throw std::invalid_argument("Missing required arguments" + ERROR_HELP_MSG);
	
	return opt;
}

void Options::PrintHelpAndExit(){
	cout << "USAGE: dns [-rx6tmc] -s server [-p port] address" << endl
	<< "LOOKUP TYPE OPTIONS:" << endl
	<< "    If no option is specified, program defaults to A record" << endl
	<< "    Only one flag of this type is allowed (you cannot use for ex. -6m)" << endl
	<< "    -6  Request AAAA record" << endl
	<< "    -t  Request TXT record" << endl
	<< "    -m  Request MX record" << endl
	<< "    -c  Request CNAME record" << endl
	<< "    -x  Request PTR record (reverse query)" << endl
	<< "        You must specify IP address instead of domain name" << endl
	<< "OPTIONS:" << endl
	<< "    -r      Use recursion (if it is supported by the server)" << endl
	<< "    -s str  DNS server address (required)" << endl
	<< "    -p int  DNS server port (optional, default=53)" << endl
	<< "    address Lookup domain name or IP address if -x is set" << endl
	<< "EXAMPLES:" << endl
	<< "    dns -r -s 8.8.8.8 google.com                (A record with recursion)" << endl
	<< "    dns -rx -s 8.8.8.8 216.58.201.110           (reverse name with recursion)" << endl
	<< "    dns -rx -s 8.8.8.8 2a00:1450:4014:801::200e (reverse name with recursion)" << endl
	<< "    dns -m -s 8.8.8.8 fit.vutbr.cz              (MX record without recursion)" << endl;

	exit(0);
}
