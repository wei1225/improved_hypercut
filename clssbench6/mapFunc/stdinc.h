



// File: stdinc.h
// David E. Taylor
// Applied Research Laboratory
// Department of Computer Science and Engineering
// Washington University in Saint Louis
// det3@arl.wustl.edu
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;
//#include "inetaddress128.h"

typedef char bit;
typedef struct{
	int len;
	float prob;
}mappingVal;
// const int false = 0;
// const int true = 1;

const int Null = 0;
const int BIGINT = 0x7fffffff;
const int EOS = '\0';
const int MAXFL= 0xfffff;

inline int max(int x, int y) { return x > y ? x : y; }
inline double max(double x, double y) { return x > y ? x : y; }
inline int min(int x, int y) { return x < y ? x : y; }
inline double min(double x, double y) { return x < y ? x : y; }
inline int abs(int x) { return x < 0 ? -x : x; }
//inline bit isdigit(int c) { return (c >= '0') && (c <= '9'); }

inline void warning(char* p) { fprintf(stderr,"Warning:%s \n",p); }
inline void fatal(char* string) {fprintf(stderr,"Fatal:%s\n",string); exit(1); }

double pow(double,double);
double log(double);

// long random(); double exp(double),log(double);


// Filter database stuff
#define ADDRLEN 128 // IPv6 
#define ADDRBYTES ADDRLEN/8
#define MAXFILTERS 130000
#define MAXSTR 100
// #define NULL 0

unsigned int MappingFunc(unsigned int x);
inline unsigned int 
MapLenForSkew(unsigned int x)
{
	if ( x >32 )
		fatal("out of range");
	unsigned int result = 0;
	if (x >=17)  //17 <= X <=32
	{
		return 2*x;
	}
	if (x<=8)
	{
		result = (unsigned int)floor(2.875 *x + 0.5);
		return result;
	}
	//els 8 < x <=16
	result = (unsigned int)floor(1.125*x+14);
	return result;
	return 0;
}
