#include "dns_utils.h"

void Dns::AppendNameToByte(string name, Bytes *byte){
	int len = name.length();
	string conv(len + 1, '*');

	int start = len;
	for (int i = len; i > 0; i--){
		conv[i] = name[i-1];
		if(name[i-1] == '.'){
			conv[i] = start - i;
			start = i-1;
		}
	}
	conv[0] = start;

	// TODO error checking

	byte->insert(byte->end(), (uint8_t *)conv.c_str(), (uint8_t *)&conv.c_str()[conv.length()]);
	byte->push_back(0);
	return;
}
