// Class automatically generated by Dev-C++ New Class wizard

#include "inetaddress128.h" // class's header file
#include <netinet/in.h>
#include <arpa/inet.h>

// class constructor
InetAddress128::InetAddress128()
{
	address[0]=0;
    address[1]=0;
	address[2]=0;
	address[3]=0;
	prefix=0;
}

InetAddress128::InetAddress128(const string& str)
{
	if(!parse(str))
		cerr<<"parser: bad address\n"<<endl;
}

// class destructor
InetAddress128::~InetAddress128(){}

// 0<=begin<=127
// 1<=len<=32
// begin+len
// e.g. 11110111.subbit(2,3)=0x00000006=00000000000000000000000000000110
// notice : the return type is unsigned int
unsigned int  InetAddress128::subbit32(unsigned int begin,unsigned int len)
{
	if(len>=1 && len<=32 && begin>=0 && begin<=127 && begin+len<=128)
    {
        InetAddress128 addr_t=subbit128(begin,len);
        //printf("Result=%s\n",subbit128(begin,len).tostring());
        addr_t = addr_t >> (128-begin-len);
        return addr_t.address[3];
    }
    else printf("InetAddress128:Wrong Arguments for SUBBIT32\n"); 
}

/*
 * 0<=begin<=127
 * 1<=len<=128
 * notice: the return type is InetAddress128
 */
InetAddress128 InetAddress128::subbit128(unsigned int begin,unsigned int len)
{
	if(len>=1 && len<=128 && begin>=0 && begin<=127 && begin+len<=128)
    {
      InetAddress128 addr_t;
      addr_t=*this;
      addr_t = addr_t << begin;
      addr_t = addr_t.low(len);
      addr_t = addr_t >> begin;
      return addr_t;
    }
    else printf("InetAddress128:Wrong Arguments for SUBBIT128\n"); 
    
}
InetAddress128 InetAddress128::operator^(const InetAddress128 addr)
{
      InetAddress128 addr_t=*this;
      for(int i=0;i<4;i++)
          addr_t.address[i]=addr_t.address[i]^addr.address[i];
      return addr_t;
}


// No description
bool InetAddress128::operator<= (const InetAddress128 addr2) const
{
	for(int i=0;i<4;i++)
	        if(addr2.address[i]<address[i]) 
            {
               return false;
            }
    return true;
}
bool InetAddress128::operator>= (const InetAddress128 addr2) const
{
	for(int i=0;i<4;i++)
	        if(addr2.address[i]>address[i]) 
            {
               return false;
            }
     return true;
}

/*
 * e.g. filter.sa.low(filter.sa_len) 
 * Return the low value accroding to its prefix length
 */
InetAddress128 InetAddress128::low(int prefixLen)
{
	  InetAddress128 addr_t;
      int i=0;
      unsigned mask=0xffffffff;
      for(i=0;i<prefix/32;i++)   
      {                         
          addr_t.address[i]=address[i];
          //printf("len=%d,i=%d mask=%x,sa[%d]=%x\n",prefix,i,mask,i,address[i]);
      }
      
      addr_t.address[i]=address[i]&(mask<<(32-prefix%32));
      //printf("len=%d,i=%d mask=%x,sa[%d]=%x\n",prefix,i,(mask<<(32-prefix%32)),i,addr_t.address[i]);
      i++;
      for(i;i<4;i++)
      {
          addr_t.address[i]=0x0;
          //printf("len=%d,i=%d mask=%x,sa[%d]=%x\n",prefix,i,0,i,address[i]);
      }
      return addr_t;
}

