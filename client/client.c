/****************** CLIENT ****************/
//  client.c
//  LabFour
//  Created by Taban Cosmos on 2/17/17.
//  Copyright © 2017 Taban Cosmos. All rights reserved.

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

/* Packet struct */
struct packet { int x; int y; };
void checkPrime(int x, int y);

/* Main */
int main() {
  struct packet pk;
  int clientSocket;
  char *ipAddress = "127.0.0.1";
  int x, y;
  struct sockaddr_in serverAddr;
  socklen_t addr_size;

  /*---- Create the socket. The three arguments are: ----*/
  clientSocket = socket(PF_INET, SOCK_STREAM, 0);
  serverAddr.sin_family = AF_INET;
  /* Set port number*/
  serverAddr.sin_port = htons(7891);
  /* Set IP address*/
  serverAddr.sin_addr.s_addr = inet_addr(ipAddress);
  /* set padding bits field to 0 */
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
  /* Connect the socket to server*/
  addr_size = sizeof serverAddr;
  connect(clientSocket, (struct sockaddr *)&serverAddr, addr_size);

  /*---- Read the message from the server into the buffer ----*/
  recv(clientSocket, &(pk), sizeof(pk), 0);
  /*---- Print the received message ----*/
  printf("Data received: %d\n", pk.x);
  printf("Data received Sqrt: %d\n", pk.y);
  checkPrime(pk.x, pk.y); //Check for prime numbers

  return 0;
}

/* Check for primes */
void checkPrime(int x, int y){
  if (x % y == 0) {
    printf("%d %s\n", x, ": is a prime number");
  } else if (x % y == y) {
    printf("%d %s %d\n", x, ": equals to y: ", y);
  }
  else {
      printf("%d %s\n", x, ": is not a prime");
  }
}
