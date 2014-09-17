// File: dbintree.h
// David E. Taylor
// Applied Research Laboratory
// Department of Computer Science and Engineering
// Washington University in Saint Louis
// det3@arl.wustl.edu
//
// Class definition for dbintree
//   - Maintains a binary tree for generating destination addresses given a list of
//     destination address prefix lengths
//   - Reads in nesting, skew, and correlation parameters from seed file
//   - Guarantees nesting will not be exceeded, attempts to maintain skew and correlation
//     statistics

typedef int level;

class dbintree {

	struct tnode {
		level lvl;
		struct tnode *parent;
		struct tnode *child0;
		struct tnode *child1;
		int wt_child0;
		int wt_child1;
		int valid;
	} *tnodes;

	struct tnode *root; // pointer to root node
	float *skew; // array of target skews for each level
	float *corr; // array of target correlations for each level 
	float *p1child; // probability that a node at a given level has one child
	float *p1childleft; //left child probability that a node at a given level has one child
	float *p1childright; //right child probability that a node at a given level has one child
	float *p2child; // probability that a node at a given level has two children
	int num_tnodes; // number of tree nodes 
	int Nest; // Maximum allowed nesting

public: dbintree();
		~dbintree();
		void delete_node(struct tnode *me);
		void read_skew(FILE*); // read in destination address tree statistics
		void read_config_skew(FILE* fp_in);
		void read_nest(FILE*); // read in source address tree statistics
		void read_corr(FILE*); // read in address correlation statistics
		void print_nest(FILE*); //print 
		void print_skew(FILE*); // print average skew per level
		void print_corr(FILE*); // print correlation per level
		void mappingCorr();
		void MappingSkew(unsigned int scale);  //map IPv4 Skew and Branch to IPv6;
};

