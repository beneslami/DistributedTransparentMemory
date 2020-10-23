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
void
initializeHashTable(void){
    for(int i=0; i< TABLESIZE; i++){
      table[i].data = 0;
      table[i].link = NULL;
    }
}

int
fetch_value_from_hash_table(int key, int num_id){
    int relative_index = (key - num_id)/N;
    int return_value;
    if(table[relative_index].link == NULL){
      return_value = table[relative_index].data;
    }
    else{
      struct node *start = table[relative_index].link;
      while(start->link != NULL){
        start = start->next;
      }
      return_value = start->data;
    }
    return return_value;
}

void
DisplayTable(int num_id){
  int from_key = num_id;
  int to_key = (TABLESIZE-1)*N + num_id;
  if(from_key > to_key){
    int temp = from_key;
    from_key = to_key;
    to_key = temp;
  }
  printf("\n-----------Hash table contents-----------\n");
  if(from_key % N == num_id && to_key % N == num){
    int i;
    int from = (from_key - num_id)/N;
    int to = (to_key - num_id)/N;
    int fetched_value, key;
    for(int i=from, i<=to; i++){
      key = i*N + num_id;
      fetched_value = fetch_value_from_hash_table(key, num_id);
      if(fetched_value != 0){
        printf("\n %d ====== %d\n", key, fetched_value);
      }
    }
  }
  else{
    printf("\n Invalid keys, hash table cannot be displayed\n");
  }
  printf("-----------------------------------\n\nENTER GET/PUT REQUEST :");
}

void
appendNode(struct node* start, int data){
  while(start->link != NULL){
    start = start->link;
  }
  start->link = (struct node*)malloc(sizeof(struct node));
  start = start->link;
  start->link = NULL;
  start->data = data;

}

void
add_data_to_hash_table(int key, int value, int num_id){
  int maxlimit_key = (TABLESIZE-1)*N + num_id;
  int relative_index, return_value;
  if(key%N == num_id && key <= maxlimit_key && key > -1){
    relative_index = (key - num_id)/N;
    if(table[relative_index].data == 0){
      table[relative_index].data = data;
      return_value = 1;
    }
    else if(table[relative_index].data != 0 && table[relative_index].link == NULL){
      table[relative_index].link = (struct node *)malloc(sizeof(struct node));
      table[relative_index].link->data = data;
      table[relative_index].link->link = NULL;
      return_value =1;
    }
    else{
      appendNode(table[relative_index].link, data);
      return_value = 1;
    }
    printf("\nRESULT: At key : %d, Value %d is inserted\n", key, value);
    puts("-----------------------------------\n\nENTER GET/PUT REQUEST :");
  }
  else{
    printf("\nERROR: Key %d Value %d cannot inserted\n", key, value);
    return_value = 0;
  }
}
/*  End  : Hash Table functions */

/* Begin : String handling functions */
char*
extract_IP_address(char *buff, char a, char b){
  char *ptr = (char*)malloc(20);
  int i = 0;
  int j = 0;
  while(buff[i] != a){
    i++;
  }
  i++;
  while(buff[i] != b){
    ptr[j++] = buff[i];
    i++;
  }
  ptr[j] = '\0';
  return ptr;
}

void
itoa(int n, char buff[]){
  int i = 0;
  int j;
  if(n == 0){
    buff[0] = '0';
    buff[1] = '\0';
  }
  while(n > 0){
    int rem = n %10;
    n = n/10;
    buff[i] = rem + 48;
    i++;
  }
  buff[i] = '\0';
  for(j = 0; j < i/2; j++){
    char temp = buff[j];
    buff[j] = buff[i - j - 1];
    buff[i - j - 1] = temp;
  }
}

int
extract_value_from_put(char request[]){
  char *b = strstr(request, ",");
  char *c = strstr(request, ")");
  int position = b - request;
  int position1 = c - request;
  int k;
  char to[16];
  memset(to, 0, 16);
  strncpy(to, request + position1, position1 - position);
  k = atoi(to);
  return k;
}

int
extract_key_from_put(char request[]){
  char *b = strstr(request, "(");
  char *c = strstr(request, ",");
  int position = b - request;
  int position1 = c - request;
  int k;
  char to[4];
  strncpy(to, request + position+1, position1 - position);
  k = atoi(to);
  fflush(stdout);
  return k;
}