// e.g. filter.sa.high(filter.sa_len) 
// Return the highvalue accroding to its prefix length
InetAddress128 InetAddress128::high(int prefixLen)
{
	  InetAddress128 addr_t;
      int i=0;
      unsigned mask=0xffffffff;
      for(i=0;i<prefix/32;i++)   
      {                         
          addr_t.address[i]=address[i];
          //printf("len=%d,i=%d mask=%x,sa[%d]=%x\n",len,i,mask,i,address[i]);
      }
      
      addr_t.address[i]=address[i]|~(mask<<(32-(prefix%32)));
      //printf("len=%d,i=%d mask=%x,sa[%d]=%x\n",len,i,(mask<<(32-len%32)),i,address[i]);
      i++;
      for(i;i<4;i++)
      {
          addr_t.address[i]=0xffffffff;
          //printf("len=%d,i=%d mask=%x,sa[%d]=%x\n",len,i,0,i,address[i]);
      }
      return addr_t;
}

bool InetAddress128::parse(const string& strString)
{
	typedef string::size_type ssize;
	string srcString;
	ssize floc= strString.find('/');
	if (floc != string::npos)
	{
		srcString= strString.substr(0,floc);
		prefix = atoi(strString.substr(floc+1).c_str());
	}
	else
	{
		srcString = strString;
		prefix = 128 ;
	}
	const char * src = srcString.c_str();
	const int IN6ADDRSZ = 16;
	const int INT16SZ = 2;
	static const char xdigits_l[] = "0123456789abcdef",
		xdigits_u[] = "0123456789ABCDEF";
	unsigned char tmp[IN6ADDRSZ], *tp, *endp, *colonp;
	const char *xdigits, *curtok;
	int ch, saw_xdigit;
	unsigned int tmpval;

	memset((tp = tmp), 0, IN6ADDRSZ);
	endp = tp + IN6ADDRSZ;
	colonp = NULL;
	/* Leading :: requires some special handling. */
	if (*src == ':')
		if (*++src != ':')
			return false;
	curtok = src;
	saw_xdigit = 0;
	tmpval = 0;
	while ((ch = *src++) != '\0') {
		const char *pch;

		if ((pch = strchr((xdigits = xdigits_l), ch)) == NULL)
			pch = strchr((xdigits = xdigits_u), ch);
		if (pch != NULL) {
			tmpval <<= 4;
			tmpval |= (pch - xdigits);
			if (tmpval > 0xffff)
				return false;
			saw_xdigit = 1;
			continue;
		}
		if (ch == ':') {
			curtok = src;
			if (!saw_xdigit) {
				if (colonp)
					return (0);
				colonp = tp;
				continue;
			}
			if (tp + INT16SZ > endp)
				return (0);
			*tp++ = (unsigned char) (tmpval >> 8) & 0xff;
			*tp++ = (unsigned char) tmpval & 0xff;
			saw_xdigit = 0;
			tmpval = 0;
			continue;
		}
		return (0);
	}
	if (saw_xdigit) {
		if (tp + INT16SZ > endp)
			return false;
		*tp++ = (unsigned char) (tmpval >> 8) & 0xff;
		*tp++ = (unsigned char) tmpval & 0xff;
	}
	if (colonp != NULL) {
		/*
		* Since some memmove()'s erroneously fail to handle
		* overlapping regions, we'll do the shift by hand.
		*/
		const int n = tp - colonp;
		int i;

		for (i = 1; i <= n; i++) {
			endp[- i] = colonp[n - i];
			colonp[n - i] = 0;
		}
		tp = endp;
	}
	if (tp != endp)
		return false;
	memcpy(address, tmp, IN6ADDRSZ);
	for(int k = 0; k < 4; k++)
	{
		address[k] = ntohl(address[k]);
	}
	return true;
}

