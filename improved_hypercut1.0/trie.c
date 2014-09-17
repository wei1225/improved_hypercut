#include "stdinc.h"
#include "hypercut.h"
#include "trie.h"
#include "dheap.h"
#include "list.h"


trie::trie(int N1, int numrules1, int bucketSize1,int NClmt1, float spfac1,float spfac2,float spfac3,float a1,float b1,float c1,struct pc_rule* rule1, int redun1, int push1, int pushthresh1) {
// Initialize trie that can have up to N1 nodes.
  int i;
  N = N1;
  numrules = numrules1;
  bucketSize = bucketSize1;
  NClmt=NClmt1;
  spfac_1 = spfac1;
  spfac_2 = spfac2;
  spfac_3 = spfac3;
  a=a1;
  b=b1;
  c=c1;
  rule = rule1;
  redun = redun1;
  push = push1;
  pushthresh = pushthresh1;
  nodeSet = new nodeItem[N+1];
  root = 1; 
  n = 1;
  n2 = 0;
  n3 = 0;
  crules=0;
  n4 = 0;
  n5 = 0;
  worstcost = 0.0;

  for(i=1; i<=N; i++) nodeSet[i].child = (int *)malloc(sizeof(int));

  printf("size of tree node %d\n", sizeof(nodeItem));
  
  nodeSet[root].isleaf = 0;
  nodeSet[root].nrules = numrules;
  nodeSet[root].nonemptylist = 0;
  for (i=0; i<MAXDIMENSIONS; i++){
    nodeSet[root].field[i].low = 0;
    if(i<2) nodeSet[root].field[i].high = 0xffffffff;
    else if(i==4) nodeSet[root].field[i].high = 255;
    else nodeSet[root].field[i].high = 65535; 
    nodeSet[root].dim[i] = 0;
    nodeSet[root].ncuts[i] = 1;
  }
  
  nodeSet[root].ruleid = (int *)calloc(numrules, sizeof(int));
  for(i=0; i< numrules; i++) nodeSet[root].ruleid[i] = i;
	
  freelist = 2;	// create list of unallocated nodes
  for (i = 2; i < N; i++) nodeSet[i].child[0] = i+1;
  nodeSet[N].child[0] = Null;
    
  createtrie();
	
}

trie::~trie() { delete [] nodeSet; }

