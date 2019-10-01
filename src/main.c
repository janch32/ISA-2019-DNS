#include "main.h"

int main(int argc, char *const *argv){
	struct ProgramOptions opt;
	
	if(parseArguments(argc, argv, &opt) != 0)
		return 1;

	return 0;
}
