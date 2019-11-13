#!/usr/bin/python3
# Testovací skript DNS resolveru

import subprocess
import os
import sys


class scolor:
	HEADER	= '\033[95m'
	GRAY	= '\033[30;1m'
	OKBLUE	= '\033[94m'
	PASS	= '\033[92m'
	WARNING	= '\033[93m'
	FAIL	= '\033[91m'
	RESET	= '\033[0m'
	BOLD	= '\033[1m'

PROGRAM = os.path.dirname(sys.argv[0]) + "/../dns"

shouldFailArgs = [
	["-s8.8.8.8", "ahoj", "svete"], # Moc argumentů
	["-s8.8.8.8"], # Nespecifikovaná dotazovaná adresa
	["-s8.8.8.8", "-qwertzuiop", "vutbr.cz"], # Neznámé flagy
	["-s8.8.8.8", "-6c", "vutbr.cz"], # Více typů dotazů
	["-s8.8.8.8", "-x", "vutbr.cz"], # vutbr.cz není validní IP adresa (reverzní záznam)
	["-s8.8.8.8", "-x", "vutbr.cz", "-p"], # Chybí parametr flagu
	["-s80d0::45fe:dqqq", "vutbr.cz"], # Nevalidní IPv6 adresa serveru
]

print(scolor.HEADER + "Argument testing (program should exit with return 1)\n" + scolor.RESET)
passed = 0
for args in shouldFailArgs:
	print("Running with", args, scolor.GRAY)
	proc = subprocess.run([PROGRAM] + args, stdout=subprocess.DEVNULL)#, stderr=subprocess.DEVNULL)
	print(scolor.RESET + "Exit code:", proc.returncode, scolor.BOLD, end=" \t")

	if proc.returncode == 1:
		print(scolor.PASS + "PASS\n" + scolor.RESET)
		passed += 1
	else:
		print(scolor.FAIL + "FAIL\n" + scolor.RESET)

print(scolor.PASS if passed == len(shouldFailArgs) else scolor.FAIL, end='')
print("Argument testing complete! Passed ", passed, "/", len(shouldFailArgs), scolor.RESET, sep='')
