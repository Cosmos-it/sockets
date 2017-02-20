/****************** SERVER CODE ****************/
//  server.c
//  LabFour
//  Created by Taban Cosmos on 2/17/17.
//  Copyright © 2017 Taban Cosmos. All rights reserved.

#include <arpa/inet.h>
#include <math.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>

/* Random generator */
long randomGenerator() {
  time_t t;
  srand((unsigned)time(&t));
  return rand() % 800 + 777;
}

/* Struct*/
struct packet { int x; int y; };

int main() {
  struct packet pk;
  int welcomeSocket, newSocket;
  char *ipAddress = "127.0.0.1";
  struct sockaddr_in serverAddr;
  struct sockaddr_storage serverStorage;
  socklen_t addr_size;

  pk.x = (int)randomGenerator();
  pk.y = sqrt((int)randomGenerator());
  /* Test the out puts */
  printf("Original: %d\n", pk.x);
  printf("Sqrt of Original: %d\n", pk.y);

  welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);
  serverAddr.sin_family = AF_INET;
  /* Set port number*/
  serverAddr.sin_port = htons(7891);
  /* Set IP address*/
  serverAddr.sin_addr.s_addr = inet_addr(ipAddress);
  /* set padding bits field to 0 */
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
  /*---- Bind the address struct to the socket ----*/
  bind(welcomeSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
  /*---- Listen on the socket, with 5 max connection requests queued ----*/
  if (listen(welcomeSocket, 5) == 0)
    printf("Listening %s\n", ipAddress);
  else
    printf("Error\n");

  /*---- Accept call creates a new socket for the incoming connection ----*/
  addr_size = sizeof serverStorage;
  newSocket = accept(welcomeSocket, (struct sockaddr *)&serverStorage, &addr_size);

  /*---- Send message to the socket of the incoming connection ----*/
  send(newSocket, &(pk.x), sizeof(pk.x), 0);

  return 0;
}
