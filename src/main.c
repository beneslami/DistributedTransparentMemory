#include <stdio.h>
#include <errno.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define TRUE      1
#define FALSE     0
#define N         6
#define IP1       127.0.0.1
#define IP2       127.0.0.1
#define IP3       127.0.0.1
#define IP4       127.0.0.1
#define IP5       127.0.0.1
#define IP6       127.0.0.1
#define TABLESIZE 100

struct node{
  int data;
  struct node *link;
};

struct HashTableEntry{
  int data;
  struct node *link;
};

struct HashTableEntry table[TABLESIZE];
int num = 0;

struct machine_id
{
	char *ip_address;
	int tcpportno;
	int udpportno;
}node[20];

/* start : Hash Table functions */

/*  End  : Hash Table functions */

int main(void){
  int master_socekt, udp_socket;
  struct sockaddr_in address, server_addr_udp, client_addr;
  int new_socket, maxfd;
  char buffer[1025], replyBuffer[1024];
  fd_set readfds;

  for(int i=0; i<N; i++){
    node[i].ip_address = "127.0.0.1";
    node[i].tcpportno = 2000 + i*2;
    node[i].udpportno = node[i].tcpportno + 1;
  }
  printf ("       INSTRUCTIONS \n\n  ================= NODE %d =======================\n",num);
	puts("   1.'put' request format : PUT(<integer>,<integer>)\n");
	puts("   2.'get' request format : GET(<integer>)\n");
	puts("   3.To print Hash Table : 'r'\n");
	puts("-----------------------------------\n\nENTER GET/PUT REQUEST :");


  initializeHashTable();

  //create UDP socket

  //UDP socket attributes for current node

  //bind UDP socket

  //create master TCP socket

  //set master socket to allow multiple connections

  //TCP socket attributes for current node

  //bind TCP socket

  //listen TCP socket

  while(1){
    FD_ZERO(&readfds);
    FD_SET(udp_socket, &readfds);     //UDP socket
    FD_SET(master_socekt, &readfds);  //TCP socket
    FD_SET(0, &readfds);              // Console
    if(master_socekt > udp_socket)
      maxfd = master_socekt;
    else
      maxfd = udp_socket;
    select(maxfd+1, &readfds, NULL, NULL, NULL);

    if(FD_ISSET(master_socekt, &readfds)){

    }
    else if(FD_ISSET(udp_socket, &readfds)){

    }
    else if(FD_ISSET(0, &readfds)){

    }
  }
  return 0;
}
