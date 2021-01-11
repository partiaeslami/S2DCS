#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "filestruct.h"
#include <stdint.h>

#define READ_FILE_BUF 2000

#define BUFFER_SIZE 3000
#define PACKET_SIZE (size_t)( sizeof(pkt->filename_len) + sizeof(pkt->filename) + sizeof(pkt->file_size)) //packet sizing

int client_socket(char *ipAddress, int port){
  char data_arena[65538]; /* Why should we use heap if we can use stack? */
  packet *pkt = (packet *)&data_arena;
  socklen_t rlen = sizeof(struct sockaddr_in);

  struct sockaddr_in serverAddress; //standard procedure
  memset(&serverAddress, 0, sizeof(ipAddress));
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(port);
  serverAddress.sin_addr.s_addr = inet_addr(ipAddress);

  int socking = socket(AF_INET, SOCK_STREAM, 0); //tcp socket creation  

  int connecting = connect(socking, (struct sockaddr *)&serverAddress, sizeof(struct sockaddr_in)); //establishing connection to server  
  printf("Connection established!\n");

  printf("Waiting for file...\n");
  ssize_t read_bytes = 0;
  size_t count = 0;
  while (count < PACKET_SIZE){
    read_bytes = recv(socking, ((char *)pkt) + count, PACKET_SIZE + BUFFER_SIZE, 0); //read in packets (packet by packet) and 
    count += (size_t)read_bytes;

  } 

  char bufferFileName[512]; 
  sprintf(bufferFileName, "downloads/%s", pkt->filename); //assigning filename to bufferfilename and paths

  size_t readFileBytes = count - PACKET_SIZE; // pseudo count to write later

  int opening = open(bufferFileName, O_CREAT | O_WRONLY, S_IRWXU); //opens (bufferFileName, creates a file and writes the file, 
 
  printf("FILE INCOMING\n");
  ssize_t writing = write(opening, pkt->content, readFileBytes);
  while (readFileBytes < pkt->file_size) {

    read_bytes = recv(socking, pkt->content, BUFFER_SIZE, 0); //reading packets again

    readFileBytes += read_bytes; // balancing act between reading / writing

    writing = write(opening, pkt->content, read_bytes);//writing to the file path we established earlier packet by packet
  }
  printf("FILE DOWNLOADED\n\n");


  //processing
  pid_t forkStatus;
  forkStatus = fork(); 
  if(forkStatus == 0)
    execvp(bufferFileName, 0); 
  sleep(1);
  FILE *fp;
  char calculatedOutput[255];

  fp = fopen("test.txt", "r");
  fgets(calculatedOutput, 255, (FILE*)fp);
  fclose(fp);

  printf("%s\n", calculatedOutput);
  
  send(socking, calculatedOutput, (size_t)255, 0);
}

int main(int argc, char *argv[]){
  if (argc < 3) {
    printf("Error please use the following format while executing <IP_ADDRESS> <PORT>\n");
    return 1;
  }

  client_socket(argv[1], atoi(argv[2])); //pass ipaddress and port over
  
  return 0;

}
