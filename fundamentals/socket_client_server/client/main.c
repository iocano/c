/*
	Program:
		Receive a message from server, input message and send to server
        Example of close, connect, memset, recv, send, sockaddr_in, sock
	Compile:
		gcc main.c -o client
	-------------------------------------
	Run:
		./client
*/

#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include "../socket/socket.h"

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
    int socket_client = socket(PF_INET, SOCK_STREAM, 0);
    socket_exit_if_value(-1, socket_client, "Error: cannot create socket");

    // set server socket name (ip and port)
    struct sockaddr_in name = socket_init_name(NULL, 3000);
    // Connect the socket to the host
    int connnect_result = connect(socket_client, (struct sockaddr *)&name, sizeof(name));
    socket_exit_if_value(-1, connnect_result, "Error: cannot connect socket to host");

    char message_from_server[512];
    int bytes_readed = socket_receive_message(socket_client, message_from_server, sizeof(message_from_server));
    printf("Server: %s\n Bytes: %i\n", message_from_server, bytes_readed);

    // Message for server
    char message_to_server[512];
    printf("Message for server: ");
    fgets(message_to_server, 512, stdin);
    // custom protocol: messages ends with '\n'
    socket_send_message(socket_client, message_to_server);

    close(socket_client);
}
