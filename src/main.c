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
#define IP        127.0.0.1
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
	int  tcpportno;
	int  udpportno;
}node[20];

/* start : Hash Table functions */

/*  End  : Hash Table functions */

int main(int argc, char *argv){
  int master_socekt, udp_socket;
  struct sockaddr_in address, server_addr_udp, client_addr;
  int new_socket, maxfd, addrlen;
  char buffer[1025], replyBuffer[1024];
  fd_set readfds;
  int node_id = atoi(argv[1]);
  int opt = TRUE, transferValue;
  int key, value;

  for(int i=0; i<N; i++){
    node[i].ip_address = IP;    //prone to error
    node[i].tcpportno = 2000 + i*2;
    node[i].udpportno = node[i].tcpportno + 1;
  }

  printf ("       INSTRUCTIONS \n\n  ================= NODE %d =======================\n",node_id);
	puts("   1.'put' request format : PUT(<integer>,<integer>)\n");
	puts("   2.'get' request format : GET(<integer>)\n");
	puts("   3.To print Hash Table : 'p'\n");
	puts("-----------------------------------\n\nENTER GET/PUT REQUEST :");


  initializeHashTable();

  //create UDP socket
  udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
  if(udp_socket < 0){
    perror("UDP socket error");
    exit(EXIT_FAILURE);
  }

  //UDP socket attributes for current node
  server_addr_udp.sin_family = AF_INET;
  server_addr_udp.sin_addr.s_addr = INADDR_ANY;
  server_addr_udp.sin_port = htons(node[node_id.udpportno]);
  memset(&server_addr_udp, 0, sizeof(server_addr_udp));

  //bind UDP socket
  int bnd = bind(udp_socket, (sockaddr*)&server_addr_udp, sizeof(server_addr_udp));
  if(bnd < 0){
    perror("bind error");
    exit(EXIT_FAILURE);
  }

  //create master TCP socket
  master_socekt = socket(AF_INET, SOCK_STREAM, 0);
  if(master_socekt < 0){
    perror("Socket Error");
    exit(EXIT_FAILURE);
  }
  printf("master TCP socket is created\n");

  //set master socket to allow multiple connections
  if(setsockopt(master_socekt, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt))< 0){
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }

  //TCP socket attributes for current node
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(node[node_id].tcpportno);

  //bind TCP socket
  bnd = bind(master_socekt, (sockaddr*)&address, sizeof(address));
  if(bnd < 0){
    perror("bind error");
    exit(EXIT_FAILURE);
  }

  //listen TCP socket
  int lsn = listen(master_socekt, 20);
  if(lsn < 0){
    perror("listen error");
    exit(EXIT_FAILURE);
  }

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
      addrlen = sizeof(address);
      new_socket = accept(master_socekt, (struct sockaddr*)&address, &addrlen);
      if(new_socket < 0){
        perror("Accept");
        exit(EXIT_FAILURE);
      }
      if(transferValue != 0){                  // means put request was made on this node , and this node need to supply the value  //TODO
        itoa(transferValue, replyBuffer);
        send(new_socket, replyBuffer, strlen(replyBuffer), 0);
        read(new_socket, buffer, 1024);
        printf("\n%s\n", buffer);
        transferValue = 0;
        close(new_socket);
      }
      else{                 // means get request was made and tcp server is only need to recieve value from client
        int read_value = read(new_socket, buffer, 1024);
        if(read_value < 0){
          perror("read");
          exit(EXIT_FAILURE);
        }
        buffer[read_value] = '\0';
        printf("\n%s\n",buffer);
      }
      close(new_socket);
    }

    else if(FD_ISSET(udp_socket, &readfds)){
        // TODO: will be changed
    }

    else if(FD_ISSET(0, &readfds)){
      char rec_buffer[2048];
      gets(rec_buffer);
      if(rec_buffer[0] == 'r' || rec_buffer[0] == 'R'){
        DisplayTable();    // TODO : implement a function to display Hash table
      }
      else if(check_if_data_is_local(node_id, rec_buffer) == 0){ // TODO: implement the check function. It return 1 if data is for this node, return 0 if data is not for this node
        char outputbuff[40], *out, flag;
        int i = 0;
        if(check_cmd(rec_buffer) == 0){   // TODO: implement check_cmd function. It returns 1 for get, 0 for put.
          transferValue = extract_value_from_put(rec_buffer);
          flag = 'r';
        }
        else{
          flag = 's';
        }
        out = forwarded_data(rec_buffer, flag);    // TODO: implement forwarded_data function. It prepares the data which is to be sent.
        for(i=0; i < strlen(out); i++){
          outputbuff[i] = *(out+i);
        }
        outputbuff[i] = '\0';
        forward_UDP(num_id+1, outputbuff);  // TODO: implement forward_UDP function.
        free(out);
      }
      else{
        printf("Processing the request here\n\n");
        if(check_cmd(rec_buffer) == 1){
          key = extract_key_from_get_request(rec_buffer);
          value = fetch_value_from_hash_table(key);
          if(value == 0){
            printf("no value in the hash table for the key %d\n", key);
          }
          else{
            printf("Key = %d\t-->\t Value = %d\n", key, value);
          }
      }
        else{
          key = extract_key_from_buffer(rec_buffer);      //TODO: implement
          value = extract_value_from_buffer(rec_buffer);  //TODO: implement
          add_data_to_hash_table(key, value);                 //TODO: implement
        }
      }
      fflush(stdout);
    } // end of probing console
  } // end of while(1)
  return 0;
}
