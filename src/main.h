#pragma once
#include <iostream>
#include <string>
#include <cstdint>
#include "options.h"
#include "dns_message.h"
#include "udpclient.h"
#include <iomanip>

/**
 * Hlavní funkce programu
 * 
 * @param argc Počet zadaných argumentů
 * @param argv Pole argumentů (první je cesta ke spuštěnému programu)
 * @return int Stavový kód programu
 */
int main(int argc, char *const *argv);
