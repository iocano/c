#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include "socket.h"
#include <ctype.h>

int socket_receive_message(int socket, char *buffer, int buffer_length)
{
    char *message = buffer;
    int message_length = buffer_length;
    int bytes_readed = 0;
    do
    {
        message += bytes_readed ;
        bytes_readed = recv(socket, message, message_length, 0);
        message_length  -= bytes_readed;
    }while ( (bytes_readed > 0) && (message[bytes_readed - 1] != '\n'));

    if (bytes_readed < 0)
    {
        return bytes_readed;
    }
    else if (buffer_length == message_length)
    {
        buffer[0] = '\0';
    }
    else
    {
        // remove "\r\n"
        message[bytes_readed - 2] = '\0';
    }
    return buffer_length - message_length;
}

int socket_send_message(int socket, const char *message)
{
    int bytes_sended = send(socket, message, strlen(message), 0);
    if (bytes_sended == -1)
    {
        fprintf(stderr, "%s: %s\n", "Error: can't send message", strerror(errno));
    }
    return bytes_sended;
}

void socket_exit_if_value(int value, int value_to_test, const char *msg)
{
    if(value == value_to_test)
    {
        fprintf(stderr, "%s.\n %s\n", msg, strerror(errno));
        exit(EXIT_FAILURE);
    }
}

struct sockaddr_in socket_init_name(char *server_ip, int server_port)
{
    struct sockaddr_in name;
    // clean garbage values
    memset(&name, 0, sizeof(name));
    /*
       sin_family (socket internet family): always is set to AF_INET: https://man7.org/linux/man-pages/man7/ip.7.html
    */
    name.sin_family = AF_INET;
    /*
       sin_port (socket internet port): port to open in network byte order (port numbers below 1024 are privileged ports)
       The htons(hostshort) function converts the unsigned short integer hostshort from host byte order to network byte order.
    */
    name.sin_port = (in_port_t)htons(server_port);
    // server respond to any local ip
    if(server_ip == NULL)
    {
        /*
           INADDR_ANY is used when you don't need to bind a socket to a specific IP.
           When you use this value as the address when calling bind(), the socket accepts connections to all the IPs of the machine.
           The htonl(hostlong) function converts the unsigned integer hostlong from host byte order to network byte order.
           sin_addr.s_addr (socket internet address socket address):address in network byte order
        */
        name.sin_addr.s_addr = htonl(INADDR_ANY);
    }
    // server respond specified ip
    else
    {
        name.sin_addr.s_addr = inet_addr(server_ip);
    }
    return name;
}
