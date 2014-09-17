#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include "stdinc.h"
#include "PrefixList.h"
#include "Dbintree.h"
#include "Sbintree.h"
using namespace std;

void ReadFileExceptLen(ifstream &inFile,ofstream &outFile);
void ReadconfigFile(ifstream &inFile);
int read_scale(FILE *fp);

map<int,vector<mappingVal> > globle;

unsigned int MappingFunc(unsigned int x)
{
	if ( (x>=23 && x <32) || x >32 )
		fatal("out of range");
	unsigned int result = 0;
	if (x<=8)
	{
		result = (unsigned int)floor(2.875 *x + 0.5);
		return result;
	}
	if (x>8 && x <23)
	{
		result = (unsigned int)floor(0.643*x+18.4);
		return result;
	}
	if (x ==32)
	{
		return 128;
	}
	fatal("error");
	return 0;
}

int main(int argc,char **argv)
{

	if (argc > 4 || argc < 2){
		fprintf(stderr,"USAGE:CBSeedGen <IPv4SeedFilename> <IPv6Config> <IPv6SeedFilename>\n");
		fprintf(stderr,"Example: CBseedGen IPv4SeedFile IPv6Config IPv6SeedFile\n");
		exit(1);
	}
	ifstream oringinFile;
	oringinFile.open(argv[1]);
	string outfile;
	if (argc <4 )
	{
		outfile.assign(argv[1]);
		outfile +="V6";
	}
	else
		outfile.assign(argv[3]);
	ofstream outResultFile;
	outResultFile.open(outfile.c_str());
	ReadFileExceptLen(oringinFile,outResultFile);
	oringinFile.close();
	outResultFile.close();


	string configName("IPv6ConfigSeedFile.txt");
	if (argv[2]  )
	{
		configName.assign(argv[2]);
	} 
	ifstream configV6(configName.c_str());
	ReadconfigFile(configV6);
	configV6.close();

	FILE *fp_in;
	char IPv4SeedFilename[256];
	char IPv6SeedFilename[256];
	strcpy(IPv4SeedFilename,argv[1]);
	strcpy(IPv6SeedFilename,outfile.c_str());

	fp_in = fopen(IPv4SeedFilename,"r");
	FILE *fpOut=fopen(IPv6SeedFilename,"at");

	int scale = read_scale(fp_in);
	// Read prefix length distributions, initialize
	PrefixList *prefixL = new PrefixList();
	prefixL->read(fp_in,fpOut,globle);

	char IPv6ConfigSeed[256];
	strcpy(IPv6ConfigSeed,configName.c_str());
	FILE *fpConfigIn=fopen(IPv6ConfigSeed,"r");
	sbintree *Stree = new sbintree();
	// Read source address nesting
	(*Stree).read_nest(fp_in);
	Stree->print_nest(fpOut);
	// Read source address skew
	(*Stree).read_skew(fp_in);
	Stree->MappingSkew(scale);
	Stree->read_config_skew(fpConfigIn);
	(*Stree).print_skew(fpOut);
	dbintree *Dtree = new dbintree();
	// Read destination address nesting
	(*Dtree).read_nest(fp_in);
	Dtree->print_nest(fpOut);
	(*Dtree).read_skew(fp_in);
	Dtree->MappingSkew(scale);
	Dtree->read_config_skew(fpConfigIn);
	(*Dtree).print_skew(fpOut);
	(*Dtree).read_corr(fp_in);
	Dtree->mappingCorr();
	Dtree->read_corr(fpConfigIn); //read self_defined corr from configFile
	(*Dtree).print_corr(fpOut);

	printf(" \tdone\n");
	return 0;
}

