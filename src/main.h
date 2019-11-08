#pragma once
#include <iostream>
#include <string>
#include <cstdint>
#include "options.h"
#include "dns_message.h"
#include "udpclient.h"
#include <iomanip>

#define PADHEX(width, val) setfill('0') << setw(width) << std::hex << (unsigned)val

/**
 * Hlavní funkce programu
 * 
 * @param argc Počet zadaných argumentů
 * @param argv Pole argumentů (první je cesta ke spuštěnému programu)
 * @return int Stavový kód programu
 */
int main(int argc, char *const *argv);

void mainProgram(int argc, char *const *argv);
