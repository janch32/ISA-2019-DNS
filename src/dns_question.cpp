#include "dns_question.hpp"

using namespace Dns;

string Question::ToString(){
	string out = this->Name + ", ";
	out += TypeToString(this->Type) + ", ";
	out += ClassToString(this->Class) + "\n";

	return out;
}

void Question::ToBytes(Bytes *byte){
	AppendNameToBytes(this->Name, byte);
	
	unsigned int index = byte->size();
	byte->resize(sizeof(uint8_t) * (index + 4));
	*(uint16_t *)&(*byte)[index] = htons(this->Type);
	*(uint16_t *)&(*byte)[index + 2] = htons(this->Class);
}

Question Question::ParseBytes(Bytes *byteptr, unsigned int *index){
	Bytes byte = *byteptr;
	Question qst = {};

	qst.Name = GetNameFromBytes(byteptr, index);
	unsigned int i = *index;

	checkListLength(byteptr->size(), i + 4);

	qst.Type = (Dns::Type)ntohs(*(uint16_t *)&byte[i]);
	qst.Class = (Dns::Class)ntohs(*(uint16_t *)&byte[i+2]);
	
	*index = i + 4;
	return qst;
}
