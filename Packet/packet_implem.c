#include "packet_interface.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>
/* Extra #includes */
/* Your code will be inserted here */

struct __attribute__((__packed__)) pkt {
ptypes_t type;
uint8_t trFlag:1;
uint8_t window:5;
uint8_t seqNum: 8;
uint16_t length;
uint32_t timeStamp;
uint32_t crc1;
char *payload;
uint32_t crc2;
};

/* Extra code */
/* Your code will be inserted here */

pkt_t* pkt_new()
{
	return calloc(1,sizeof(pkt_t));
}

void pkt_del(pkt_t *pkt)
{
    free(pkt);
}

pkt_status_code pkt_decode(const char *data, const size_t len, pkt_t *pkt)
{
  if(memcpy((void *)pkt,(const void *)data,(size_t)len)==NULL)return E_NOMEM;
  pkt_set_length(pkt,ntohs(pkt_get_length((const pkt_t *)pkt)));
  pkt_set_crc1(pkt,ntohs(pkt_get_crc1((const pkt_t *)pkt)));
  pkt_set_crc2(pkt,ntohs(pkt_get_crc2((const pkt_t *)pkt)));
  return PKT_OK;
}

pkt_status_code pkt_encode(const pkt_t* pkt, char *buf, size_t *len)
{
	pkt_set_length((pkt_t*)pkt,htons(pkt_get_length((const pkt_t *)pkt)));
	pkt_set_crc1((pkt_t *)pkt,htons(pkt_get_crc1((const pkt_t *)pkt)));
	pkt_set_crc2((pkt_t *)pkt,htons(pkt_get_crc2((const pkt_t *)pkt)));
  if(memcpy((void *)pkt,(const void*)buf,(size_t)len)==NULL){
    fprintf(stderr, "Error: pkt_encode fwrite, %s\n",strerror(errno));
    return E_UNCONSISTENT;
  }
	pkt_set_length((pkt_t *)pkt,ntohs(pkt_get_length((const pkt_t *)pkt)));
  pkt_set_crc1((pkt_t *)pkt,ntohs(pkt_get_crc1((const pkt_t *)pkt)));
  pkt_set_crc2((pkt_t *)pkt,ntohs(pkt_get_crc2((const pkt_t *)pkt)));
  return PKT_OK;
}

ptypes_t pkt_get_type(const pkt_t* pkt)
{
  return pkt->type;
}

uint8_t  pkt_get_tr(const pkt_t* pkt)
{
	return pkt->trFlag;
}

uint8_t  pkt_get_window(const pkt_t* pkt)
{
	return pkt->window;
}

uint8_t  pkt_get_seqnum(const pkt_t* pkt)
{
	return pkt->seqNum;
}

uint16_t pkt_get_length(const pkt_t* pkt)
{
	return pkt->length;
}

uint32_t pkt_get_timestamp(const pkt_t* pkt)
{
	return pkt->timeStamp;
}

uint32_t pkt_get_crc1(const pkt_t* pkt)
{
	return pkt->crc1;
}

uint32_t pkt_get_crc2(const pkt_t* pkt)
{
	return pkt->crc2;
}

const char* pkt_get_payload(const pkt_t* pkt)
{
	return (const char*)pkt->payload;
}


pkt_status_code pkt_set_type(pkt_t *pkt, const ptypes_t type)
{
	pkt->type=type;
  return PKT_OK;
}

pkt_status_code pkt_set_tr(pkt_t *pkt, const uint8_t tr)
{
	pkt->trFlag=tr;
  return PKT_OK;
}

pkt_status_code pkt_set_window(pkt_t *pkt, const uint8_t window)
{
	pkt->window=window;
  return PKT_OK;
}

pkt_status_code pkt_set_seqnum(pkt_t *pkt, const uint8_t seqnum)
{
	pkt->seqNum=seqnum;
  return PKT_OK;
}

pkt_status_code pkt_set_length(pkt_t *pkt, const uint16_t length)
{
	pkt->length=length;
  return PKT_OK;
}

pkt_status_code pkt_set_timestamp(pkt_t *pkt, const uint32_t timestamp)
{
  pkt->timeStamp=timestamp;
  return PKT_OK;
}

pkt_status_code pkt_set_crc1(pkt_t *pkt, const uint32_t crc1)
{
	pkt->crc1=crc1;
  return PKT_OK;
}

pkt_status_code pkt_set_crc2(pkt_t *pkt, const uint32_t crc2)
{
	pkt->crc2=crc2;
  return PKT_OK;
}

pkt_status_code pkt_set_payload(pkt_t *pkt,const char *data,const uint16_t length)
{
	if(strncpy(pkt->payload,data,length)==NULL)return  E_NOMEM;
  pkt_set_length(pkt,length);
  return PKT_OK;
}
 int main(int argc, char const *argv[]) {
 	pkt_t *myPkt=pkt_new();
	char *buf=(char *)malloc(sizeof(myPkt));
 	return 0;
 }
