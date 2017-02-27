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

/*--------------- packets ---------------*/
struct packet {
  int x;
  int y;
};
/*--------------- results ---------------*/
struct results {
  int result;
};

/*-------------- defined functions ----------------*/
void checkPrime(int x, int y); //checkPrime
void *threadFunction(void *socket); //threadFunction

/*------------------------------*/
int clientSocket, checkConnection; //global  variables
struct packet pk;
struct results rst;
pthread_t td;
const int running = 1;

/*-------------- Main client function ----------------*/
int main() {
  struct sockaddr_in serverAddr;
  char *ipAddress = "127.0.0.1";
  int x, y, i = 1;
  socklen_t addr_size;

  /*---- Create the socket. The three arguments are: ----*/
  clientSocket = socket(PF_INET, SOCK_STREAM, 0);
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(7893);  /* Set port number*/
  serverAddr.sin_addr.s_addr = inet_addr(ipAddress);  /* Set IP address*/
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  /* set padding bits field to 0 */
  addr_size = sizeof serverAddr;  /* Connect the socket to server*/
  checkConnection =
      connect(clientSocket, (struct sockaddr *)&serverAddr, addr_size);
  if (checkConnection == -1) {
    perror("Error connections");
    exit(-1);
  }
  while (running) {
    if (checkConnection == -1)
      break;
    threadFunction(&clientSocket); // calling threadFunction
  }

  pthread_exit(NULL);
  return 0;
}

/*--------------- thread function ---------------*/
void *threadFunction(void *socket) {
  while (running) {
    recv(clientSocket, &(pk), sizeof(pk), 0); // receive data from server
    printf("Server data: %d\n", pk.x);        // print received data
    printf("Sqrt: %d\n", pk.y);               // print received data
    checkPrime(pk.x, pk.y);                   // Check for prime numbers
    pthread_create(&td, NULL, threadFunction, (void *)&socket); // thread
  }
}

/*---------------- check primes and send data  --------------*/
void checkPrime(int x, int y) {
  if (x % y == 0) {
    rst.result = x;
    write(clientSocket, &rst, sizeof(rst));
    write(clientSocket, &(rst), 1);
  } else if (y == sqrt(x)) {
    rst.result = x;
    write(clientSocket, &rst, sizeof(rst));
    write(clientSocket, &(rst), 1);
  } else {
    rst.result = x; // assign
    write(clientSocket, &rst, sizeof(rst));
    write(clientSocket, &(rst), 1);
  }
}
