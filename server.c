/*
A simple server and client project
 In server side you will have the following process:
    1. Create socket with socket().
    2. Use bind() to set the IP and port. (Only for server)
    3. Then you call listen() to listen for connections
    4. Establish the connection and being ready to send and recieve data.

Resources reference:
1. https://nikhilroxtomar.medium.com/tcp-client-server-implementation-in-c-idiot-developer-52509a6c1f59
2. https://www.cs.cmu.edu/~srini/15-441/S10/lectures/r01-sockets.pdf

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//For network programming
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>

//For handling errors
#include <errno.h>

#define PORT 5555

int main()
{


    int rv; //A variable to whole any return value
    int server_socket_fd, client_socket_fd; // This will store the socket file descriptor when you call socket();
    int client_address_size;
    struct sockaddr_in server_address, client_address; //Creating a sockaddr_in structure for both server and client.
    char buffer[2048]; //This buffer is to be use for sending and receiving data between client and server.


    /* 1. Create a socket */


    //From man socket: int socket(int domain, int type, int protocol);
    server_socket_fd = socket(AF_INET, SOCK_STREAM, 0); //based on the man pages you can find the protocol number in /etc/protocols
    //AF_INET is for IPv4 Internet protocols 
    //SOCK_STREAM is used as we want to have 2 way reliable connection (TCP)
    //0 stands for Internet Protocol
    if (server_socket_fd < 0) //Here we use perror() to handle the errors
    {
        perror("Socket creation failed!\n");
        exit(1);
    }

    else
    {
        printf("TCP Socket created!\n");
    }




    /* Initializing server address and port */

    memset(&server_address, '\0', sizeof(server_address)); //This is to zero out the whole structure
    server_address.sin_family = AF_INET; 
    server_address.sin_addr.s_addr = htonl(INADDR_ANY); //This is to set it to receive connection from any address or you can just set it to the local loopback address.
    //You can set: int *ip = "127.0.0.1"; then server_address.sin_addr.s_addr = inet_addr(ip);
    //inet_addr translate an internet address into network byte order.
    server_address.sin_port = htons(PORT); //The port to listen to.
    //htons translates a short integer from host byte order to network byte order




    /* Binding the socket_fd with the server information above */

    rv = bind(server_socket_fd, (struct sockaddr*)&server_address, sizeof(server_address));

    if (rv < 0)
    {
        perror("Erro binding...\n");
        exit(1);
    }



    /* Listen to incoming connections */

    //int listen(int sockfd, int backlog);
    rv = listen(server_socket_fd, 10);
    if (rv < 0)
    {
        perror("Error listening...\n");
        exit(1);
    }

    printf("Listening for connection on port: %d\n", PORT);
    //The backlog argument defines the max length incoming connections may queue. If exceed queue length, client may receive an error.
    //You can treat listen() as a queue where all the connections that comes in, queue up and wait for their turn to be handled.
    //Just to note that this server only handles one single connection at any one time.

    //If you only want to accept one connection and end the connection, you can follow the below code:
    // rv = accept(socket_fd, (struct sockaddr *) &client_address, sizeof(client_address));
    // if (rv < 0)
    // {
    //     perror("Error accepting...\n");
    //     exit(1);
    // } 
    // else
    // {
    //     printf("Client connected...\n");
    // }

    //If not we can use a while loop to continuously accept new connections, but again only one single connection is handle at any one time.
    while(1)
    {
        client_address_size = sizeof(client_address);
        client_socket_fd = accept(server_socket_fd, (struct sockaddr *) &client_address, &client_address_size ); //GOT ISSUE!!!
        if (client_socket_fd < 0)
        {
            perror("Error accepting...\n");
            exit(1);
        }
        printf("Connection between server and client has been established successfully.\n");
        //We can also use recv() and send() to receieve and send data with the client.
        //or using read() and write() to do the same.

        //Below is an example of using recv() and send()

        //Receving from client
        memset(buffer, '\0', sizeof(buffer)); //Zeroing out the buffer
        recv(client_socket_fd, buffer, sizeof(buffer), 0); //ssize_t recv(int sockfd, void *buf, size_t len, int flags);
        //This receive the client socket file descriptor and store it in buffer.
        //4th arg is 0 means no flag set.
        printf("Client Message: %s\n", buffer); //We then print out the client message that is store in the buffer.

        //Sending to client
        memset(buffer, '\0', sizeof(buffer)); //Zeroing out the buffer
        strcpy(buffer, "Hi! I am server. Nice to meet you!\n");
        printf("Server Message: %s\n", buffer);
        send(client_socket_fd, buffer, strlen(buffer), 0); //Pass the buffer info which contains the server message into the client socket file descriptor and send it over back to the client
        
        //Closing the client socket file descriptor
        close(client_socket_fd);
        printf("Client has been disconnected.\n");

    }

    return 0;



}