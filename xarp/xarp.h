#ifndef __XARP__
  #define __XARP__

  #include <stdio.h>
  #include <string.h>
  #include <stdlib.h>

  #define XARP_SHOW 0
  #define XARP_RES 1
  #define XARP_ADD 2
  #define XARP_DEL 3
  #define XARP_TTL 4
  #define XIFCONFIG_ETHERNET_INFO 5

  char* build_xarp_show_message();
  char* build_xarp_res_message(char** args);
  char* build_xarp_add_message(char** args);
#endif
