#ifndef __XARP__
  #define __XARP__

  #include <stdio.h>
  #include <string.h>
  #include <stdlib.h>
  #include "../communication.h"

  #define XARP_SHOW 0
  #define XARP_RES 1
  #define XARP_ADD 2
  #define XARP_DEL 3
  #define XARP_TTL 4
  #define XIFCONFIG_INFO 5


  char* build_xarp_show_message();
  char* build_xarp_res_message(char** args);
  char* build_xarp_add_message(char** args);
  char* build_xarp_del_message(char** args);
  char* build_xarp_ttl_message(char* ttl);
  char* get_mac_addr_bytes_from_string(char* colon_format_mac);
  char* get_ttl_bytes_from_string(char* str_ttl);
#endif
