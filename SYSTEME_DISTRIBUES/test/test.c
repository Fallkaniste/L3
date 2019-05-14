    #define _GNU_SOURCE /* To get defns of NI_MAXSERV and NI_MAXHOST */
    #include <arpa/inet.h>
    #include <sys/socket.h>
    #include <netdb.h>
    #include <ifaddrs.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <unistd.h>
    #include <linux/if_link.h>

    char* getIP()
    {
      struct sockaddr_in *pAddr;
      struct ifaddrs *addrs;

      getifaddrs(&addrs);

      while(addrs)
      {
        if(addrs->ifa_addr && addrs->ifa_addr->sa_family== AF_INET)
        {
          pAddr=(struct sockaddr_in *)addrs->ifa_addr;
        }
        addrs=addrs->ifa_next;
      }

      return inet_ntoa(pAddr->sin_addr);
    }

    int main(int argc, char *argv[])
    {
      char* IP;
	    IP=getIP();
      printf("\n\t IP : %s", IP);
    }
