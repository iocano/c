/*
	Program:
		Wait for clients, when one client connects, create a child, send a message to client and
        receive a message back, then finalizes that connection and keeps waiting for clients
        Example of accept, bind, close, fork, listen, memset, recv, send, setsockopt, sockaddr, socket, recv
	Compile:
		gcc main.c -o server
	-------------------------------------
	Run:
		./server
*/

#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include "../socket/socket.h"

void handle_client_connection(int socket_client);

int main()
{
    /*
        create socket
        arguments:
            DOMAIN: Could be:
            PF_INET: Protocol Family Internet or
            AF_INET: Address Family Internet
            * Note from https://man7.org/linux/man-pages/man2/socket.2.html about PF_INET == AF_INET
            * Standard recommends use AF_*)
        TYPE:
            SOCK_STREAM: Provides sequenced, reliable, two-way, connection-based byte streams (tcp)
            SOCK_DGRAM: Supports datagrams connectionless, unreliable messages of a fixed maximum length (udp)
        PROTOCOL:
            The apropiate for protocol family (domain), the protocols are on /etc/protocols file.
            This case '0' is for IP
        socket examples:
            tcp_socket = socket(AF_INET, SOCK_STREAM, 0);
            udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
            raw_socket = socket(AF_INET, SOCK_RAW, protocol);
    */
    int socket_listener = socket(AF_INET, SOCK_STREAM, 0);
    socket_exit_if_value(-1, socket_listener, "Error: cannot create socket");

    // reuse ip address
    int reuse = 1;
    int set_sock_opt_result = setsockopt(socket_listener, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(int));
    socket_exit_if_value(-1, set_sock_opt_result , "Error: cannot set reuse option");

    // set socket name
    struct sockaddr_in name = socket_init_name(NULL, 3000);

    // bind address to socket (Traditionally called “assigning a name to a socket”)
    int bind_result = bind(socket_listener, (struct sockaddr *)&name, sizeof(name));
    socket_exit_if_value(-1, bind_result , "Error: cannot bind address to socket");

    // listen for clients
    int listen_result = listen(socket_listener, 10);
    socket_exit_if_value(-1, listen_result, "Error: cannot listen");

    puts("Server waiting for clients:");
    while (1)
    {
        // struct for client data
        struct sockaddr_storage client_address;
        unsigned int client_address_size = sizeof(client_address);

        // accept client connection
        int socket_client = accept(socket_listener, (struct sockaddr *)&client_address, &client_address_size);

        /*
           If fork return 0 we are on child process
           fork function creates a new process by duplicating the calling process
           New process is referred as child process
           Calling process is referred as parent process
           The child process and the parent process run in separate memory spaces.
           At the time of fork() both memory spaces have the same content.
           Acctions performed by one of the processes do not affect the other.
           fork return:
                -1 : creation of child process was unsuccessful
                0 : return to the child
                > 0 : return the pid of the child process to the parent
       */
        if (fork() == 0)
        {
            // close socket_listener copy, on child we dont need it
            close(socket_listener);
            // send/receive message to/from client
            handle_client_connection(socket_client);
            // child exit
            exit(0);
        }
        close(socket_client);
    }
    close(socket_listener);
    return 0;
}

void handle_client_connection(int socket_client)
{
    // custom protocol: messages ends with '\n'
    int bytes_sended = socket_send_message(socket_client, "Connected\n");

    char message_from_client[512];
    int bytes_readed = socket_receive_message(socket_client, message_from_client, sizeof(message_from_client));
    printf("Client: %s, Bytes: %i\n", message_from_client, bytes_readed);

    // close socket client on child
    close(socket_client);
}