/*
// e.g. : sa.parse(3FFE:3200:0000:0000:0000:0000:0000:0000/24)
bool InetAddress128::parse(const char* str)
{
	unsigned temp_addr[8];
    int matches = sscanf(str, "%x:%x:%x:%x:%x:%x:%x:%x/%d",
		       &temp_addr[0], &temp_addr[1], &temp_addr[2], &temp_addr[3], 
               &temp_addr[4], &temp_addr[5], &temp_addr[6], &temp_addr[7], &prefix);
    // Check the number of matches
    //printf("matches = %d of %s\n",matches,str);
    if (matches == 8 | matches == 9) {
	// Valid format 
    	
        
    	address[0]=( temp_addr[0]<<16 )| temp_addr[1];
    	address[1]=( temp_addr[2]<<16 )| temp_addr[3];
    	address[2]=( temp_addr[4]<<16 )| temp_addr[5];
    	address[3]=( temp_addr[6]<<16 )| temp_addr[7];
//    	printf("addr0=%x\n",address[0]);
//        printf("addr1=%x\n",address[1]);
//        printf("addr2=%x\n",address[2]);
//        printf("addr3=%x\n",address[3]);
    	if(matches == 8) prefix=0;
    }
    else 
        return false;
    return true;
}
*/

// No description
void InetAddress128::printNoLen() const
{
    cout<<ntopNoLen()<<endl;
}

void InetAddress128::printWithLen() const
{
	cout<<ntopWithLen()<<endl;
}


string InetAddress128::ntopNoLen() const
{
	char * str = new char[100];
	sprintf(str,"%04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x", 
		address[0]>>16, address[0]&0x0000ffff,
		address[1]>>16, address[1]&0x0000ffff,
		address[2]>>16, address[2]&0x0000ffff,
		address[3]>>16, address[3]&0x0000ffff);
	string result(str);
	delete(str);
	return result;
}
string InetAddress128::ntopWithLen() const
{
	char *str = new char[100];
	sprintf(str,"%s/%d",ntopNoLen().c_str(),prefix);
	string result(str);
	delete(str);
	return result;
}
char* InetAddress128::tostring() const
{
    char* str=new char[500];
	sprintf(str,"%04x:%04x:%04x:%04x:%04x:%04x:%04x:%04x/%d", 
                address[0]>>16, address[0]&0x0000ffff,
                address[1]>>16, address[1]&0x0000ffff,
                address[2]>>16, address[2]&0x0000ffff,
                address[3]>>16, address[3]&0x0000ffff,prefix);
     return str;
 }



// No description
bool const InetAddress128::operator==(const InetAddress128 &addr2) const
{
    bool equal=true;
    for(int i=0;i<4;i++)
    {
        if(address[i]!=addr2.address[i])
        {
          equal=false;
          break;
        }
    }
    return equal;
}

// No description
bool const InetAddress128::operator>(const unsigned int n)
{
	for(int i=0;i<3;i++)
	{
        if(address[i]!=0)
            return true;
    } 
    if(address[3]>n)  
        return true;
    else
        return false;
}

// No description
bool const InetAddress128::operator<(const InetAddress128 &addr2) const
{
	for(int i=0;i<4;i++)
	{
		if (address[i] >= addr2.address[i])
			 return false;
	}
  return true;
}

// No description
bool const InetAddress128::operator>(const InetAddress128 addr2) const
{
	for(int i=0;i<4;i++)
	{
		if(address[i] <= addr2.address[i])
			return false;
	}
	return true;
}
InetAddress128 InetAddress128::operator<<(unsigned int n)
	{
		InetAddress128 result;
		InetAddress128 temp=*this;
		for(int i =0;i<n/32; ++i)
		{
			address[0]=address[1];  
			address[1]=address[2];  
			address[2]=address[3];  
			address[3]=0;     
		}
		if(n%32!=0)
    {
    	for(int i=0;i<3;i++)
        address[i]=(address[i]<<(n%32))|(address[i+1]>>(32-(n%32)));
      address[3]=address[3]<<(n%32); 
     }
    result = *this;
    *this = temp;
		return result;
	}
	
