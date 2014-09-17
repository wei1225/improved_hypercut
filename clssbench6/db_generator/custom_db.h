// File: custom_db.h
// David E. Taylor
// Applied Research Laboratory
// Department of Computer Science and Engineering
// Washington University in Saint Louis
// det3@arl.wustl.edu
//
// Generates a synthetic database from seed file and input parameters

#ifndef __CUSTOM_DB_H_ 
#define __CUSTOM_DB_H_

int custom_db_gen(int num_filters, FilterList* filters, FILE* fp_in, int smoothness, float addr_scope, float port_scope, int branch);
int read_scale(FILE *fp);
double random_scope(float scope_x);
void fprint_filter(FILE *fp, struct filterV6* filt);
void select_ports(int, struct filterV6*, PortList*, PortList*, PortList*, PortList*);
int SetLast64bit(int num_filters,struct filterV6 filters[]);
int remove_redundant_filters(int num_filters, FilterList* filters, filterV6* temp_filters);

struct rtuple {
  int da_len;
  int sa_len;
  int dp_width;
  int sp_width;
  int prot_valid;
  int freq;
  double ri_high;
  double ri_low;
};

struct rwidth {
  int width;
  int high;
  int low;
};

struct rprot {
  int num;
  float ww_high;
  float ww_low;
  float ws_high;
  float ws_low;
  float ss_high;
  float ss_low;
};

#endif

