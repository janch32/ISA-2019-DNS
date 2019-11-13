#!/bin/bash
# Skript pro zabalení souborů do archivu určených pro odevzdání

make clean
rm -f xchalo16.tar
cp -f doc/manual.pdf manual.pdf
tar -cv -f xchalo16.tar Makefile README.md src obj manual.pdf
rm -f manual.pdf
