
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>

struct record{
  uint32_t type: 15;
  uint32_t f: 1;
  uint32_t length: 16;
  char *payload;
  uint32_t footer;
};
/**
 * Initialise un enregistrement
 * @pre: r!= NULL
 * @post: record_get_type(r) == 0 && record_get_length(r) == 0
 *           && record_has_footer(r) == 0
 * @return: 1 en cas d'erreur, 0 sinon
 */
int record_init(struct record *r){
  if(!r)return 1;
  r->type=0;
  r->length=0;
  r->footer=0;
  return 0;
}
/**
 * Libère les ressources consommées par un enregistrement
 * @pre: r!= NULL
 */
 void record_free(struct record *r){
   free(r->payload);
   free(r);
 }
 /**
 * Renvoie le type d'un enregistrement
 * @pre: r != NULL
 */
int record_get_type(const struct record *r){
  return (int)(r->type);
}

/**
 * Définit le type d'un enregistrement
 * @pre: r != NULL
 * @post: record_get_type(r) == (type & 0x7FFF)
 */
void record_set_type(struct record *r, int type){
  r->type=(uint32_t)(type);
}

/**
 * Renvoie la taille du payload de l'enregistrement (dans l'endianness native de la machine!)
 * @pre: r!= NULL
 */
int record_get_length(const struct record *r){
  return (int)(r->length);
}

/**
 * Définit le payload de l'enregistrement, en copiant n octets
 * du buffer. Si le buffer est NULL (ou de taille 0), supprime
 * le payload
 * @pre: r != NULL && buf != NULL && n > 0
 * @post: record_get_length(r) == (n & 0xFFFF)
             && record_get_payload(<buf2>, n) == (n & 0xFFFF)
 *        && memcmp(buf, <buf2>, (n & 0xFFFF)) == 0
 * @return: -1 en cas d'erreur, 0 sinon
 */
int record_set_payload(struct record *r,const char * buf, int n){
  r->length=(uint16_t)n;
  r->payload=(char *)malloc(n);
  if(r->payload==NULL)return -1;
  strncpy(r->payload,buf,n);
  return 0;
}

/**
 * Copie jusqu'à n octets du payload dans un buffer
 * pré-alloué de taille n
 * @pre: r != NULL && buf != NULL && n > 0
 * @return: n', le nombre d'octets copiés dans le buffer
 * @post: n' <= n && n' <= record_get_length(r)
 */
int record_get_payload(const struct record *r,char *buf, int n){
  if(r->payload==NULL) return -1;
  strncpy(buf,r->payload,n);
  return record_get_length(r);
}

/**
 * Teste si l'enregistrement possède un footer
 * @pre: r != NULL
 * @return: 1 si l'enregistrement a un footer, 0 sinon
 */
int record_has_footer(const struct record *r){
  if(r->footer==0){
    return 0;
  }
  return 1;
}

/**
 * Supprime le footer d'un enregistrement
 * @pre: r != NULL
 * @post: record_has_footer(r) == 0
 */
void record_delete_footer(struct record *r){
  r->footer=0;
}

/**
 * Définit l'uuid d'un enregistrement
 * @pre: r != NULL
 * @post: record_has_footer(r) &&
          record_get_uuid(r, &<uuid2>) => uuid2 == uuid
 */
void record_set_uuid(struct record *r, unsigned int uuid){
  r->footer=uuid;
}

/**
 * Extrait l'uuid d'un enregistrement
 * @pre: r != NULL
 * @post: (record_has_footer(r) && uuid != 0) ||
 *        (!record_has_footer(r) && uuid == 0)
 */
unsigned int record_get_uuid(const struct record *r){
  return r->footer;
}
/**
 * Ecrit un enregistrement dans un fichier
 * @pre: r != NULL && f != NULL
 * @return: n', le nombre d'octets écrits dans le fichier.
           -1 en cas d'erreur
           Séparation en 3 partie pour l'écriture par
           facilité ("*","&",...) et a cause des différentes
           tailles des composantes de la structure
 */
 int record_write(const struct record *r, FILE *f){
   int nbytes_written=0;
   //Creation de myRec a imprimer
   struct record myRec;
   myRec.type=r->type;
   myRec.f=record_has_footer(r);
   myRec.length=htons(r->length);
   //Ecriture du Header
   int err= fwrite(&myRec,sizeof(uint32_t),1,f);
   if(err!=1){
     fprintf(stderr, "Err:fwrite Header\n");
     return -1;
   }
   nbytes_written=err*sizeof(uint32_t);
   //Ecriture du payload
   if(record_get_length(r)>0){
     err=fwrite(r->payload,1,record_get_length(r),f);
     if(err!=(int)record_get_length(r)){
       fprintf(stderr, "Err:fwrite payload (err!=(int)record_get_length(r))n");
       return -1;
     }
     nbytes_written=nbytes_written+err;
   }
   //Ecriture du footer
   if(record_has_footer(r)){
     int footer=record_get_uuid(r);
     err=fwrite(&footer,sizeof(uint32_t),1,f);
     if(err!=1){
       fprintf(stderr, "Err:Err: fwrite footer (err==-1)\n");
       return -1;
     }
     nbytes_written=nbytes_written+err*sizeof(uint32_t);
  }
   return nbytes_written;

 }
 /**
  * Lit un enregistrement depuis un fichier
  * @pre: r != NULL && f != NULL
  * @return: n', le nombre d'octets luts dans le fichier.
            -1 en cas d'erreur
  */
  int record_read(struct record *r, FILE *f){
      int nbytes_read=0;
      //Lecture du header
      int err=fread(r,sizeof(uint32_t),1,f);
      if(err!=1){
        fprintf(stderr, "Err:reading header (err!=1)\n");
        return -1;
      }
      nbytes_read=nbytes_read+sizeof(uint32_t);
      /* htonl, htons, ntohl, ntohs
      - convert values between host and network byte order
      On the i386 the host byte order is Least Significant
       Byte first, whereas the network byte order, as used
       on the Internet, is Most Significant Byte first.
      */
      r->length=ntohs(r->length);
      int len=record_get_length(r);
      //Lecture du payload
      if(len>0){
        //creation du buffer pour utiliser record_set_payload
        char *buff=(char *)malloc(len*sizeof(char));
        if(buff==NULL){
          fprintf(stderr, "Err:buf==NULL\n");
          return -1;
        }
        err= fread(buff,sizeof(char),len,f);
        if(err!=len){
          fprintf(stderr, "Err: fread payload err!=len\n");
          return -1;
        }
        //tester
        if(record_set_payload(r,buff,len)==-1){
          fprintf(stderr, "Err: set_payload returned -1\n");
          return -1;
        }
        nbytes_read=nbytes_read+err;
      }
      //Lecture du footer
      if(record_has_footer(r)){
        err=fread(&(r->footer),sizeof(uint32_t),1,f);
        if(err!=1){
          fprintf(stderr, "Err: reading footer err!=1\n");
          return -1;
        }
        nbytes_read=nbytes_read+err*sizeof(uint32_t);
      }
      return nbytes_read;
  }
int main(int argc, char const *argv[]) {
  return 0;
}
