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
return 0;
}
int main(int argc, char const *argv[]){
    //uint8_t taille=(uint8_t)15;
    //uint8_t header=0b10100000;
    //header=header|taille;
    uint32_t data[6]={0b01101101000001110000010100111111,0b11011001000010100000000000000000,0b11010010101010110000000101011111,0b11011101010111110000000000000000,0b0001111111110000000010111010,0b11010011011111000000000100100000};
    for(int i=0;i<6;i++){
      printf("Number %d\n",i);
      check(data[i]);
    }
    printf("%d\n",(int)sizeof(uint32_t));
    char *truc=malloc(sizeof(char)*3);
    truc="okla";
    printf("%c\n",*(truc+1));
return 0;
}
