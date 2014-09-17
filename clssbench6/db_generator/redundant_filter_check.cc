// File: redundant_filter_check.cc
// David E. Taylor
// Applied Research Laboratory
// Department of Computer Science and Engineering
// Washington University in Saint Louis
// det3@arl.wustl.edu
//
// Compares two filters and returns a Boolean (1 = True, 0 = False)

#include "stdinc.h"
#include "redundant_filter_check.h"

int redundant_check(struct filterV6 filt1, struct filterV6 filt2){
  return (sa_prefix_match(filt1, filt2) == 1 &&
	  da_prefix_match(filt1, filt2) == 1 &&
	  sp_range_match(filt1, filt2) == 1 &&
	  dp_range_match(filt1, filt2) == 1 &&
	  prot_match(filt1, filt2) == 1 &&
	  flag_match(filt1,filt2) == 1);
}

int sa_prefix_match(struct filterV6 filt1, struct filterV6 filt2){

  InetAddress128 addr1, addr2;
  int len;

  len = filt1.sa_len;
  // Check source address length
  if (len == filt2.sa_len) {
    if (len != 0){
      // Check source address prefixes
      addr1 = filt1.sa;
      // mask bits
      //addr1 = ((addr1 >> (ADDRLEN-len)) << (ADDRLEN-len));
      addr1 = addr1.subbit128(0,len);
      
      addr2 = filt2.sa;
      // mask bits
      //addr2 = ((addr2 >> (ADDRLEN-len)) << (ADDRLEN-len));
      addr2 = addr2.subbit128(0,len);
    }
    // Check source address match
    if (addr1 == addr2) return 1;
  }
  return 0;
}

int da_prefix_match(struct filterV6 filt1, struct filterV6 filt2){

  InetAddress128 addr1, addr2;
  int len;

  len = filt1.da_len;
  // Check source address length
  if (len == filt2.da_len) {
    if (len != 0){
      // Check source address prefixes
      addr1 = filt1.da;
      // mask bits
      //addr1 = ((addr1 >> (ADDRLEN-len)) << (ADDRLEN-len));
      addr1 = addr1.subbit128(0,len);
      
      addr2 = filt2.da;
      // mask bits
      //addr2 = ((addr2 >> (ADDRLEN-len)) << (ADDRLEN-len));
      addr2 = addr2.subbit128(0,len);
    }
    
    // Check source address match
    if (addr1 == addr2) return 1;
  }
  return 0;
}


int sp_range_match(struct filterV6 filt1, struct filterV6 filt2){
  if (filt1.sp[0] == filt2.sp[0] && 
      filt1.sp[1] == filt2.sp[1])
    return 1;
  else
    return 0;
}

int dp_range_match(struct filterV6 filt1, struct filterV6 filt2){
  if (filt1.dp[0] == filt2.dp[0] && 
      filt1.dp[1] == filt2.dp[1])
    return 1;
  else
    return 0;
}

int prot_match(struct filterV6 filt1, struct filterV6 filt2){
  if (filt1.prot_num == filt2.prot_num) return 1;
  else return 0;
}

int flag_match(struct filterV6 filt1, struct filterV6 filt2){
  if (filt1.flags == filt2.flags && filt1.flags_mask == filt2.flags_mask) return 1;
  else return 0;
}

