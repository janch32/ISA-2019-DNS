#include "main.hpp"

using namespace std;

int main(int argc, char *const *argv){
	try{
		mainProgram(argc, argv);
	}catch(std::exception &ex){
		cerr << ex.what() << endl;
		return 1;
	}

	return 0;
}

void mainProgram(int argc, char *const *argv){
	#ifdef DEBUG
	cout << "Warning: DEBUG flag set. To compile as release build run: make clean && make" << endl << endl;
	#endif
	
	Options opt = Options::Parse(argc, argv);

	// Vyrobit dotaz
	Dns::Message request;
	request.RecursionDesired = opt.RecursionDesired;
	
	Dns::Question qst;
	qst.Class = Dns::CLASS_IN;
	qst.Type = opt.RequestType;
	if(qst.Type == Dns::TYPE_PTR)
		qst.Name = Dns::AddressToRevLookup(opt.LookupAddress);
	else
		qst.Name = opt.LookupAddress;
	
	// Přidání dotazu do zprávy
	request.Question.push_back(qst);

	// Převést zprávu na bajty a odeslat 
	Dns::Bytes bytes = request.ToBytes();
	uint8_t buffer[Dns::BUFFER_SIZE];
	UdpClient::SendRequest(opt.DnsServerHost, opt.DnsServerPort, bytes.data(), bytes.size(), buffer, Dns::BUFFER_SIZE);	

	// Buffer na bajty
	Dns::Bytes bufferVec(buffer, buffer + sizeof(buffer) / sizeof(buffer[0]));
	Dns::Message response = Dns::Message::ParseBytes(&bufferVec);
	
	if(response.ID != request.ID)
		throw std::runtime_error("Failed to parse server response - ID mismatch (Request ID: " + to_string(request.ID) + ", Response ID: " + to_string(response.ID));
	
	cout << response.ToString();

	// Vypsat odeslané a přijaté bajty pokud je aplikace zkompilována s debug flagem
	#ifdef DEBUG
		cout << endl << "==DEBUG==";
		cout << endl << "REQUEST" << endl;
		for (unsigned int i = 0; i < bytes.size(); i++)
			cout << PADHEX(2, (int)bytes[i]) << " ";

		cout << endl << "RESPONSE" << endl;
		for (unsigned int i = 0; i < sizeof(buffer); i++)
			cout << PADHEX(2, (int)buffer[i]) << " ";

		Dns::Bytes responseConvBack = response.ToBytes();
		cout << endl << "RESPONSE_CONV_BACK" << endl;
		for (unsigned int i = 0; i < responseConvBack.size(); i++)
			cout << PADHEX(2, (int)responseConvBack[i]) << " ";
		cout << endl;
	#endif
}
