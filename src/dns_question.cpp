#include "dns_question.h"

using namespace Dns;

void Question::ToBytes(Bytes *byte){
	AppendNameToByte(this->Name, byte);
	
	int index = byte->size();
	byte->resize(index + 4);
	*(uint16_t *)&(*byte)[index] = htons(this->Type);
	*(uint16_t *)&(*byte)[index + 2] = htons(this->Class);
}
