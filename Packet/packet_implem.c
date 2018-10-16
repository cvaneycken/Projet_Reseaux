#include "packet_interface.h"

/* Extra #includes */
/* Your code will be inserted here */

struct __attribute__((__packed__)) pkt {
//Header begins
uint8_t type:2;
uint8_t trFlag:1;
uint8_t window:5;
uint8_t seqNum: 8;
uint16_t length;
//Header ends -> 32 Bytes
uint32_t timeStamp;
uint32_t crc1;
char *payload;
uint32_t crc2;
};

/* Extra code */
//Displays adequat error
pkt_status_code dispErr(pkt_status_code a){
	pkt_status_code c;
	switch (c=a) {
		case PKT_OK:
			return PKT_OK;
			break;
		case E_TYPE:
			fprintf(stderr, "Erreur liee au champs Type\n");
			return c;
			break;
		case E_TR:
			fprintf(stderr, "Erreur liee au champ TR\n");
			return c;
			break;
		case E_LENGTH:
			fprintf(stderr, "Erreur liee au champ Length\n");
			return c;
			break;
		case E_CRC:
			fprintf(stderr, "CRC invalide\n");
			return c;
			break;
		case E_WINDOW:
			fprintf(stderr, "Erreur liee au champ Window\n");
			return c;
			break;
		case E_SEQNUM:
			fprintf(stderr, "Numero de sequence invalide\n");
			return c;
			break;
		case E_NOMEM:
			fprintf(stderr, "Pas assez de memoire\n");
			return c;
			break;
		case E_NOHEADER:
			fprintf(stderr, "Le paquet n'a pas de header (trop court)\n");
			return c;
			break;
		case E_UNCONSISTENT:
			fprintf(stderr, "Le paquet est incoherent\n");
			return c;
			break;
	}
	return PKT_OK;
}
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
	//header is 32 Bytes long
	char *header=malloc(sizeof(uint32_t));
	if(header==NULL) return dispErr(E_NOMEM);
	if(len<sizeof(uint32_t)) return dispErr(E_NOHEADER);
	memcpy((void *)header,(const void *)data,sizeof(uint32_t));
	//Writing TYPE which is defined by 2 1st bits
	const ptypes_t type=(*header)>>6;
	if(type!=1&type!=2&type!=3){
		return dispErr(E_TYPE);
	}
	pkt_set_type(pkt,type);
	//Writing the Tr bit
	uint8_t h=((*header)>>5)&0b00000001;
	pkt_set_tr(pkt,h);
	//Writing  the window
	h=(*header)&0b00011111;
	pkt_set_window(pkt,h);
	//Writing the seqNum which is from 9th to 19h bit so header+1=After 1 byte
	h=*(header+1);
	pkt_set_seqnum(pkt,h);
	//Writing length
	uint16_t length;
	memcpy(&length,(header+2),sizeof(uint16_t));
	if(length>(uint16_t)MAX_PAYLOAD_SIZE) return dispErr(E_LENGTH);
	pkt_set_length(pkt,ntohs(length));
	//if type!= data,(it's ack or nack) it can't have any payload =>length==0?
	if(type!=1&&pkt_get_length(pkt)!=0){
		fprintf(stderr, "pkt of type:%d and length%d\n",pkt_get_type(pkt),pkt_get_length(pkt));
		return dispErr(E_UNCONSISTENT);
	}
	// Writing timestamp
	uint32_t ts;
	memcpy(&ts,(data+4),sizeof(uint32_t));
	pkt_set_timestamp(pkt,ts);
	//Writing CRC1
	uint32_t crc1;
	memcpy(&crc1,(data+8),sizeof(uint32_t));
	crc1=ntohl(crc1);
	if(pkt_get_tr(pkt)==0&&crc1!=(uint32_t)crc32(0,(Bytef *)data,(uInt)(2*sizeof(uint32_t)))){
		fprintf(stderr, "Err: decode, crc1 not matching\n");
		pkt_del(pkt);
		return dispErr(E_CRC);
	}
	pkt_set_crc1(pkt,crc1);
	//Writing payload
	char *payload=malloc(sizeof(length));
	if(payload==NULL)return dispErr(E_NOMEM);
	memcpy(payload,(data+12),length);
	pkt_set_payload(pkt,payload,length);
	//Writing CRC2
	uint32_t crc2;
	memcpy(&crc2,(data+12+length),sizeof(uint32_t));
	crc2=ntohl(crc2);
	if(crc2!=(uint32_t)crc32(0,(Bytef *)payload,(uInt)length)){
		fprintf(stderr, "Err: decode, crc2 not matching\n");
		pkt_del(pkt);
		return dispErr(E_CRC);
	}
	return PKT_OK;
}

pkt_status_code pkt_encode(const pkt_t* pkt, char *buf, size_t *len)
{
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
 	return 0;
 }
