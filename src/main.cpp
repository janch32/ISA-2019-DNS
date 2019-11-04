#include "main.h"

using namespace std;

int main(int argc, char *const *argv){
	Options opt;

	try{
		opt = Options::Parse(argc, argv);
	}catch(std::exception &ex){
		cerr << ex.what() << endl;
		return 1;
	}
	
	Dns::Message request;
	request.RecursionDesired = opt.RecursionDesired;
	
	Dns::Question qst;
	qst.Class = Dns::CLASS_IN;
	qst.Type = opt.RequestType;
	if(qst.Type == Dns::TYPE_PTR)
		qst.Name = Dns::AddressToRevLookup(opt.LookupAddress);
	else
		qst.Name = opt.LookupAddress;
	
	request.Question.push_back(qst);
	
	Dns::Bytes bytes = request.ToBytes();
	uint8_t buffer[Dns::BUFFER_SIZE];
	UdpClient::SendRequest(opt.DnsServerHost, opt.DnsServerPort, bytes.data(), bytes.size(), buffer, Dns::BUFFER_SIZE);	

	Dns::Bytes bufferVec(buffer, buffer + sizeof(buffer) / sizeof(buffer[0]));
	Dns::Message response = Dns::Message::ParseBytes(&bufferVec);

	cout << response.ToString();

	return 0;
	
	cout << hex << uppercase << setw(2);
	cout << "Request:" << endl;
	for (unsigned int i = 0; i < bytes.size(); i++)
		cout << (int)bytes[i] << " ";
	cout << endl;
	
	cout << "Response:" << hex << endl;
	for (unsigned int i = 0; i < sizeof(buffer); i++)
		cout << (int)buffer[i] << " ";
	cout << dec << endl;

	/*Dns::Bytes rec(buffer, buffer + sizeof(buffer) / sizeof(buffer[0]));
	Dns::Message res = Dns::Message::ParseBytes(&rec);
	Dns::Bytes res2 = res.ToBytes();

	cout << "Response conv-back:" << endl;
	for (unsigned int i = 0; i < res2.size(); i++)
		cout << (int)res2[i] << " ";
	cout << endl;*/

	return 0;
}
