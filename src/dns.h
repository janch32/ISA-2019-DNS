#include <arpa/inet.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define OPCODE_QUERY	0
#define OPCODE_IQUERY	1
#define OPCODE_STATUS	2

#define RCODE_SUCCESS			0
#define RCODE_FORMAT_ERROR		1
#define RCODE_SERVER_FAILTURE	2
#define RCODE_NAME_ERROR		3
#define RCODE_NOT_IMPLMENTED	4
#define RCODE_REFUSED			5

#define TYPE_A		1
#define TYPE_NS		2
#define TYPE_CNAME	5
#define TYPE_MX		15
#define TYPE_TXT	16

#define CLASS_IN 1
#define CLASS_CS 2
#define CLASS_CH 3
#define CLASS_HS 4

/**
 * 
 * @see https://tools.ietf.org/html/rfc1035 (pages 25-28)
 */
typedef struct DnsMessage{
	/** Request ID */
	uint16_t id;
	/** 0 = dotaz, 1 = odpověď */
	uint8_t qr : 1;
	/** Specifikace typu požadavku (0=standard, 1=inverse, 2=server status) */
	uint8_t opcode : 4;
	/** Authoritative answer */
	uint8_t aa : 1;
	/** Truncation */
	uint8_t tc : 1;
	/** Recursion desired */
	uint8_t rd : 1;
	/** Recursion available */
	uint8_t ra : 1;
	/** Reserved (zerofill) */
	uint8_t z : 3;
	/** Response code */
	uint8_t rcode : 4;
	/** Počet záznamů v dotazové sekci */
	uint16_t qdcount;
	/** Počet záznamů v sekci odpovědí */
	uint16_t ancount;
	/** Počet záznamů v sekci autoritativních serverů */
	uint16_t nscount;
	/** Počet záznamů v sekci ostatních dat */
	uint16_t arcount;

	struct DnsMessageQuestion *question;
	struct DnsMessageResource *answer;
	struct DnsMessageResource *authority;
	struct DnsMessageResource *additional;
} *pDnsMessage;


/**
 * 
 * @see https://tools.ietf.org/html/rfc1035 (section 4.1.2, pages 28-29)
 */
typedef struct DnsMessageQuestion{
	uint8_t *qname;
	uint16_t qtype;
	uint16_t qclass;
} *pDnsMessageQuestion;


/**
 * 
 * @see https://tools.ietf.org/html/rfc1035 (section 4.1.3, pages 29-30)
 */
typedef struct DnsMessageResource{
	uint8_t *name;
	uint16_t type;
	uint16_t class;
	uint32_t ttl;
	uint16_t rdlength;
	uint8_t *rdata;
} *pDnsMessageResource;

uint16_t generateId();

void createRequest(pDnsMessage message, uint8_t *host, bool recursion, uint16_t type)