int
extract_key_from_get_request(char request[]){
  char *b = strstr(request, "(");
  char *c = strstr(request, ")");
  int position = b - request;
  int position1 = c - request;
  int k;
  char to[4];
  strncpy(to, request + position+1, position1 - position);
  k = atoi(to);
  fflush(stdout);
  return k;
}

int
check_cmd(char request[]){
  if(request[0] =='g' || request[0] == 'G')
    return 1;
  else
    return 0;
}

int
extract_node_ID(char request[]){
  char *node = extract_IP_address(buff, ')', '[');
  return atoi(node);
}

int
check_if_data_is_local(int node_id, char request[]){
  int k;
  if(check_cmd(request) == 1){
    k = extract_key_from_get_request(request);
  }
  else{
    k = extract_key_from_put(requst);
  }
  if(k % N) == num_id)
    return 1;
  else
    return 0;
}

/* End : String handling functions */

int
main(int argc, char *argv){
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
  addr_len = sizeof(struct sockaddr);

  //create master TCP socket
  master_socekt = socket(AF_INET, SOCK_STREAM, 0);
  if(master_socekt < 0){
    perror("Socket Error");add_data_to_hash_table(key, value, num_id);
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
      if(transferValue != 0){
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
      char rec_buff[1024];
      int len = recvfrom(udp_socket, rec_buff, 1024, 0, (struct sockaddr*)&client_addr, &addr_len);
      rec_buff[len] = '\0';
      printf(".... UDP packet is received from IP-ADDRESS: %s, PORT: %d, Node_ID: %d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), extract_node_ID(rec_buff));
      printf("%s\n",rec_buff);
      if(check_if_data_is_local(node_id, rec_buff) == 0){
        forward_UDP(node_id+1, rec_buff);
        puts("\n---Enter GET/PUT request----\n\n");
      }
      else{
        printf("Processing the request in this node\n\n");
        key = extract_key_from_put(rec_buff);
        int node_no = extract_node_ID(rec_buff);

        int sock, bytes_received;
        char send_data[1024], recv_data[1024];
        char flag = rec_buff[strlen(rec_buff)-2];
        struct hostent *host;
        struct sockaddr_in server_addr;
        host = gethostbyname(extract_IP_address(rec_buff, '[',','));
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if(sock == -1){
          perror("socket");
          exit(EXIT_FAILURE);
        }
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(extract_value_from_put(rec_buff));
        server_addr.sin_addr = *((struct in_addr)host->h_addr);
        bzero(&(server_addr.sin_zero), 0);
        if(connect(sock, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)) == -1){
          perror("connect");
          exit(EXIT_FAILURE);
        }
        if(flag == 's'){
          int fetched_value = fetch_value_from_hash_table(key, num_id);
          if(fetched_value != 0){add_data_to_hash_table
            sprintf(send_data, "Value = %d. The value is retrieved from node %d\n\n---Enter GET/PUT request----\n\n", fetched_value, node_id);
          }
          else{
            sprintf(send_data, "RESULT: no hash entry for this key on the node %d.\n\n---Enter GET/PUT request----\n\n", node_id);
          }
          send(sock, send_data, strlen(send_data), 0);
        }
        else{
          bytes_received = recv(sock, recv_data, 1024);
          recv_data[bytes_received] = '\0';
          if(add_data_to_hash_table(key, atoi(recv_data))){
              sprintf(send_data, "PUT operation done successfully. data added to node %d\n\n---Enter GET/PUT request----\n\n", node_no);
          }
          else{
              strcpy(send_data, "PUT operation has been failed");
          }
          send(sock, send_data, strlen(send_data), 0);
          close(sock);
          fflush(stdout);
        }
    }

    else if(FD_ISSET(0, &readfds)){
      char rec_buffer[2048];
      gets(rec_buffer);
      if(rec_buffer[0] == 'r' || rec_buffer[0] == 'R'){
        DisplayTable(num_id);
      }
      else if(check_if_data_is_local(node_id, rec_buffer) == 0){
        char outputbuff[40], *out, flag;
        int i = 0;
        if(check_cmd(rec_buffer) == 0){
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
          value = fetch_value_from_hash_table(key, num_id);
          if(value == 0){
            printf("no value in the hash table for the key %d\n", key);
          }
          else{
            printf("Key = %d\t-->\t Value = %d\n", key, value);
          }
      }
        else{
          key = extract_key_from_put(rec_buffer);
          value = extract_value_from_put(rec_buffer);
          add_data_to_hash_table(key, value, num_id);
        }
      }
      fflush(stdout);
    }// end of probing console
  } // end of while(1)
  return 0;
}
