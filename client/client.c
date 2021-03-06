///////////////////////////////////////////////////////
//  client.c
//  LabFour
//  Created by Taban Cosmos on 2/17/17.
//  Copyright © 2017 Taban Cosmos. All rights reserved.
///////////////////////////////////////////////////////

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

///////////////// Packets /////////////////////////////
struct packet {
  int x;
  int y;
};

//////////////// Results /////////////////////////
struct results {
  int result;
};

/////////////// Defined functions /////////////////
void checkPrime(int x, int y);      // checkPrime
void *threadFunction(void *socket); // threadFunction

int clientSocket, checkConnection; // global  variables
struct packet pk;
struct results rst;
pthread_t td;
const int running = 1;

///////////  Main client function ////////
int main() {
  struct sockaddr_in serverAddr;
  char *ipAddress = "127.0.0.1";
  int x, y, i = 1;
  socklen_t addr_size;

  ///////////////////////////////////////////////////////
  //// Create the socket. The three arguments are: /////
  clientSocket = socket(PF_INET, SOCK_STREAM, 0);
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(7893);                 /// Set port number
  serverAddr.sin_addr.s_addr = inet_addr(ipAddress); /// Set IP address
  memset(serverAddr.sin_zero, '\0',
         sizeof serverAddr.sin_zero); /// set padding bits field to 0
  addr_size = sizeof serverAddr;      /// Connect the socket to server
  checkConnection =
      connect(clientSocket, (struct sockaddr *)&serverAddr, addr_size);
  while (running) {
    if (checkConnection == 1)
      break;
    threadFunction(&clientSocket); //// calling threadFunction
  }
  pthread_exit(NULL);
  close(clientSocket);
  return 0;
}

///////////// Thread function /////////////////
void *threadFunction(void *socket) {
  int soc = (int)socket;
  while (running) {
    int rec =
        recv(clientSocket, &(pk), sizeof(pk), 0); // receive data from server
    if (rec > 0) {
      printf("Receiving data...\n");
      printf("x %d\n", pk.x);
      printf("y %d\n", pk.y);
      checkPrime(pk.x, pk.y);  // Check for prime numbers
      pthread_create(&td, NULL, threadFunction, (void *)&socket); // thread
    } else {
      printf("Connect closed...");
      exit(1);
    }
  }
  close(soc);
}

//////////////// check primes and send data  ////////////////
void checkPrime(int x, int y) {
  if (x % y == 0) {
    rst.result = x ;
    write(clientSocket, &rst, sizeof(rst));
  } else {
    rst.result = x;
    write(clientSocket, &rst, sizeof(rst));
  }
}