void trie::choose_np_dim(nodeItem *v){
  int *nrule=NULL;
  int min_dim; 
  double min_pef=100000.0;
  int r1[MAXDIMENSIONS], lo1[MAXDIMENSIONS], hi1[MAXDIMENSIONS];
  int i1[MAXDIMENSIONS];
  int nc[MAXDIMENSIONS];
  int NC;
  int done;
  int *nr[MAXDIMENSIONS];      //number of rules in each child
  int ncomponent[MAXDIMENSIONS];
  float avgcomponent;         
  int i,j,k;
  int _lo,_hi,_r;
  int temp, tmpkey;
  dheap H1(MAXRULES,2);
  dheap H2(MAXRULES,2);
  int Nr;
  int flag1 = 1;
  int empty1;
  int total_rule;
  int total_rule1=0;
  
  for(k=0; k<MAXDIMENSIONS; k++) nr[k]=NULL;

  //count the unique components on each dimension
  for(k=0; k<MAXDIMENSIONS; k++){
    ncomponent[k]=0;    
    for(i = 0; i<v->nrules; i++){
      if(rule[v->ruleid[i]].field[k].low < v->field[k].low){
        H1.insert(v->ruleid[i], v->field[k].low); 	
      }else{
        H1.insert(v->ruleid[i], rule[v->ruleid[i]].field[k].low);
      }
    }
    while(H1.findmin()!= Null){
      temp = H1.findmin();
      tmpkey = H1.key(temp);
      while(tmpkey == H1.key(H1.findmin())){
        j = H1.deletemin();
    	  if(rule[j].field[k].high > v->field[k].high){
    	    H2.insert(j, v->field[k].high);	
    	  }else{
    	    H2.insert(j, rule[j].field[k].high);
    	  }
      }
      while(H2.findmin() != Null){
    	  ncomponent[k]++;
    	  j = H2.findmin();
    	  tmpkey = H2.key(j);
        while(H2.findmin() != Null && H2.key(j) == tmpkey){
    	    H2.deletemin();
    	    j = H2.findmin();
    	  }
      }  		
    }     
  }
  
  //choose the set of dimensions to cut
  if(ncomponent[0]==1 && ncomponent[1]==1 && ncomponent[2]==1 &&
     ncomponent[3]==1 && ncomponent[4]==1){
      for(k=0; k<MAXDIMENSIONS; k++){
        v->dim[k] = 0;
        v->ncuts[k] = 1;
      }
      printf("here you go\n");
      return;
  }
  
  avgcomponent = 0.0;
  for(k=0; k<MAXDIMENSIONS; k++) avgcomponent += ncomponent[k];
  avgcomponent = avgcomponent/MAXDIMENSIONS;
  
  for(k=0; k<MAXDIMENSIONS; k++){
    if(ncomponent[k] > avgcomponent && v->field[k].high - v->field[k].low > 1){
      v->dim[k] = 1;
      nc[k] = 2;
    }else{
      v->dim[k] = 0;
      nc[k] = 1;
    }
  }

  //choose the number of cuts
  NC=1;
  for(k=0; k<MAXDIMENSIONS; k++){
    if(v->dim[k] == 1){
      NC = NC * nc[k];
    }
  }
 
 while(NC*2<=spfac_1 * sqrt(v->nrules) && NC*2<= MAXCUTS && NC*2<= NClmt && total_rule1<=spfac_2*v->nrules){
    NC*=2;
    
    //nrule=(int *)malloc(NC*sizeof(int));
    int nrule[NC];
    for(k=0; k<MAXDIMENSIONS; k++){
      if(v->dim[k] == 1){
        nc[k]*=2;
        r1[0] = (v->field[0].high - v->field[0].low)/nc[0];//可以改成移位
        lo1[0] = v->field[0].low;
        hi1[0] = lo1[0] + r1[0];
        int child_id=0;
        for(i=0;i<NC;i++)
          nrule[i]=0;
        for(i1[0] = 0; i1[0] < nc[0]; i1[0]++){
          r1[1] = (v->field[1].high - v->field[1].low)/nc[1];
          lo1[1] = v->field[1].low;
          hi1[1] = lo1[1] + r1[1];
          for(i1[1] = 0; i1[1] < nc[1]; i1[1]++){
            r1[2] = (v->field[2].high - v->field[2].low)/nc[2];
            lo1[2] = v->field[2].low;
            hi1[2] = lo1[2] + r1[2];
            for(i1[2] = 0; i1[2] < nc[2]; i1[2]++){
              r1[3] = (v->field[3].high - v->field[3].low)/nc[3];
              lo1[3] = v->field[3].low;
              hi1[3] = lo1[3] + r1[3]; 
              for(i1[3] = 0; i1[3] < nc[3]; i1[3]++){
                r1[4] = (v->field[4].high - v->field[4].low)/nc[4];
                lo1[4] = v->field[4].low;
                hi1[4] = lo1[4] + r1[4]; 
                for(i1[4] = 0; i1[4] < nc[4];i1[4]++,child_id++){
                  empty1 = 1;
                  for(j=0; j<v->nrules; j++){
                    flag1 = 1;
                    for(int k2 = 0; k2 < MAXDIMENSIONS; k2++){
                      if(rule[v->ruleid[j]].field[k2].low > hi1[k2] || rule[v->ruleid[j]].field[k2].high < lo1[k2]){
                        flag1 = 0;
                        break;
                      }
                    }
                    
                    if(flag1 == 1){
                      
                      empty1 = 0;
                      nrule[child_id]++;
                    }
                  }
                  
                  lo1[4] = hi1[4] + 1;
                  hi1[4] = lo1[4] + r1[4];
                  
                }
                lo1[3] = hi1[3] + 1;
                hi1[3] = lo1[3] + r1[3];
              }
              lo1[2] = hi1[2] + 1;
              hi1[2] = lo1[2] + r1[2];
            }
            lo1[1] = hi1[1] + 1;
            hi1[1] = lo1[1] + r1[1];
          }
          lo1[0] = hi1[0] + 1;
          hi1[0] = lo1[0] + r1[0];
        }
        
        total_rule=0;
        for(i=0;i<NC;i++){
          total_rule+=nrule[i];
          
        }
        double sum=0.0;
        double afs=total_rule/(double)NC;
        double opfs=v->nrules/(double)NClmt;
        double index_dump=(total_rule-v->nrules)/(double)v->nrules;
        for(i=0;i<NC;i++){
          sum+=(nrule[i]>afs ? nrule[i]-afs:afs-nrule[i]); 
        }
        double index_bat=(sum/NC)/opfs;
        double index_afs=afs/opfs;
        double pef=a*index_dump+b*index_bat+c*index_afs;
        printf("total_rule=%d  ",total_rule);
        printf("v->nrules=%d  ",v->nrules);
        printf("NC=%d\n",NC);
        //printf("afs[%d]=%f\n",k,afs);
        //printf("sum[%d]=%f\n",k,sum);
        printf("index_dump[%d]=%f  ",k,index_dump); 
        printf("index_bat[%d]=%f  ",k,index_bat); 
        printf("index_afs[%d]=%f  ",k,index_afs);
         
        printf("pef[%d]=%f\n",k,pef);    
         
        if(pef<min_pef){
          min_pef=pef;
          min_dim=k;
          crules=n3+total_rule;
          total_rule1=total_rule;
        }
        nc[k]/=2;
      }
       
    }
   
    //printf("NC=%d\n",NC);
    //printf("min_dim=%d\n",min_dim);
    for(k=0; k<MAXDIMENSIONS; k++){
      if(v->dim[k]== 1 && k==min_dim)nc[k]*=2;
      //printf("_nc[%d]=%d\n",k,nc[k]);
      if(nc[k] == 1) v->dim[k] = 0;
    }  
     
  }
  //printf("v->nrules=%d,NC=%d\n",v->nrules,NC);
  for(k=0; k<MAXDIMENSIONS; k++){
    if(v->dim[k] == 1){
      v->ncuts[k] = nc[k];
    }else{
      v->ncuts[k] = 1;
    }
  }
   //printf("NC = %d,nc[0]=%d,nc[1]=%d,nc[2]=%d,nc[3]=%d,nc[4]=%d,v->nrules=%d\n", NC, v->ncuts[0], v->ncuts[1], v->ncuts[2], v->ncuts[3], v->ncuts[4],v->nrules);
   //printf(">>NC = %d (%d:%d:%d:%d:%d) @ layer %d with %d rules\n", NC, v->ncuts[0], v->ncuts[1], v->ncuts[2], v->ncuts[3], v->ncuts[4], pass, v->nrules);
  v->child = (int *)realloc(v->child, NC * sizeof(int));
	
}