InetAddress128 InetAddress128::operator>>(unsigned int n)
	{
		InetAddress128 result;
		InetAddress128 temp=*this;
		for(int i=0;i<n/32;++i)       
			{                          
			 	address[3]=address[2];   
			 	address[2]=address[1];   
    	 	address[1]=address[0];   
			 	address[0]=0;            
    	}            
    if(n%32!=0)
		{
    	for(int i=3;i>0;i--)
    	{
         address[i]=(address[i]>>(n%32))|(address[i-1]<<(32-(n%32)));
      }
      address[0]=address[0]>>(n%32);
    }
		result = *this;
    *this = temp;
		return result;
	}
	
ostream& operator<< (ostream& os, const InetAddress128& IPv6)
{
	return os<<IPv6.tostring();
}

/*
// No description
bool InetAddress128::left(unsigned int n)
{
	if(n>128) return false;
	left32(n/32);
    if(n%32!=0)
    {
    	for(int i=0;i<3;i++)
        address[i]=(address[i]<<(n%32))|(address[i+1]>>(32-(n%32)));
      address[3]=address[3]<<(n%32); 
     }
}

bool InetAddress128::right(unsigned int n)
{
	if(n>128) return false;
	right32(n/32);
	
	if(n%32!=0)
	{
    	for(int i=3;i>0;i--)
    	{
            address[i]=(address[i]>>(n%32))|(address[i-1]<<(32-(n%32)));
      }
        address[0]=address[0]>>(n%32);
    }
}
*/
/*
// Arithmetic 
// Notice : overflow detection
InetAddress128 InetAddress128::operator-(const InetAddress128 addr2)
{
	char* str1=new char[500];
	sprintf(str1,"%08x%08x%08x%08x",address[0],address[1],address[2],address[3]);
	RossiBigInt arg1 (str1, HEX_DIGIT);
	char* str2=new char[500];
	sprintf(str2,"%08x%08x%08x%08x",addr2.address[0],addr2.address[1],addr2.address[2],addr2.address[3]);
	RossiBigInt arg2 (str2, HEX_DIGIT);
	RossiBigInt res=arg1-arg2;
	char* str_t=new char[500];
	sprintf(str_t,"%032s",res.getstr_pure_hex_value().c_str());
	InetAddress128 addr_t;
    sscanf(str_t,"%8x%8x%8x%8x",addr_t.address,addr_t.address+1,addr_t.address+2,addr_t.address+3);
	return addr_t;
}

// Notice : overflow detection
InetAddress128 InetAddress128::operator+(const InetAddress128 addr2)
{
	char* str1=new char[500];
	sprintf(str1,"%08x%08x%08x%08x",address[0],address[1],address[2],address[3]);
	RossiBigInt arg1 (str1, HEX_DIGIT);
	char* str2=new char[500];
	sprintf(str2,"%08x%08x%08x%08x",addr2.address[0],addr2.address[1],addr2.address[2],addr2.address[3]);
	RossiBigInt arg2 (str2, HEX_DIGIT);
	RossiBigInt res=arg1+arg2;
	char* str_t=new char[500];
	sprintf(str_t,"%032s",res.getstr_pure_hex_value().c_str());
	InetAddress128 addr_t;
    sscanf(str_t,"%8x%8x%8x%8x",addr_t.address,addr_t.address+1,addr_t.address+2,addr_t.address+3);
	return addr_t;
}

// No description
InetAddress128 InetAddress128::operator/(const unsigned int n)
{
	char* str1=new char[500];
	sprintf(str1,"%08x%08x%08x%08x",address[0],address[1],address[2],address[3]);
	RossiBigInt arg1 (str1, HEX_DIGIT);
	char* str2=new char[32];
	sprintf(str2,"%d",n);
	RossiBigInt arg2 (str2, DEC_DIGIT);
	RossiBigInt res=arg1/arg2;
	char* str_t=new char[500];
	sprintf(str_t,"%032s",res.getstr_pure_hex_value().c_str());
	InetAddress128 addr_t;
    sscanf(str_t,"%8x%8x%8x%8x",addr_t.address,addr_t.address+1,addr_t.address+2,addr_t.address+3);
	return addr_t;
}
*/

