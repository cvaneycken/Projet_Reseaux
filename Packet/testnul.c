#include "packet_interface.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <sys/ioctl.h>
#include <inttypes.h>


int check(uint32_t data){
  uint8_t header=(uint8_t)(data>>24);
  uint16_t length=(uint16_t)(data);

  printf("Length of: %d\n", length);
  //uint8_t tr_test=0b00000001;
  //printf("%d\n",header&tr_test);
  //uint8_t win_test=0b00011111;

uint8_t headertype=header>>6;
uint8_t headertr=(header&0b00100000)>>5;
uint8_t seqnum=(header>>16)&0b00000000000000000000000011111111;
uint8_t headerwin=header&0b00011111;;
if(headertype==(uint8_t)1){
  printf("It's a DATA %d\n", headertype);
}

if(headertype==(uint8_t)2){
  printf("It's an ACK %d\n", headertype);
}

if(headertype==(uint8_t)3){
  printf("It's a NACK %d\n", headertype);
}
if(headertype!=(uint8_t)1&&headertype!=(uint8_t)2&&headertype!=(uint8_t)3){
  printf("TYPE unknown\n");
}
if(headertr==1){
  printf("Truncated\n");
}
else{
  printf("Not truncated %d\n",headertr);
}
printf("Window of size:%d\n", headerwin);
printf("seqnum:%d\n", seqnum);
return 0;
}
int main(int argc, char const *argv[]){
    //uint8_t taille=(uint8_t)15;
    //uint8_t header=0b10100000;
    //header=header|taille;
    uint32_t data=0b01101101000001110000010100111111;
    check(data);
    uint32_t type=(uint32_t)1<<30;
    uint32_t tr_h=(uint32_t)1<<29;
    uint32_t window=(uint32_t)13<<24;
    uint32_t seqnum=(uint32_t)15<<16;
    uint32_t length=(uint32_t)1343;
    uint32_t header= type|tr_h|window|seqnum|length;
    check(header);
return 0;
}