void trie::remove_redundancy(nodeItem *v){
  	
  int cover;
  int tmp, tmp2;
  
  if(v->nrules == 1) return;
  
  tmp = v->nrules -1;
  tmp2 = v->nrules -2;
  while(tmp >= 1){
    for(int i = tmp2; i >= 0; i--){
      for(int k= 0; k < MAXDIMENSIONS; k++){
      	cover = 1;
        if(max(rule[v->ruleid[i]].field[k].low, v->field[k].low) > max(rule[v->ruleid[tmp]].field[k].low, v->field[k].low) ||
           min(rule[v->ruleid[i]].field[k].high,v->field[k].high)< min(rule[v->ruleid[tmp]].field[k].high,v->field[k].high)){
          cover = 0;
          break;
        }
      }
      if(cover == 1){
      	
      	for(int j = tmp; j < v->nrules-1; j++){
      	  v->ruleid[j] = v->ruleid[j+1]; 	
      	}
      	
      	v->nrules --;
      	//printf("rule %d is covered by %d, becomes %d rules @ %d\n", v->ruleid[tmp], v->ruleid[i], v->nrules, pass); 
      	n2++;
      	break;
      }
    }
    tmp --;
    tmp2 --;
  } 	                       
   		
}


void trie::pushing_rule(nodeItem *v){
  
  int i, j;
  int index = 0;
  int cover = 0;
  //int tmp = v-> nrules;
  
  for(i = 0; i < v->nrules; i++){
    if(cover == 1) i--;
    cover = 1;
    for(int k=0; k < MAXDIMENSIONS; k++){
      if(v->dim[k] == 1){
      	if(rule[v->ruleid[i]].field[k].low > v->field[k].low ||
      	   rule[v->ruleid[i]].field[k].high < v->field[k].high){
      	  cover = 0;
      	  break;
      	}
      }
    }
    if(cover == 1){
      v->nonemptylist = 1;
      v->rulelist = (int *)realloc(v->rulelist, (index+1)*sizeof(int));
      v->rulelist[index] = v->ruleid[i];
      
      for(j = i+1; j < v->nrules; j++){
      	v->ruleid[j-1] = v->ruleid[j];
      }
      v->nrules --;
      index ++;
      v->lengthoflist = index;
      n3++;
      n5++;
      //break; //only hold 1!!!
    }
  }
  
  //if(index != 0){
  //  printf("hold %d rules @ layer %d, %d => %d rules\n", index, pass, tmp, v->nrules);
  //}
    	
}



