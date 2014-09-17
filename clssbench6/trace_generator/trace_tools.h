// File: trace_tools.h
// David E. Taylor 
// Applied Research Laboratory
// Department of Computer Science and Engineering
// Washington University in Saint Louis
// det3@arl.wustl.edu
//
// Functions for generating synthetic trace of headers
//
#include "flist.h"
#include "FilterList.h"

int header_gen(int d, FilterList* filters, FILE *fp, float a, float b, int scale);
void RandomCorner(int RandFilt, FilterList* filters, struct NewIPHeader* new_hdr);
int MyPareto(float a, float b);
//void convert_filters(int d, FilterList* filters, flist *filts);
int LinearSearch(FilterList* filters,struct NewIPHeader* hdr);

