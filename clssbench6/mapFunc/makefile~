## File: makefile
## Zhouxj@BUPTNIC
##
## IPv4 Seed Map to IPv6Seed
## See README file for details
 
CC = g++
CFLAGS = -g -pg
##CFLAGS = -O2
.cpp.o:
	${CC} ${CFLAGS} -c $*.cpp
	
MapFuc:	CBSeedGen.o PrefixList.o Sbintree.o Dbintree.o 
	${CC} ${CFLAGS} CBSeedGen.o PrefixList.o Sbintree.o Dbintree.o  -o MapFuc 
Sbintree.o : stdinc.h Sbintree.h
Dbintree.o : stdinc.h Dbintree.h
PrefixList.o : stdinc.h PrefixList.h
CBSeedGen.o : stdinc.h Sbintree.h Dbintree.h PrefixList.h

all:	MapFuc