void trie::createtrie(){
  
  list Q(MAXNODES);
  int last;
  int nr;
  int empty;
  int u,v;
  int r[MAXDIMENSIONS], lo[MAXDIMENSIONS], hi[MAXDIMENSIONS];
  int index, flag;
  int i[MAXDIMENSIONS];
  int j, k, t;
  
  Q &= root; last = root; pass = 0;
  
  while(Q(1) != Null ){
  	
    v = Q(1); Q <<= 1;
    
    //printf("dequeue %d\n", v);
    if(redun == 1){
      remove_redundancy(&nodeSet[v]);
    }
    if(nodeSet[v].nrules > bucketSize){
      choose_np_dim(&nodeSet[v]);
      if(push == 1 && pass <= pushthresh ){
        pushing_rule(&nodeSet[v]);
      }
    }
      //printf("crules=%d\n",crules);
      //printf("n3=%d\n",n3);
      if(nodeSet[v].nrules > bucketSize && crules<=spfac_3*numrules){
        index = 0;
        r[0] = (nodeSet[v].field[0].high - nodeSet[v].field[0].low)/nodeSet[v].ncuts[0];
        lo[0] = nodeSet[v].field[0].low;
        hi[0] = lo[0] + r[0];	
        for(i[0] = 0; i[0] < nodeSet[v].ncuts[0]; i[0]++){
      
          r[1] = (nodeSet[v].field[1].high - nodeSet[v].field[1].low)/nodeSet[v].ncuts[1];
          lo[1] = nodeSet[v].field[1].low;
          hi[1] = lo[1] + r[1];
          for(i[1] = 0; i[1] < nodeSet[v].ncuts[1]; i[1]++){
      
            r[2] = (nodeSet[v].field[2].high - nodeSet[v].field[2].low)/nodeSet[v].ncuts[2];

            lo[2] = nodeSet[v].field[2].low;
            hi[2] = lo[2] + r[2];
            for(i[2] = 0; i[2] < nodeSet[v].ncuts[2]; i[2]++){
      
              r[3] = (nodeSet[v].field[3].high - nodeSet[v].field[3].low)/nodeSet[v].ncuts[3];
              lo[3] = nodeSet[v].field[3].low;
              hi[3] = lo[3] + r[3];
              for(i[3] = 0; i[3] < nodeSet[v].ncuts[3]; i[3]++){
      
                r[4] = (nodeSet[v].field[4].high - nodeSet[v].field[4].low)/nodeSet[v].ncuts[4];
                lo[4] = nodeSet[v].field[4].low;
                hi[4] = lo[4] + r[4];
                for(i[4] = 0; i[4] < nodeSet[v].ncuts[4]; i[4]++){
      
                  empty = 1;
	          nr = 0;
                  for(j=0; j<nodeSet[v].nrules; j++){
                    flag = 1;
                    for(k = 0; k < MAXDIMENSIONS; k++){
                      if(rule[nodeSet[v].ruleid[j]].field[k].low > hi[k] || rule[nodeSet[v].ruleid[j]].field[k].high < lo[k]){
                        flag = 0;
                        break;
                      }
                    }
                    if(flag == 1){
                      empty = 0;
                      nr++;
                    }
                  }
                
                  if(!empty){
                    n++;
                    nodeSet[v].child[index] = freelist; 
                    u = freelist;
                    freelist = nodeSet[freelist].child[0];	
                    nodeSet[u].nrules = nr;
                    nodeSet[u].nonemptylist = 0;
                  
                    if(nr <= bucketSize){
                      nodeSet[u].isleaf = 1;
                      n3 += nr;
                    }else{
                      nodeSet[u].isleaf = 0;
                      Q &= u;
                      //printf("enque %d\n", u);
                    }
                    for (t=0; t<MAXDIMENSIONS; t++){
                      if(nodeSet[v].dim[t] == 1){
        	        nodeSet[u].field[t].low = lo[t];
	                nodeSet[u].field[t].high= hi[t];
                      }else{
                        nodeSet[u].field[t].low = nodeSet[v].field[t].low;
	                nodeSet[u].field[t].high= nodeSet[v].field[t].high;
                      }
                    }
                   
                    int s = 0;
                    nodeSet[u].ruleid = (int *)calloc(nodeSet[v].nrules, sizeof(int));
                    for(j=0; j<nodeSet[v].nrules; j++){
                      flag = 1;
                      for(k = 0; k < MAXDIMENSIONS; k++){
                        if(nodeSet[v].dim[k] == 1){
                          if(rule[nodeSet[v].ruleid[j]].field[k].low > hi[k] || rule[nodeSet[v].ruleid[j]].field[k].high < lo[k]){
                            flag = 0;
                            break;
                          }
                        }
                      }
                      if(flag == 1){
                        nodeSet[u].ruleid[s] = nodeSet[v].ruleid[j];
                        s++;
                      }
                    }
                  }else{
                    nodeSet[v].child[index] = Null;
                    //printf("Null=%d\n",Null);
                  }  
                  
                  index ++;
                  lo[4] = hi[4] + 1;
                  hi[4] = lo[4] + r[4];
                }
                lo[3] = hi[3] + 1;
                hi[3] = lo[3] + r[3];
              }
              lo[2] = hi[2] + 1;
              hi[2] = lo[2] + r[2];
            }
            lo[1] = hi[1] + 1;
            hi[1] = lo[1] + r[1];
          }
          lo[0] = hi[0] + 1;
          hi[0] = lo[0] + r[0];
        }
      
        if(v == last){
	  pass ++;
	  last = Q.tail();
        }
      }
    else{
      nodeSet[v].isleaf = 1;
      n3 += nodeSet[v].nrules;
      
      
    }
    
  }	
}

