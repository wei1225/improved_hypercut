// File: PrefixList.cc
// David E. Taylor
// Applied Research Laboratory
// Department of Computer Science and Engineering
// Washington University in Saint Louis
// det3@arl.wustl.edu
//
// Class definition for PrefixList

#include "stdinc.h"
#include "PrefixList.h"



PrefixList::PrefixList() {
	N = 2*ADDRLEN + 1;
	cdist = 0;
	prefixes = new struct prefix*[25];
	for (int type = 0; type < 25; type++){
		prefixes[type] = new struct prefix[N];
		for (int i = 0; i < N; i++) {
			prefixes[type][i].prob = 0;
			//for (int j = 0; j < 33; j++) prefixes[type][i].sprob[j] = 0;
			for (int j = 0; j < ADDRLEN + 1; j++) 
			{
				prefixes[type][i].sprob[j] = 0;//modified j < 33 to j < 129
			}

		}
	}
}

PrefixList::~PrefixList() {
	for (int type = 0; type < 25; type++) 
	{
		delete prefixes[type];
	}
	delete prefixes;
}

void PrefixList::read(FILE* fp,FILE *fpOut,map<int,vector<mappingVal> > globle){
	for (int type = 0; type < 25; type++) read_type(type,fp,fpOut,globle);
	return;
}

