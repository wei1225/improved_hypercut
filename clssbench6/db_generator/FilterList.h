// File: FilterList.h
// David E. Taylor
// Applied Research Laboratory
// Department of Computer Science and Engineering
// Washington University in Saint Louis
// det3@arl.wustl.edu
//
// Class definition for FilterList

// List of filters with dynamic memory allocation
// edit by zhouxj for V6 no any guarantee
#ifndef __FILTERLIST_H_ 
#define __FILTERLIST_H_

#define TCPPRO 0.6
#define UDPPRO 0.8

struct FilterList_item {
	struct filterV6 filt;
  //struct filterV6 filt;
  struct FilterList_item *prev;
  struct FilterList_item *next;
};

class FilterList {
  struct FilterList_item *first; // beginning of list
  struct FilterList_item *last;  // end of list
  int num;   // number of filters in the list
 public: FilterList();
  ~FilterList();
  void	clear();		// remove everything
  struct FilterList_item* operator()(int);    // access item
  void	insert(struct FilterList_item*, struct filterV6); // insert filter at prior to given item
  void	operator&=(struct filterV6);  // append item
  void	operator<<=(int);	    // remove initial items
  void	operator=(FilterList*);	    // copy list
  void	push(struct filterV6);        // prepend item
  void  print(FILE *fp);        // Print filters
  int	size();			// return size of list
 private:
 	bool addrSort(FilterList_item * lhs,FilterList_item * rhs);
};
  
#endif