//perform packet classification
int trie::trieLookup(int* header){
  
  int index[MAXDIMENSIONS];
  
  int cvalue[MAXDIMENSIONS];
  int cover, cchild;
  int cuts[MAXDIMENSIONS];
  int cnode = root;
  int match = 0;
  int nbits[MAXDIMENSIONS];
  int i,j,k;
  int minID = numrules;
  int *cnodelist = NULL;
  int ncnode = 0;
  float cost = 0.0;
  
  for(i = 0; i< MAXDIMENSIONS; i++){
    if(i == 4) index[i] = 8;
    else if(i >= 2) index[i] = 16;
    else index[i] = 32;
  }
 
  while(nodeSet[cnode].isleaf != 1){
        
    n4 += NODESIZE;
    cost += NODESIZE;
    if(nodeSet[cnode].nonemptylist == 1){
      cnodelist = (int *)realloc(cnodelist, (ncnode+1)*sizeof(int));
      cnodelist[ncnode] = cnode;
      ncnode ++;
    }


    if(nodeSet[cnode].nonemptylist == 1){
      for(i = 0; i < nodeSet[cnode].lengthoflist; i++){
        n4+=RULEPTSIZE+RULESIZE;
        cost+=RULEPTSIZE+RULESIZE;
        cover = 1;
        for(k = 0; k < MAXDIMENSIONS; k++){
          if(rule[nodeSet[cnode].rulelist[i]].field[k].low > header[k] ||
             rule[nodeSet[cnode].rulelist[i]].field[k].high< header[k]){
            cover = 0;
            break;
          }
        }
        if(cover == 1){
          //printf("rule %d is matched internally\n", nodeSet[cnode].rulelist[i]);
      	  if(minID > nodeSet[cnode].rulelist[i]) minID = nodeSet[cnode].rulelist[i];
          match = 1;
          break;
        }
      }
    }
    
    for(i = 0; i< MAXDIMENSIONS; i++){nbits[i] = 0;}
    for(i = 0; i< MAXDIMENSIONS; i++){
      if(nodeSet[cnode].dim[i] == 1){
        cuts[i] = nodeSet[cnode].ncuts[i];
        while(cuts[i] != 1){
      	  nbits[i] ++;
      	  cuts[i] /= 2;
      	}
      	cvalue[i] = 0;
      	for(k = index[i]; k > index[i] - nbits[i]; k--){
          if((header[i] & 1<<(k-1)) != 0){
      	    cvalue[i] += (int)pow(2, k-index[i]+nbits[i]-1);
          }
        }
      }else{
        cvalue[i] = 0;
      }
    }
    
    cchild = 0;
    for(i = 0; i< MAXDIMENSIONS-1; i++){
      for(k = i+1; k < MAXDIMENSIONS; k ++){
      	 cvalue[i] *= nodeSet[cnode].ncuts[k];
      }
      cchild += cvalue[i];
    }
    cchild += cvalue[MAXDIMENSIONS-1];
    
    cnode = nodeSet[cnode].child[cchild];  	
    
    if(cnode == Null) break;
    
    for(i = 0; i< MAXDIMENSIONS; i++){
      index[i] -= nbits[i];
    }
  }
  
  if(cnode != Null){
    
    if(nodeSet[cnode].nonemptylist == 1){
      for(i = 0; i < nodeSet[cnode].lengthoflist; i++){
        n4+=RULEPTSIZE+RULESIZE;
        cost+=RULEPTSIZE+RULESIZE;
        cover = 1;
        for(k = 0; k < MAXDIMENSIONS; k++){
          if(rule[nodeSet[cnode].rulelist[i]].field[k].low > header[k] ||
             rule[nodeSet[cnode].rulelist[i]].field[k].high< header[k]){
            cover = 0;
            break;
          }
        }
        if(cover == 1){
          //printf("rule %d is matched internally\n", nodeSet[cnode].rulelist[i]);
      	  if(minID > nodeSet[cnode].rulelist[i]) minID = nodeSet[cnode].rulelist[i];
          match = 1;
          break;
        }
      }
    }
    
    for( i = 0; i < nodeSet[cnode].nrules; i++){
      n4+=RULEPTSIZE+RULESIZE;
      cost+= RULEPTSIZE+RULESIZE;
      cover = 1;
      for(k = 0; k < MAXDIMENSIONS; k++){
        if(rule[nodeSet[cnode].ruleid[i]].field[k].low > header[k] ||
           rule[nodeSet[cnode].ruleid[i]].field[k].high< header[k]){
          cover = 0;
          break;
        }
      }
      if(cover == 1){
      	//printf("rule %d is matched in leaf\n", nodeSet[cnode].ruleid[i]);
        if(minID > nodeSet[cnode].ruleid[i]) minID = nodeSet[cnode].ruleid[i];
        match = 1;
        break;
      }
    }
  }

  if(ncnode != 0){
    for(j=ncnode-1; j>=0; j--){
      for(i = 0; i < nodeSet[cnodelist[j]].lengthoflist; i++){
        if(nodeSet[cnodelist[j]].rulelist[i] < minID){
          n4 += RULEPTSIZE+RULESIZE;
          cost += RULEPTSIZE+RULESIZE;
        }else{
          n4 += RULEPTSIZE;
          cost += RULEPTSIZE;
          break;
        }
        cover = 1;
        for(k = 0; k < MAXDIMENSIONS; k++){
          if(rule[nodeSet[cnodelist[j]].rulelist[i]].field[k].low > header[k] ||
             rule[nodeSet[cnodelist[j]].rulelist[i]].field[k].high< header[k]){
            cover = 0;
            break;
          }
        }
        if(cover == 1){
          //printf("rule %d is matched internally\n", nodeSet[cnode].rulelist[i]);
          if(minID > nodeSet[cnode].rulelist[i]) minID = nodeSet[cnode].rulelist[i];
          minID = nodeSet[cnodelist[j]].rulelist[i];
          match = 1;
          break;
        }
      }
    }
  }
  
  if(worstcost < cost) worstcost = cost;
  
  if(match == 1){
    return minID;
  }else{
    return Null;
  }
		
}