void ReadFileExceptLen(ifstream &inFile,ofstream &outFile)
{
	string oneLine;
	string done("-wc_wc");
	do 
	{
		getline(inFile,oneLine);
		outFile<<oneLine<<endl;
	} while (oneLine !=done);
	/*
	getline(inFile,oneLine);

	while (oneLine !="#")
	{
	string word;
	float pro[129];
	memset(pro,0,sizeof(pro));
	istringstream sstream(oneLine);
	sstream>>word;
	int oringinalLength=atoi(word.c_str());
	int maxMapTotalLength=0;   //影射完的总长度
	sstream>>word;
	while (word !="")
	{
	int srcLength=atoi(word.c_str());
	int srcMap=0;
	int dstLength=oringinalLength-srcLength;
	int dstMap=0;
	sstream>>word;
	float srcPro=(float)atof(word.c_str());
	if (dstLength < 23 || dstLength > 31)
	{
	dstMap=MappingFunc(dstLength);
	} 
	else
	{
	map<int,vector<mappingVal> >::iterator it = globle.find(dstLength);
	if (it != globle.end())
	{
	dstMap= it->second.back().len;
	}
	}
	if (srcLength < 23 || srcLength >31)
	{
	srcMap = MappingFunc(srcLength);
	pro[srcMap] = srcPro;
	} 
	else
	{
	map<int,vector<mappingVal> >::iterator it = globle.find(dstLength);
	if (it != globle.end())
	{
	vector<mappingVal> tmpVec(it->second);
	for (vector<mappingVal>::size_type ix=0; ix<tmpVec.size(); ++ix)
	{
	pro[tmpVec[ix].len]+=tmpVec[ix].prob * srcPro;
	}
	int tmpMax=dstMap + tmpVec.back().len;
	maxMapTotalLength= (tmpMax>maxMapTotalLength)? tmpMax:maxMapTotalLength;
	}
	}
	word.clear();
	sstream>>word;
	}
	outFile<<maxMapTotalLength<<"\t";
	for (int ii=0; ii<129;++ii )
	{
	if (pro[ii] != 0.0)
	{
	outFile<<ii<<"\t"<<pro[ii]<<"\t";
	}
	}
	outFile<<endl;
	getline(inFile,oneLine);
	}
	outFile<<"#"<<endl;
	*/
	return;
}
void ReadSnest(ifstream &inFile,ofstream &outFile)
{
	string oneLine;
	getline(inFile,oneLine);
	int done =0;
	while (!inFile.eof() || done ==0)
	{
		if (oneLine == "-snest")
			done = 1;
	}
	if (inFile.eof())
	{
		cerr<<"No source address nest specified for custom distribution."<<endl;
		exit(1);
	}
	outFile<<oneLine<<endl;
	getline(inFile,oneLine);
	outFile<<oneLine<<endl;
	outFile<<"#"<<endl;
}
void HandleSSkew(ifstream &inFile,ofstream &outFile)
{
	string oneLine;
	getline(inFile,oneLine);
	int done =0;
	while (!inFile.eof() || done ==0)
	{
		if (oneLine == "-sskew")
			done = 1;
	}
	if (inFile.eof())
	{
		cerr<<"No source address skew specified for custom distribution."<<endl;
		exit(1);
	}
	outFile<<oneLine<<endl;
	getline(inFile,oneLine);
	while (oneLine !="#")
	{
		istringstream sstream(oneLine);
	}
	return;
}

void ReadDnest(ifstream &inFile,ofstream &outFile)
{
	string oneLine;
	getline(inFile,oneLine);
	int done =0;
	while (!inFile.eof() || done ==0)
	{
		if (oneLine == "-dnest")
			done = 1;
	}
	if (inFile.eof())
	{
		cerr<<"No source address nest specified for custom distribution."<<endl;
		exit(1);
	}
	outFile<<oneLine<<endl;
	getline(inFile,oneLine);
	outFile<<oneLine<<endl;
	outFile<<"#"<<endl;
}

void ReadconfigFile(ifstream &inFile)
{
	string oneLine;
	getline(inFile,oneLine);
	while(oneLine != "-globle")
	{
		getline(inFile,oneLine);
	}
	string word;
	mappingVal tmpMap={0,0.0};
	vector<mappingVal> tmpVec;
	getline(inFile,oneLine);
	while(oneLine !="#")
	{
		istringstream stream(oneLine);
		stream>>word;
		int oringinal=atoi(word.c_str());
		stream>>word;
		while(word !="")
		{
			tmpMap.len = atoi(word.c_str());
			stream>>word;
			tmpMap.prob =(float)atof(word.c_str());
			tmpVec.push_back(tmpMap);
			word.clear();
			stream>>word;
		}
		globle.insert(make_pair(oringinal,tmpVec));
		tmpVec.clear();
		getline(inFile,oneLine);
	}
	return ;
}

void HandlePrefixLen(ifstream &inFile,ofstream &outFile)
{
	return;
}

int read_scale(FILE *fp){
	int done = 0;
	int matches = 0;
	char comm[30];
	char scale_comm[]="-scale";
	int scale = 0;

	// read in scale
	while (matches != EOF && done == 0) {
		matches = fscanf(fp,"%s",comm);
		if (strcmp(comm,scale_comm) == 0) done = 1;
	}
	if (matches == EOF) {
		fprintf(stderr,"Warning: Could not find -scale identifier.\n");
		return scale;
	}
	done = 0;
	// char scomm[500];
	// int scomm_len = 500;
	while (done == 0) {
		// Read a line of the input
		// printf("Reading a line from the input file...\n");
		// fgets(scomm,scomm_len,fp);
		// Read a line of the input
		// matches = sscanf(scomm,"%d",&scale);
		matches = fscanf(fp,"%d",&scale);
		if (matches == 1) done = 1;
	}
	return scale;
}
