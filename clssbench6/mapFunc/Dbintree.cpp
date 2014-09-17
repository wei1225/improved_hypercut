// File: dbintree.cc
// David E. Taylor
// Applied Research Laboratory
// Department of Computer Science and Engineering
// Washington University in Saint Louis
// det3@arl.wustl.edu
//
// Class definition for dbintree
#include "stdinc.h"
#include "Dbintree.h"

//#define _DEBUG
dbintree::dbintree() {
	// Initialize to graph with N vertices and no edges.
	skew = new float[ADDRLEN+1];
	corr = new float[ADDRLEN+1];
	p1child = new float[ADDRLEN+1];
	p1childleft = new float[ADDRLEN+1];
	p1childright = new float[ADDRLEN+1];
	p2child = new float[ADDRLEN+1];
	num_tnodes = 0;
	root = NULL;
	for (int u = 0; u < ADDRLEN+1; u++) {
		skew[u] = 0;
		corr[u] = 0;
		p1child[u] = 0;
		p1childleft[u] =0;
		p1childright[u]=0;
		p2child[u] = 0;
	}
}

dbintree::~dbintree() {
	delete(skew);
	delete(corr);
	delete(p1child);
	delete(p1childleft);
	delete(p1childright);
	delete(p2child);
	// call recursive node destructor
	if (root != NULL) delete_node(root);
}

void dbintree::delete_node(struct tnode *me){
	if (me->child0 != NULL) delete_node(me->child0);
	if (me->child1 != NULL) delete_node(me->child1);
	delete(me);
	return;
}

void dbintree::read_nest(FILE* fp_in){
	int done = 0;
	int matches = MAXFILTERS;
	char comm[7];
	char da_comm[]="-dnest";

	// read in destination address nest
	// printf("read in destination address nest\n");
	while (matches != EOF && done == 0) {
		matches = fscanf(fp_in,"%s",comm);
		// printf("comm = %s\n",comm);
		// printf("matches = %d\n",matches);
		if (strcmp(comm,da_comm) == 0) done = 1;
	}
	if (matches == EOF) {
		fprintf(stderr,"No destination address nest specified for custom distribution.\n");
		exit(1);
	}
	matches = fscanf(fp_in,"%d",&Nest);
	// printf("matches = %d\n",matches);
	// printf("Nest = %d\n",Nest);
	return;
}

void dbintree::print_nest(FILE* fp_in)
{
	fprintf(fp_in,"-dnest\n%d\n#\n",Nest);
	return;
}

void dbintree::read_skew(FILE* fp_in){
	int done = 0;
	int matches = MAXFILTERS;
	int level;
	float p1_t; 
	float p1_left;
	float p1_right;
	float p2_t;
	float f_skew;
	char comm[7];
	char sa_comm[]="-dskew";

	// read in destination address skew
	// printf("read in destination address skew\n");
	while (matches != EOF && done == 0) {
		matches = fscanf(fp_in,"%s",comm);
		// printf("comm = %s\n",comm);
		// printf("matches = %d\n",matches);
		if (strcmp(comm,sa_comm) == 0) done = 1;
	}
	if (matches == EOF) {
		fprintf(stderr,"No destination address skew specified for custom distribution.\n");
		exit(1);
	}
	done = 0;
	while(done == 0){
		matches = fscanf(fp_in,"%d\t%f\t%f\t%f",&level,&p1_t,&p2_t,&f_skew);
		// printf("matches = %d\n",matches);
		// printf("level = %d, skew = %.4f\n",level,skew);
		if (matches == 4) {
			if (level <= ADDRLEN) {
				if (level <= ADDRLEN) {
					p1child[level] = p1_t;
					p1childleft[level] =500.0;
					p1childright[level] = 500.0;
					p2child[level] = p2_t;
					skew[level] = f_skew;
				}
			}
			else {
				fprintf(stderr,"Level for destination address skew is greater than 128.\n");
				exit(1);
			}
			// printf("Read line: %d\t%.4f\t%.4f\t%.4f\n",level,p1_t,p2_t,f_skew);
		}
		else {
			done = 1;
		}
	}
	return;
}

