#!/bin/bash
# Skript pro zabalení souborů do archivu určených pro odevzdání

make clean
rm -f xchalo16.tar
cp -f doc/manual.pdf manual.pdf
tar -cv -f xchalo16.tar obj src test Makefile manual.pdf README
rm -f manual.pdf
