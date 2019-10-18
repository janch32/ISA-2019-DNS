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

	Dns::Question requestQ;
	requestQ.Name = opt.LookupAddress;
	requestQ.Class = Dns::CLASS_IN;
	requestQ.Type = Dns::TYPE_A;
	
	Dns::Message request;
	request.RecursionDesired = opt.RecursionDesired;
	request.Question.push_front(requestQ);
	
	Dns::Bytes bytes = request.ToBytes();

	/*cout << hex << uppercase << setw(2);
	cout << "Request:" << endl;
	for (uint i = 0; i < bytes.size(); i++)
		cout << (int)bytes[i] << " ";
	cout << endl;*/
	
	
	uint8_t buffer[UDP_BUFFER_LENGTH];
	UdpClient::SendRequest(opt.DnsServerHost, opt.DnsServerPort, bytes.data(), bytes.size(), buffer, UDP_BUFFER_LENGTH);
	
	cout << "Response:" << hex << endl;
	for (uint i = 0; i < sizeof(buffer); i++)
		cout << (int)buffer[i] << " ";
	cout << dec << endl;

	Dns::Bytes bufferVec(buffer, buffer + sizeof(buffer) / sizeof(buffer[0]));
	Dns::Message response = Dns::Message::ParseBytes(&bufferVec);

	cout << response.ToString();
	/*Dns::Bytes rec(buffer, buffer + sizeof(buffer) / sizeof(buffer[0]));
	Dns::Message res = Dns::Message::ParseBytes(&rec);
	Dns::Bytes res2 = res.ToBytes();

	cout << "Response conv-back:" << endl;
	for (uint i = 0; i < res2.size(); i++)
		cout << (int)res2[i] << " ";
	cout << endl;*/

	return 0;
}