void PrefixList::read_type(int type,FILE *fp,FILE *fpOut,map<int,vector<mappingVal> > globle) {
	int done = 0;
	int matches = 0;
	char comm[100];
	char wc_wc_comm[]="-wc_wc";
	char wc_lo_comm[]="-wc_lo";
	char wc_hi_comm[]="-wc_hi";
	char wc_ar_comm[]="-wc_ar";
	char wc_em_comm[]="-wc_em";
	char lo_wc_comm[]="-lo_wc";
	char lo_lo_comm[]="-lo_lo";
	char lo_hi_comm[]="-lo_hi";
	char lo_ar_comm[]="-lo_ar";
	char lo_em_comm[]="-lo_em";
	char hi_wc_comm[]="-hi_wc";
	char hi_lo_comm[]="-hi_lo";
	char hi_hi_comm[]="-hi_hi";
	char hi_ar_comm[]="-hi_ar";
	char hi_em_comm[]="-hi_em";
	char ar_wc_comm[]="-ar_wc";
	char ar_lo_comm[]="-ar_lo";
	char ar_hi_comm[]="-ar_hi";
	char ar_ar_comm[]="-ar_ar";
	char ar_em_comm[]="-ar_em";
	char em_wc_comm[]="-em_wc";
	char em_lo_comm[]="-em_lo";
	char em_hi_comm[]="-em_hi";
	char em_ar_comm[]="-em_ar";
	char em_em_comm[]="-em_em";

	// read in port width/range
	while (matches != EOF && done == 0) {
		matches = fscanf(fp,"%s\n",comm);
		// printf("comm = %s\n",comm);
		if (type == 0 && (strcmp(comm,wc_wc_comm) == 0)) 
		{
			done = 1;
			//fprintf(fpOut,"%s\n",wc_wc_comm);
		}
		else if (type == 1 && (strcmp(comm,wc_hi_comm) == 0)) 
		{
			done = 1;
			fprintf(fpOut,"%s\n",wc_hi_comm);
		}
		else if (type == 2 && (strcmp(comm,hi_wc_comm) == 0)) 
		{
			done = 1;
			fprintf(fpOut,"%s\n",hi_wc_comm);
		}
		else if (type == 3 && (strcmp(comm,hi_hi_comm) == 0)) 
		{
			done = 1;
			fprintf(fpOut,"%s\n",hi_hi_comm);
		}
		else if (type == 4 && (strcmp(comm,wc_lo_comm) == 0)) 
		{
			done = 1;
			fprintf(fpOut,"%s\n",wc_lo_comm);
		}
		else if (type == 5 && (strcmp(comm,lo_wc_comm) == 0))
		{
			done = 1;
			fprintf(fpOut,"%s\n",lo_wc_comm);
		}
		else if (type == 6 && (strcmp(comm,hi_lo_comm) == 0))
		{
			done = 1;
			fprintf(fpOut,"%s\n",hi_lo_comm);
		}
		else if (type == 7 && (strcmp(comm,lo_hi_comm) == 0))
		{
			done = 1;
			fprintf(fpOut,"%s\n",lo_hi_comm);
		}
		else if (type == 8 && (strcmp(comm,lo_lo_comm) == 0)) 
		{
			done = 1;
			fprintf(fpOut,"%s\n",lo_lo_comm);
		}
		else if (type == 9 && (strcmp(comm,wc_ar_comm) == 0)) 
		{
			done = 1;
			fprintf(fpOut,"%s\n",wc_ar_comm);
		}
		else if (type == 10 && (strcmp(comm,ar_wc_comm) == 0)) 
		{
			done = 1;
			fprintf(fpOut,"%s\n",ar_wc_comm);
		}
		else if (type == 11 && (strcmp(comm,hi_ar_comm) == 0)) {
			done = 1;
			fprintf(fpOut,"%s\n",hi_ar_comm);
		}
		else if (type == 12 && (strcmp(comm,ar_hi_comm) == 0)) 
		{
			done = 1;
			fprintf(fpOut,"%s\n",ar_hi_comm);
		}
		else if (type == 13 && (strcmp(comm,wc_em_comm) == 0)) {
			done = 1;
			fprintf(fpOut,"%s\n",wc_em_comm);
		}
		else if (type == 14 && (strcmp(comm,em_wc_comm) == 0)){
			done = 1;
			fprintf(fpOut,"%s\n",em_wc_comm);
		}
		else if (type == 15 && (strcmp(comm,hi_em_comm) == 0)) {
			done = 1;
			fprintf(fpOut,"%s\n",hi_em_comm);
		}
		else if (type == 16 && (strcmp(comm,em_hi_comm) == 0)) {
			done = 1;
			fprintf(fpOut,"%s\n",em_hi_comm);
		}
		else if (type == 17 && (strcmp(comm,lo_ar_comm) == 0)) {
			done = 1;
			fprintf(fpOut,"%s\n",lo_ar_comm);
		}
		else if (type == 18 && (strcmp(comm,ar_lo_comm) == 0)) {
			done = 1;
			fprintf(fpOut,"%s\n",ar_lo_comm);
		} 
		else if (type == 19 && (strcmp(comm,lo_em_comm) == 0)) {
			done = 1;
			fprintf(fpOut,"%s\n",lo_em_comm);
		} 
		else if (type == 20 && (strcmp(comm,em_lo_comm) == 0)) {
			done = 1;
			fprintf(fpOut,"%s\n",em_lo_comm);
		}
		else if (type == 21 && (strcmp(comm,ar_ar_comm) == 0)) {
			done = 1;
			fprintf(fpOut,"%s\n",ar_ar_comm);
		}
		else if (type == 22 && (strcmp(comm,ar_em_comm) == 0)) {
			done = 1;
			fprintf(fpOut,"%s\n",ar_em_comm);
		} 
		else if (type == 23 && (strcmp(comm,em_ar_comm) == 0)) {
			done = 1;
			fprintf(fpOut,"%s\n",em_ar_comm);
		}
		else if (type == 24 && (strcmp(comm,em_em_comm) == 0)) {
			done = 1;
			fprintf(fpOut,"%s\n",em_em_comm);
		}
	}
	if (matches == EOF) {
		fprintf(stderr,"Warning: Could not find proper identifier.\nNo prefix information taken from parameter file.\n");
		return;
	}
	// printf("Found identifier; done = %d\n",done);
	done = 0;
	int tlen = 0;
	int slen = 0;
	float prob = 0;
	//int lens[34];
	//float probs[34];
	int lens[ADDRLEN+2];
	int dstlens[ADDRLEN+2];
	float probs[ADDRLEN+2];
	float pro[ADDRLEN+2];

	char scomm[500];
	int scomm_len = 500;
	while (done == 0) {
		fgets(scomm,scomm_len,fp);
		memset(dstlens,0,sizeof(dstlens));
		memset(pro,0,sizeof(pro));
		// Read a line of the input
		//matches = sscanf(scomm,"%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f",&lens[0],&probs[0],&lens[1],&probs[1],&lens[2],&probs[2],&lens[3],&probs[3],&lens[4],&probs[4],&lens[5],&probs[5],&lens[6],&probs[6],&lens[7],&probs[7],&lens[8],&probs[8],&lens[9],&probs[9],&lens[10],&probs[10],&lens[11],&probs[11],&lens[12],&probs[12],&lens[13],&probs[13],&lens[14],&probs[14],&lens[15],&probs[15],&lens[16],&probs[16],&lens[17],&probs[17],&lens[18],&probs[18],&lens[19],&probs[19],&lens[20],&probs[20],&lens[21],&probs[21],&lens[22],&probs[22],&lens[23],&probs[23],&lens[24],&probs[24],&lens[25],&probs[25],&lens[26],&probs[26],&lens[27],&probs[27],&lens[28],&probs[28],&lens[29],&probs[29],&lens[30],&probs[30],&lens[31],&probs[31],&lens[32],&probs[32],&lens[33],&probs[33]);
		matches = sscanf(scomm,"%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f \
							   %d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f \
							   %d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f \
							   %d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f\t%d,%f \
							   ",&lens[0],&probs[0],&lens[1],&probs[1],&lens[2],&probs[2],&lens[3],&probs[3],&lens[4],&probs[4],&lens[5],&probs[5],&lens[6],&probs[6],&lens[7],&probs[7],&lens[8],&probs[8],&lens[9],&probs[9],&lens[10],&probs[10],&lens[11],&probs[11],&lens[12],&probs[12],&lens[13],&probs[13],&lens[14],&probs[14],&lens[15],&probs[15],&lens[16],&probs[16],&lens[17],&probs[17],&lens[18],&probs[18],&lens[19],&probs[19],&lens[20],&probs[20],&lens[21],&probs[21],&lens[22],&probs[22],&lens[23],&probs[23],&lens[24],&probs[24],&lens[25],&probs[25],&lens[26],&probs[26],&lens[27],&probs[27],&lens[28],&probs[28],&lens[29],&probs[29],&lens[30],&probs[30],&lens[31],&probs[31] \
							   ,&lens[32],&probs[32],&lens[33],&probs[33],&lens[34],&probs[34],&lens[35],&probs[35],&lens[36],&probs[36],&lens[37],&probs[37],&lens[38],&probs[38],&lens[39],&probs[39],&lens[40],&probs[40],&lens[41],&probs[41],&lens[42],&probs[42],&lens[43],&probs[43],&lens[44],&probs[44],&lens[45],&probs[45],&lens[46],&probs[46],&lens[47],&probs[47],&lens[48],&probs[48],&lens[49],&probs[49],&lens[50],&probs[50],&lens[51],&probs[51],&lens[52],&probs[52],&lens[53],&probs[53],&lens[54],&probs[54],&lens[55],&probs[55],&lens[56],&probs[56],&lens[57],&probs[57],&lens[58],&probs[58],&lens[59],&probs[59],&lens[60],&probs[60],&lens[61],&probs[61],&lens[62],&probs[62],&lens[63],&probs[63] \
							   ,&lens[64],&probs[64],&lens[65],&probs[65],&lens[66],&probs[66],&lens[67],&probs[67],&lens[68],&probs[68],&lens[69],&probs[69],&lens[70],&probs[70],&lens[71],&probs[71],&lens[72],&probs[72],&lens[73],&probs[73],&lens[74],&probs[74],&lens[75],&probs[75],&lens[76],&probs[76],&lens[77],&probs[77],&lens[78],&probs[78],&lens[79],&probs[79],&lens[80],&probs[80],&lens[81],&probs[81],&lens[82],&probs[82],&lens[83],&probs[83],&lens[84],&probs[84],&lens[85],&probs[85],&lens[86],&probs[86],&lens[87],&probs[87],&lens[88],&probs[88],&lens[89],&probs[89],&lens[90],&probs[90],&lens[91],&probs[91],&lens[92],&probs[92],&lens[93],&probs[93],&lens[94],&probs[94],&lens[95],&probs[95] \
							   ,&lens[96],&probs[96],&lens[97],&probs[97],&lens[98],&probs[98],&lens[99],&probs[99],&lens[100],&probs[100],&lens[101],&probs[101],&lens[102],&probs[102],&lens[103],&probs[103],&lens[104],&probs[104],&lens[105],&probs[105],&lens[106],&probs[106],&lens[107],&probs[107],&lens[108],&probs[108],&lens[109],&probs[109],&lens[110],&probs[110],&lens[111],&probs[111],&lens[112],&probs[112],&lens[113],&probs[113],&lens[114],&probs[114],&lens[115],&probs[115],&lens[116],&probs[116],&lens[117],&probs[117],&lens[118],&probs[118],&lens[119],&probs[119],&lens[120],&probs[120],&lens[121],&probs[121],&lens[122],&probs[122],&lens[123],&probs[123],&lens[124],&probs[124],&lens[125],&probs[125],&lens[126],&probs[126],&lens[127],&probs[127],&lens[128],&probs[128],&lens[129],&probs[129]);


		if (matches >= 4) {
			// Assign total probability
			prefixes[type][lens[0]].prob = probs[0];
			matches = (matches - 2)/2;
			// printf("matches = %d\n",matches);
			int maxMapTotalLength = 0;
			int MaxSrcLength = 0;
			for (int j = 1; j <= matches; j++) {
				// printf("Assigning prefixes[type][%d].sprob[%d] = %.4f\n",lens[0],lens[j],probs[j]);
				//prefixes[type][lens[0]].sprob[lens[j]] = probs[j];
				if (lens[j] < 23 || lens[j] >31)
				{
					int srcMapLen = MappingFunc(lens[j]);
					pro[srcMapLen] = probs[j];
					MaxSrcLength = srcMapLen > MaxSrcLength ? srcMapLen : MaxSrcLength;
				} 
				else
				{
					map<int,vector<mappingVal> >::iterator it = globle.find(lens[j]);
					if (it != globle.end())
					{
						vector<mappingVal> tmpVec(it->second);
						for (vector<mappingVal>::size_type ix=0; ix<tmpVec.size(); ++ix)
						{
							pro[tmpVec[ix].len]+=tmpVec[ix].prob * probs[j];
							MaxSrcLength = tmpVec[ix].len >  MaxSrcLength ? tmpVec[ix].len : MaxSrcLength;
						}
					}
				}

			}
			dstlens[matches] = lens[0] -lens[matches];
			int dstMapLen = 0;
			int srcMapLen = 0;
			if (dstlens[matches] < 23 || dstlens[matches] >31)
			{
				dstMapLen = MappingFunc(dstlens[matches]);
			}
			else
			{
				map<int,vector<mappingVal> >::iterator it = globle.find(dstlens[matches]);
				if (it != globle.end())
				{
					dstMapLen=(it->second)[0].len;
				}
			}
			if (lens[matches] < 23 || lens[matches] >31)
			{
				srcMapLen = MappingFunc(lens[matches]);
			}
			else
			{
				map<int,vector<mappingVal> >::iterator it = globle.find(lens[matches]);
				if (it != globle.end())
				{
					srcMapLen= (it->second)[0].len;
				}
			}
			maxMapTotalLength = dstMapLen + srcMapLen;
			maxMapTotalLength = maxMapTotalLength > MaxSrcLength ? maxMapTotalLength : MaxSrcLength;
			prefixes[type][maxMapTotalLength].prob =probs[0];
			fprintf(fpOut,"%d,%.8f",maxMapTotalLength,prefixes[type][maxMapTotalLength].prob);
			for (int j = 0 ; j < ADDRLEN +2 ; ++j)
			{
				if (pro[j] != 0)
				{
					prefixes[type][maxMapTotalLength].sprob[j] = pro[j];
					fprintf(fpOut,"\t%d,%.8f",j,prefixes[type][maxMapTotalLength].sprob[j]);
				}
			}
			fprintf(fpOut,"\n");
		}
		else done = 1;
	}
	fprintf(fpOut,"#\n");
	return;
}



void PrefixList::print(int type, FILE *fp) {
	// printf("N = %d\n",N);
	for (int i = 0; i < N; i++){
		if (prefixes[type][i].prob != 0) {
			fprintf(fp,"%d,%.8f",i,prefixes[type][i].prob);
			for (int j = 0; j <= ADDRLEN; j++) {
				if (prefixes[type][i].sprob[j] != 0) {
					fprintf(fp,"\t%d,%.8f",j,prefixes[type][i].sprob[j]);
				}
			}
			fprintf(fp,"\n");
		}
	}

	return;
}


