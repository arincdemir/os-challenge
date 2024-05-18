#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "dehash.h"

#define BUFFER_SIZE 1024
#define BACKLOG_SIZE 1000

void bigEndianHashToLittleEndian(char * hash) {
    char temp[32];
    for (int i = 0; i < 32; i++) {
        temp[31 - i] = hash[i];
    }
    memcpy(hash, temp, 32);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Please provide a port number\n");
        exit(1);
    }

    int port = atoi(argv[1]);
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Error creating socket");
        exit(1);
    }

    // Set up server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket to server address
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error binding");
        exit(1);
    }

    // Listen for incoming connections
    if (listen(server_socket, BACKLOG_SIZE) == -1) {
        perror("Error listening");
        exit(1);
    }

    printf("Server is listening on port %d...\n", port);

    int connection_number = 0;
    while (1) {
        // Accept incoming connection
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_len);
        if (client_socket == -1) {
            perror("Error accepting client connection");
            continue;
        }

        printf("[#%d]Accepted connection from %s:%d\n", connection_number, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        connection_number++;
        
        // Read data from the client
        ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received == -1) {
            perror("Error receiving data");
            close(client_socket);
            continue;
        }

        // Null-terminate the received data
        buffer[bytes_received] = '\0';


        // Seperate the request into hash, start, end, and priority and change big endian to little endian
        char hash[32];
        unsigned long start;
        unsigned long end;
        char priority;

        memcpy(hash, buffer, 32);
        memcpy(&start, buffer+32, 8);
        memcpy(&end, buffer+40, 8);
        memcpy(&priority, buffer+48, 1);

        //bigEndianHashToLittleEndian(hash);
        start = be64toh(start);
        end = be64toh(end);

        printf("Our current hash: ");
        printHashHex(hash);
        printf("Start: %lu\n", start);
        printf("End: %lu\n", end);
        printf("Priority: %d\n", priority);

        // Print and send back the received data
        //unsigned long response = 31;
        unsigned long response = dehash(hash, start, end);
        response = htobe64(response);
        send(client_socket, &response, 8, 0);

        // Close the client socket
        close(client_socket);
    }

    // Close the server socket
    close(server_socket);

    return 0;
}
