// File: PrefixList.h
// David E. Taylor
// Applied Research Laboratory
// Department of Computer Science and Engineering
// Washington University in Saint Louis
// det3@arl.wustl.edu
//
// Class definition for PrefixList
//   - Maintains distribution of address prefix lengths
//       * Each item is a (total length, probability) pair with an associated distribution
//         of source address prefix lengths (destination length defined by difference)
//   - Reads in distribution from seed file
//   - Applies smoothing adjustment to distributions
//   - Selects length based on two random inputs

#ifndef _PREFIXLIST_H_
#define _PREFIXLIST_H_


struct prefix {
	float prob;
	//float sprob[33];
	float sprob[ADDRLEN+1];
};

class PrefixList {
	int N; // PrefixList of N prefixes
	struct prefix** prefixes; // array of prefix structs
	int cdist; // flag signaling if cummulative distribution has been computed
	void read_type(int type,FILE *fp,FILE *fpOut,map<int,vector<mappingVal> > globle) ; // Read prefix information from input file *fp, type = t
public: PrefixList();
		~PrefixList();
		void read(FILE* fp,FILE *fpOut,map<int,vector<mappingVal> > globle); // Read prefix information from input file *fp
		void print(int type, FILE*); // Print prefix distribution
};

#endif
