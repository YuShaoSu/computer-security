#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <linux/udp.h>
#include <linux/ip.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <string>
#include <sstream>
#include <unistd.h>
//#include <asm/types.h>

struct dnshdr {
	__u16 id;
	__u16 flag;
	__u16 qs_c;
	__u16 ans_c;
	__u16 ah_count;
	__u16 ar_count;
};

struct dnsqry {
	__u16 t;
	__u16 c;
};

// EDNS part
struct edns {
	__u16 edns_type;
	__u16 edns_class;
	__u16 edns_ttl_up;
	__u16 edns_ttl_low;
	__u16 edns_rdlen;
};
