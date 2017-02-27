/****************** CLIENT ****************/
//  client.c
//  LabFour
//  Created by Taban Cosmos on 2/17/17.
//  Copyright © 2017 Taban Cosmos. All rights reserved.

#include <arpa/inet.h>
#include <math.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

/* Packet struct */
struct packet {
  int x;
  int y;
};

struct results {
  int result;
};

void checkPrime(int x, int y);
void *threadFunction(void *socket);

// Global variables:
int clientSocket, checkConnection;
struct packet pk;
struct results rst;
pthread_t td;
const int running = 1;

/* Main */
int main() {
  struct sockaddr_in serverAddr;
  char *ipAddress = "127.0.0.1";
  int x, y, i = 1;
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
  checkConnection = connect(clientSocket, (struct sockaddr *)&serverAddr, addr_size);
  if (checkConnection == -1) {
    perror("Error connections");
    exit(-1);
  }
  while (running) {
    if (checkConnection == -1)
      break;
    /* code */
    threadFunction(&clientSocket);              // calling threadFunction
    //send(clientSocket, &(rst), sizeof(rst), 0); // send the new data to client
  }
  pthread_exit(NULL);

  return 0;
}

void *threadFunction(void *socket) {
  /*---- Print the received message ----*/
  while (running) {
    /*---- Read the message from struct packet ----*/
    recv(clientSocket, &(pk), sizeof(pk), 0); // receive data from server
    printf("Data received: %d\n", pk.x);      // print received data
    printf("Data received Sqrt: %d\n", pk.y); // print received data
    checkPrime(pk.x, pk.y);  // Check for prime numbers
    write (clientSocket, &rst, sizeof (rst));
    write(clientSocket, &(rst), 1); // print result
    pthread_create(&td, NULL, threadFunction, (void *)&socket);
  }
}

/* Check for primes */
void checkPrime(int x, int y) {
  /* code */
  if (x % y == 0) {
    rst.result = x;
    printf("%d %s\n", rst.result, ": not prime numnber");
  } else if (y == sqrt(x)) {
    rst.result = y;
    printf("%d %s\n", rst.result, ": not prime numnber");
  } else {
    rst.result = x; // assign
    printf("%d\n", rst.result);
  }
}
