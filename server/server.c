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
  const int MAX_CONNECTION = 10;

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


  while (1) { //Client is allowed to make request to this part many times
    pk.x = (int)randomGenerator(); //Get a huge generated number
    pk.y = sqrt((int)randomGenerator()); //Get sqrt of the generated number

    /* Test the out puts */
    printf("Original: %d\n", pk.x); //Test values for x
    printf("Sqrt of Original: %d\n", pk.y); //Test values for y

    if (listen(welcomeSocket, MAX_CONNECTION) == 0)
      printf("Listening %s\n", ipAddress); //Show the address server is listening at
    else
    printf("Error\n"); //Print error if there is connection issues

    addr_size = sizeof serverStorage;
    newSocket = accept(welcomeSocket, (struct sockaddr *)&serverStorage, &addr_size);
    send(newSocket, &(pk), sizeof(pk), 0); // send the packet to the client.
  }

  return 0;
}
