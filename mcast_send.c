#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GROUP_ADDR "239.255.1.1"
#define LOCAL_ADDR "192.168.4.21"
#define PORT 5500
#define MESSAGE "hello"

int main (int argc, char *argv[])
{
  struct in_addr        localInterface;
  struct sockaddr_in    groupSock;
  int                   sendfd;
  int                   datalen;
  char                  databuf[32];

  sendfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sendfd < 0) {
    perror("opening datagram socket");
    return 0;
  }

  memset((char *) &groupSock, 0, sizeof(groupSock));
  groupSock.sin_family = AF_INET;
  groupSock.sin_addr.s_addr = inet_addr(GROUP_ADDR);
  groupSock.sin_port = htons(PORT);

 
    char loopch=0;

    if (setsockopt(sendfd, IPPROTO_IP, IP_MULTICAST_LOOP, (char *)&loopch, sizeof(loopch)) < 0) {
      perror("setting IP_MULTICAST_LOOP:");
      close(sendfd);
      return 0;
    }

 
  localInterface.s_addr = inet_addr(LOCAL_ADDR);
  if (setsockopt(sendfd, IPPROTO_IP, IP_MULTICAST_IF, (char *)&localInterface, sizeof(localInterface)) < 0) {
    perror("setting local interface");
    close(sendfd);
    return 0;
  }


  
  
  strcpy(databuf, MESSAGE);
  datalen = strlen(databuf);
  if (sendto(sendfd, databuf, datalen, 0, (struct sockaddr*)&groupSock, sizeof(groupSock)) < 0)
  {
    perror("sending datagram message");
  }
  
  close(sendfd);
  return 0;
}