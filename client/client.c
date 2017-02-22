/****************** CLIENT ****************/
//  client.c
//  LabFour
//  Created by Taban Cosmos on 2/17/17.
//  Copyright © 2017 Taban Cosmos. All rights reserved.

#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

/* Packet struct */
struct packet {
  int x;
  int y;
};
void checkPrime(int x, int y);
void *threadFunction(void *socket);

// Global variables:
int clientSocket;
struct packet pk;
pthread_t td;

/* Main */
int main() {
  char *ipAddress = "127.0.0.1";
  int x, y, i = 1;
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
  while (1) {
    threadFunction(&clientSocket);
  }
  pthread_exit(NULL);

  return 0;
}

void *threadFunction(void *socket) {
    /*---- Print the received message ----*/
    while (1) {
      /*---- Read the message from struct packet ----*/
      recv(clientSocket, &(pk), sizeof(pk), 0);
      printf("Data received: %d\n", pk.x);
      printf("Data received Sqrt: %d\n", pk.y);
      checkPrime(pk.x, pk.y); // Check for prime numbers
      pthread_create(&td, NULL, threadFunction, (void *)&socket);

    }
}

/* Check for primes */
void checkPrime(int x, int y) {
  /* code */
  if (x % y == 0) {
    printf("%d %s\n", x, ": is a prime number");
  } else if (x / y == y) {
    printf("%d %s %d\n", x / y, ": equals to y: ", y);
  } else {
    printf("%d %s\n", x, ": is not a prime");
  }
}
