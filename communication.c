#include "communication.h"


unsigned char* get_ip_addr_bytes_from_string(char* dotted_dec_ip){
  int values[4];
  int i;
  unsigned char* ip_bytes;
  ip_bytes = malloc(sizeof(char)*4);

  sscanf(dotted_dec_ip, "%d.%d.%d.%d",
  &values[3], &values[2], &values[1], &values[0]);

  for(i = 0; i < 4; i++){
    ip_bytes[i] = (unsigned char) values[i];
  }
  return ip_bytes;
}

void printBits(size_t const size, void const * const ptr){
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;

    for (i=size-1;i>=0;i--)
    {
        for (j=7;j>=0;j--)
        {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
    }
    puts("");
}

// int main(void){

// 	return 0;
// }
