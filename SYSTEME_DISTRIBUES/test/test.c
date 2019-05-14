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
      struct ifaddrs *addrs, *tmp;
      getifaddrs(&addrs);
      tmp=addrs;
      struct sockaddr_in *pAddr;

      while(tmp)
      {
        if(tmp->ifa_addr && tmp->ifa_addr->sa_family== AF_INET)
        {
          pAddr=(struct sockaddr_in *)tmp->ifa_addr;
        }
        tmp=tmp->ifa_next;
      }
    }

    int main(int argc, char *argv[])
    {
      char* IP;
	    IP=getIP();
      printf("\n\t IP : %s", IP);
    }
