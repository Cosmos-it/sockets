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
#include <unistd.h>

/*-----------function declaration -----------*/
long randomGenerator();
/*-------------- packet ----------------*/
struct packet {
  int x;
  int y;
};
/*-------------- resut ----------------*/
struct results {
  int result;
};
/*------------- Random generator -----------*/
long randomGenerator() {
  time_t t;
  srand((unsigned)time(&t));
  return rand() % 800 + 777;
}
/*------------ main server function -----------*/
int main() {
  struct packet pk;   // initialize packet
  struct results rst; // initialize result
  int welcomeSocket, newSocket;
  char *ipAddress = "127.0.0.1"; // id address
  struct sockaddr_in serverAddr; // socket address
  struct sockaddr_storage serverStorage;
  socklen_t addr_size;
  int MAX_CONNECTION = 5; // max connection
  int running = 1;        // continues loop const

  /*-------------- Create the socket. The three arguments are: ----------*/
  welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(7893);                 /* Set port number*/
  serverAddr.sin_addr.s_addr = inet_addr(ipAddress); // Set IP address
  memset(serverAddr.sin_zero, '\0',
         sizeof serverAddr.sin_zero); // set padding bits field to 0
  bind(welcomeSocket, (struct sockaddr *)&serverAddr,
       sizeof(serverAddr)); // Bind the address struct to the socket

  while (running) { // Client is allowed to make request to this part many times
    if (listen(welcomeSocket, MAX_CONNECTION) == 0)
      printf("Listening %s\n",
             ipAddress); // Show the address server is listening at
    else
      printf("Error\n"); // Print error if there is connection issues

    addr_size = sizeof serverStorage;
    newSocket =
        accept(welcomeSocket, (struct sockaddr *)&serverStorage, &addr_size);

    pk.x = (int)randomGenerator();          // Get a huge generated number
    pk.y = sqrt((int)randomGenerator());    // Get sqrt of the generated number
    printf("Original: %d\n", pk.x);         // Tests the out puts
    printf("Sqrt of Original: %d\n", pk.y); // Tests the out puts
    send(newSocket, &(pk), sizeof(pk), 0);  // send the packet to the client.
    read(newSocket, &(rst), sizeof(rst));   // read from client
    read(newSocket, &(rst), sizeof(rst));   // read from client
    printf("Received %d\n",
           rst.result); // print received data for testing purpose

    if (rst.result % 2 == 0) break; // break loop if result is not a prime
  }

  return 0;
}
