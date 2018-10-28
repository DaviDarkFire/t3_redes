#ifndef __DEFINES__
  #define __DEFINES__
  #include <stdio.h>
  #include <sys/ioctl.h>
  #include <arpa/inet.h>
  #include <net/if.h>
  #include <string.h>
  #include <stdlib.h>
  #include <stddef.h>             /* offsetof */
  #include <linux/sockios.h>
  #include <netinet/in.h>
  #if __GLIBC__ >=2 && __GLIBC_MINOR >= 1
  #include <netpacket/packet.h>
  #include <net/ethernet.h>
  #else
  #include <asm/types.h>
  #include <linux/if_ether.h>
  #endif

  #define ifreq_offsetof(x)  offsetof(struct ifreq, x)

  #define DEFAULT_MODE 0
  #define CONFIG_IP_MODE 1
  #define SET_MTU_MODE 2
#endif
