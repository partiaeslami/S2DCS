#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include "filestruct.h"

#include <stdint.h>
#define READ_FILE_BUF 2000
#define BUFFER_SIZE 3000
#define PACKET_SIZE sizeof(pkt->filename_len) + sizeof(pkt->filename) + sizeof(pkt->file_size) + reading // packet sizing

int supersum =0;

static void sumthreads(int returnval){

  supersum+=returnval;
  printf("SUM: %d\n", supersum);

}

static void handle_client(int accepting){
  char data_arena[65536];
  packet *pkt = (packet *)data_arena;

  printf("SENDING PAYLOAD OVER\n"); //send a file over from input
  strcpy(pkt->filename,"payload\n");
  pkt->filename_len = strlen(pkt->filename) - 1;
  pkt->filename[pkt->filename_len] = '\0';

  struct stat buffer;
  int stating = stat(pkt->filename, &buffer); // loading file from packet into buffer
  pkt->file_size = buffer.st_size; //extracting file size into packet
  printf("SIZE: %ld bytes\n", pkt->file_size);

  int opening = open(pkt->filename, O_RDONLY); //(filepath, mode) opening file to read only and prepraring to send (returns file descriptor)

  ssize_t reading = read(opening, pkt->content, pkt->file_size);
  // referencing the open we extract a file from on disk and have the content ready to be read from "pkt-> content" 
  // read requires some finite amount of bytes to read from, so we give it the size of the file to read from the packet

  ssize_t sending = send(accepting, pkt, PACKET_SIZE, 0); // sending a packet over

  uint64_t count = reading; //sent the first packet

  while (count < (pkt->file_size)) { //continue until done
    reading = read(opening, pkt->content, READ_FILE_BUF); //packet by packet
    
    sending = send(accepting, pkt->content, reading, 0);

    count += reading;
  }

  printf("SENT\n");
 // bzero(,);
  char bufferInput[255];
  recv(accepting,bufferInput, 255, 0);
  printf("%d\n",atoi(bufferInput));
  sumthreads(atoi(bufferInput));
}

static int server_sokcet(char *ipAddress, int port){
  struct sockaddr_in serverAddress; //standard procedure
  memset(&serverAddress, 0, sizeof(ipAddress));
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(port);
  serverAddress.sin_addr.s_addr = inet_addr(ipAddress);

  int socking = socket(AF_INET, SOCK_STREAM, 0); //tcp socket creation

  int binding = bind(socking, (struct sockaddr *)&serverAddress, sizeof(serverAddress)); //binding socket to the address given

  int listening = listen(socking, 3); //listening and allowing three clients maximum just for now

  printf("IPADDRESS: %s   PORT: %d\n", ipAddress, port);
  
  int accepting;

  struct sockaddr_in clientAddress; //preparing client(s)
  memset(&clientAddress, 0, sizeof(clientAddress));

  socklen_t len = sizeof(struct sockaddr_in);
  while( 1 == 1 ) {
    
    accepting = accept(socking, (struct sockaddr *)&clientAddress, &len);
    pthread_t newthread;

    printf("INCOMING CLIENT (%s:%d)\n",inet_ntoa(clientAddress.sin_addr), ntohs(clientAddress.sin_port));
    
    if (accepting < 0) {
      perror("client failed..."); //error checking for listening
      continue; //move on for any other clients
    } else {
     pthread_create(&newthread, NULL, (void*) handle_client, accepting); //pass accept over to handle incoming clients
    }

  }
}

int main(int argc, char *argv[]){
  if (argc < 3) {
    printf("Error please use the following format while executing <IP_ADDRESS> <PORT>\n");
    return 1;
  }

  server_sokcet(argv[1], atoi(argv[2])); //pass ipaddress and port over

  return 0;

}