void dbintree::read_config_skew(FILE* fp_in){
	int done = 0;
	int matches = MAXFILTERS;
	int level;
	float p1_t; 
	float p1_left;
	float p1_right;
	float p2_t;
	float f_skew;
	char comm[20];
	char sa_comm[]="-dskew";

	// read in destination address skew
	// printf("read in destination address skew\n");
	while (matches != EOF && done == 0) {
		matches = fscanf(fp_in,"%s",comm);
		// printf("comm = %s\n",comm);
		// printf("matches = %d\n",matches);
		if (strcmp(comm,sa_comm) == 0) done = 1;
	}
	if (matches == EOF) {
		fprintf(stderr,"No destination address skew specified for custom distribution.\n");
		exit(1);
	}
	done = 0;
	while(done == 0){
		matches = fscanf(fp_in,"%d\t%f\t%f\t%f\t%f\t%f",&level,&p1_t,&p1_left,&p1_right,&p2_t,&f_skew);
		// printf("matches = %d\n",matches);
		// printf("level = %d, skew = %.4f\n",level,skew);
		if (matches == 6) {
			//if (level <= 32) {
			if (level <= ADDRLEN) {
				p1child[level] = p1_t;
				p1childleft[level] = p1_left;
				p1childright[level] = p1_right;
				p2child[level] = p2_t;
				skew[level] = f_skew;
			}
			else {
				fprintf(stderr,"Level for destination address skew is greater than 128.\n");
				exit(1);
			}
			// printf("Read line: %d\t%.4f\t%.4f\t%.4f\n",level,p1_t,p2_t,f_skew);
		}
		else {
			done = 1;
		}
	}
	return;
}

void dbintree::read_corr(FILE* fp_in){
	int done = 0;
	int matches = 0;
	char comm[30];
	char p_comm[]="-pcorr";
	int index;
	float icorr;

	// read in address correlation
	while (matches != EOF && done == 0) {
		matches = fscanf(fp_in,"%s",comm);
		if (strcmp(comm,p_comm) == 0) done = 1;
	}
	if (matches == EOF) {
		fprintf(stderr,"No prefix correlation specified for custom distribution.\n");
		return;
	}
	done = 0;
	for (int i = 0; i < MAXFILTERS && done == 0; i++){
		matches = fscanf(fp_in,"%d %f",&index,&icorr);
		if(matches ==2)
			corr[index] = icorr;
		else
			done = 1;
	}

	return;
}

void dbintree::print_skew(FILE *fp) {

	fprintf(fp,"-dskew\n");
	//for (int i = 0; i < 33; i++) {
	for (int i = 0; i <= 64; i++) {
		fprintf(fp,"%d\t%.8f\t%.8f\t%.8f\t%.8f\t%.8f\n",
			i,p1child[i],p1childleft[i],p1childright[i],p2child[i],skew[i]);
	}
	for (int i = 65; i < ADDRLEN+1; i++) {
		if (p1child[i] || p2child[i] || skew[i])
			fprintf(fp,"%d\t%.8f\t%.8f\t%.8f\t%.8f\t%.8f\n",
				i,p1child[i],p1childleft[i],p1childright[i],p2child[i],skew[i]);
	}
	fprintf(fp,"#\n");
	return;
}

void dbintree::print_corr(FILE *fp) {

	fprintf(fp,"-pcorr\n");
	//for (int i = 0; i < 33; i++) {
	for (int i = 0; i < ADDRLEN+1; i++) {
		fprintf(fp,"%d\t%.8f\n",i,corr[i]);
	}
	fprintf(fp,"#\n");
	return;
}


void dbintree::mappingCorr()
{
	int tmpCorr[ADDRLEN+1];
	memset(tmpCorr,0,sizeof(tmpCorr));
	for (int i = 0 ; i < 33 ; i++)
	{
		int srcMapLen = MapLenForSkew(i);
		tmpCorr[srcMapLen] = corr[i];
	}
	for (int i = 0 ; i < 33 ; i++)
	{
		corr[i] = tmpCorr[i];
	}
//	memcpy(corr,tmpCorr,sizeof(tmpCorr));
	return ;
}

