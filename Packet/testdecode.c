#include "packet_implem.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <sys/ioctl.h>
#include <inttypes.h>

int main()
{
  //ACK
  uint64_t type = 1<<62;
  uint64_t tr = 0<<61; //0 pour ACK
  uint64_t window = 30<<56; //[0-31]
  uint64_t seqnum = 200<<48; //[0-255]
  uint64_t length = 0; //0 pour ACK
  uint64_t timestamp = 300; // sur 4 Bytes
  //Is timestamp part of the header?
  uint64_t ack1_pre = type|tr|window|seqnum|length|seqnum;
  //uint64_t crc1 = (uint64_t)crc32(0,(Bytef *)ack1_pre);
  uint64_t ack1 = ack1_pre&crc1;
  char * buf = &((char)ack1);
  pkt_t pac = pkt_new();
  size_t size_ack = 8;
  pkt_decode(buf,size_ack,pac);
  return 0;
}
