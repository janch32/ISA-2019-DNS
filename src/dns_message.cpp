#include "dns_message.h"

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

Bytes Message::ToBytes(){
	Bytes byte(12, 0);

	*(uint16_t *)&byte[0] = htons(this->ID);
	byte[2] = (this->IsResponse & 1) << 7;
	byte[2] += (this->Opcode & 0xF) << 3;
	byte[2] += (this->AuthoritativeAnswer & 1) << 2;
	byte[2] += (this->Truncated & 1) << 1;
	byte[2] += this->RecursionDesired & 1;
	byte[3] = (this->RecursionAvailable & 1) << 7;
	byte[3] += this->ResponseCode & 0xF;
	*(uint16_t *)&byte[4] = htons(this->Question.size());
	*(uint16_t *)&byte[6] = htons(this->Answer.size());
	*(uint16_t *)&byte[8] = htons(this->Authority.size());
	*(uint16_t *)&byte[10] = htons(this->Additional.size());

	for(Dns::Question q : this->Question)
		q.ToBytes(&byte);
	/*for(Dns::Resource r : this->Answer)
		r.ToBytes(&byte);
	for(Dns::Resource r : this->Authority)
		r.ToBytes(&byte);
	for(Dns::Resource r : this->Additional)
		r.ToBytes(&byte);*/

	return byte;
}