void dbintree::MappingSkew(unsigned int scale)
{
	float tmpp1child[ADDRLEN+1];
	float tmpp1childleft[ADDRLEN+1];
	float tmpp1childright[ADDRLEN+1];
	float tmpp2child[ADDRLEN+1];
	float tmpskew[ADDRLEN+1];
	for (unsigned int ii = 0; ii < ADDRLEN +1 ; ++ii)
	{
		tmpp1child[ii] = 0;
		tmpp1childleft[ii] = 0;
		tmpp1childright[ii] = 0;
		tmpp2child[ii] = 0;
		tmpskew[ii] = 0;
	}
	unsigned int IPv6Len = 0;
	//前三个就定死这样了,以便尽可能产生001开头的地址
	tmpp1child[0] = 1.0;
	tmpp1childleft[0] = 1.0;
	tmpp1childright[0] = 0.0 ;
	tmpp2child[0] = 0.0;
	tmpskew[0] = 1.0;

	tmpp1child[1] = 1.0;
	tmpp1childleft[1] = 1.0;
	tmpp1childright[1] = 0.0 ;
	tmpp2child[1] = 0.0;
	tmpskew[1] = 1.0;

	tmpp1child[2] = 1.0;
	tmpp1childleft[2] = 0.0;
	tmpp1childright[2] = 1.0 ;
	tmpp2child[2] = 0.0;
	tmpskew[2] = 1.0;

	tmpp1child[3] = 1.0;
	tmpp1childleft[3] = 1.0;
	tmpp1childright[3] = 0.0 ;
	tmpp2child[3] = 0.0;
	tmpskew[3] = 1.0;
	//先对IPv4中4到32进行影射.
	for (unsigned int level = 4 ; level <= 32 ; ++level)
	{
		IPv6Len = MapLenForSkew(level);
		tmpskew[IPv6Len] = skew[level];
		tmpp1child[IPv6Len] = p1child[level];
		if (tmpp1child[IPv6Len])
		{
			tmpp1childleft[IPv6Len] = 0.5;
			tmpp1childright[IPv6Len] = 0.5 ;
		}
		else
		{
			tmpp1childleft[IPv6Len] = 0.0;
			tmpp1childright[IPv6Len] = 0.0 ;
		}
		tmpp2child[IPv6Len] = p2child[level];
	}

	for (unsigned int ii = 0; ii < ADDRLEN +1 ; ++ii)
	{
		p1child[ii] = tmpp1child[ii];
		p1childleft[ii]= tmpp1childleft[ii];
		p1childright[ii] = tmpp1childright[ii];
		p2child[ii] = tmpp2child[ii];
		skew[ii] = tmpskew[ii];
	}

	//其它没有影射到的长度作另外的处理.
	unsigned int lastLevel = 3;
	unsigned int currLevel = 0;
	for (unsigned int level = 4 ; level <=64 ; ++level)
	{
		if ( ( (0 == skew[level]) && (0 == p1child[level]) && (0 ==p2child[level]) )
			|| (500 == p1childleft[level]))
		{
			while(((0 == skew[level]) && (0 == p1child[level]) && (0 ==p2child[level]))
				|| (500 == p1childleft[level]))
				++level;
			currLevel = level;
			float insSkew = (skew[currLevel] - skew [lastLevel])/(currLevel - lastLevel);
			float insp1child = (p1child[currLevel] - p1child[lastLevel])/(currLevel - lastLevel);
			float insp2child = (p2child[currLevel] - p2child[lastLevel])/(currLevel - lastLevel);
			for (int dis=lastLevel+1 ; dis < currLevel ; ++dis  )
			{
				skew[dis] = insSkew * (dis - lastLevel) + skew[lastLevel];
				p1child[dis] = insp1child * (dis - lastLevel) + p1child[lastLevel];
				if (p1child[dis])
				{
					p1childleft[dis] = 0.5;
					p1childright[dis] = 0.5;
				}
				else
				{
					p1childleft[dis] = 0.0;
					p1childright[dis] = 0.0;
				}
				p2child[dis] = insp2child * (dis - lastLevel) + p2child[lastLevel];
			}
		} 
		lastLevel = level;
	}
	return;
}

