#include "dns_message.hpp"

using namespace Dns;

uint16_t Message::GenerateId(){
	static bool seedSet = false;
	if(!seedSet){
		srand(time(NULL));
		seedSet = 1;
	}

	return (uint16_t)rand();
}

Message::Message(){
	this->ID = GenerateId();
}

string Message::ToString(){
	string out = "Authoritative: ";
	out += this->AuthoritativeAnswer ? "Yes" : "No";
	out += ", Recursive: ";
	out += this->RecursionDesired && this->RecursionAvailable ? "Yes" : "No";
	out += ", Truncated: ";
	out += this->Truncated ? "Yes" : "No";

	if(this->ResponseCode != SUCCESS){
		out += "\nError: " + RcodeToString(this->ResponseCode);
		return out + "\n";
	}

	out += "\nQuestion section (" + to_string(this->Question.size()) + ")\n";
	for(Dns::Question q : this->Question)
		out += "  " + q.ToString();

	out += "Answer section (" + to_string(this->Answer.size()) + ")\n";
	for(Dns::Resource r : this->Answer)
		out += "  " + r.ToString();
	
	out += "Authority section (" + to_string(this->Authority.size()) + ")\n";
	for(Dns::Resource r : this->Authority)
		out += "  " + r.ToString();
	
	out += "Additional section (" + to_string(this->Additional.size()) + ")\n";
	for(Dns::Resource r : this->Additional)
		out += "  " + r.ToString();

	return out;
}

Bytes Message::ToBytes(){
	Bytes byte(12, 0);

	*(uint16_t *)&byte[0] = htons(this->ID);
	byte[2] = (this->IsResponse & 1) << 7;
	byte[2] |= (this->Opcode & 0xF) << 3;
	byte[2] |= (this->AuthoritativeAnswer & 1) << 2;
	byte[2] |= (this->Truncated & 1) << 1;
	byte[2] |= this->RecursionDesired & 1;
	byte[3] = (this->RecursionAvailable & 1) << 7;
	byte[3] |= this->ResponseCode & 0xF;
	*(uint16_t *)&byte[4] = htons(this->Question.size());
	*(uint16_t *)&byte[6] = htons(this->Answer.size());
	*(uint16_t *)&byte[8] = htons(this->Authority.size());
	*(uint16_t *)&byte[10] = htons(this->Additional.size());

	for(Dns::Question q : this->Question)
		q.ToBytes(&byte);
	for(Dns::Resource r : this->Answer)
		r.ToBytes(&byte);
	for(Dns::Resource r : this->Authority)
		r.ToBytes(&byte);
	for(Dns::Resource r : this->Additional)
		r.ToBytes(&byte);
	
	return byte;
}

Message Message::ParseBytes(Bytes *byteptr){
	Bytes byte = *byteptr;
	Message msg = {};

	checkListLength(byteptr->size(), 12);
	
	msg.ID = ntohs(*(uint16_t *)&byte[0]);
	msg.IsResponse = (byte[2] & 0x80) != 0;
	msg.Opcode = (Dns::Opcode)((byte[2] & 0x78) >> 3);
	msg.AuthoritativeAnswer = (byte[2] & 0x4) != 0;
	msg.Truncated = (byte[2] & 0x2) != 0;
	msg.RecursionDesired = (byte[2] & 0x1) != 0;
	msg.RecursionAvailable = (byte[3] & 0x80) != 0;
	msg.ResponseCode = (Dns::Rcode)(byte[3] & 0xF);

	unsigned int index = 12;
	int count = ntohs(*(uint16_t *)&byte[4]);
	for (int i = 0; i < count; i++)
		msg.Question.push_back(Dns::Question::ParseBytes(byteptr, &index));
	
	count = ntohs(*(uint16_t *)&byte[6]);
	for (int i = 0; i < count; i++)
		msg.Answer.push_back(Dns::Resource::ParseBytes(byteptr, &index));
	
	count = ntohs(*(uint16_t *)&byte[8]);
	for (int i = 0; i < count; i++)
		msg.Authority.push_back(Dns::Resource::ParseBytes(byteptr, &index));
	
	count = ntohs(*(uint16_t *)&byte[10]);
	for (int i = 0; i < count; i++)
		msg.Additional.push_back(Dns::Resource::ParseBytes(byteptr, &index));

	return msg;
}
