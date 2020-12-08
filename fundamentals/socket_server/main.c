/*
    Program:
        Start a server (port 30000) on any local interface (ip) listening for clients, once conection is stablished with a client send a string
        Display a error message if an error occurs and socket function where error start set errno var
        Example of accept, bind, close, Display errno, listen, rand, send, setsockopt, sockaddr_in, srand
    Compile:
        gcc main.c -o socket_server
    -------------------------------------
    Run:
        ./socket_server
    Test: (Works with any local address on port 3000)
        Telnet 127.0.0.1 30000
    View error code:
        echo $? on terminal
*/

#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>

void error(const char* message)
{
        fprintf(stderr, "%s: %s\n", message, strerror(errno));
        exit(1);
}

int main(int argc, char *argv[])
{
    char *days[] = { "Monday\r\n", "Tuesday\r\n", "Wednesday\r\n", "Thursday\r\n", "Friday\r\n",  "Saturday\r\n", "Sunday\r\n" };

    // Use current time as seed for random generator (rand())
    srand(time(0));

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
    int listener_d = socket(AF_INET, SOCK_STREAM, 0);
    if(listener_d == -1)
    {
        error("Error: creating socket");
    }

    int reuse = 1;
    /*
        set reuse address option on socket layer
        arguments:
            sockfd: socket file descriptor
            level: level at which the option resides. SOL_SOCKET stands for socket level
            optname: name of the option. SO_REUSEADDR allows reuse addresses for this socket (restart socket without problem)
            optval: value to set in the option. This case nonzero value means "yes"
            socklen_t: used to access option value
    */
    if (setsockopt(listener_d, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1)
    {
        error("Error: can't set reuse option on socket");
    }

    // initializing socket name
    struct sockaddr_in name;
    /*
       sin_family (socket internet family): always is set to AF_INET: https://man7.org/linux/man-pages/man7/ip.7.html
    */
    name.sin_family = AF_INET;
    /*
       sin_port (socket internet port): port to open in network byte order (port numbers below 1024 are privileged ports)
       The htons(hostshort) function converts the unsigned short integer hostshort from host byte order to network byte order.
    */
    name.sin_port = (in_port_t)htons(30000);
    /*
       INADDR_ANY is used when you don't need to bind a socket to a specific IP.
       When you use this value as the address when calling bind(), the socket accepts connections to all the IPs of the machine.
       The htonl(hostlong) function converts the unsigned integer hostlong from host byte order to network byte order.
       sin_addr.s_addr (socket internet address socket address):address in network byte order
    */
    name.sin_addr.s_addr = htonl(INADDR_ANY);

    // bind address to socket (Traditionally called “assigning a name to a socket”)
    if (bind(listener_d, (struct sockaddr *)&name, sizeof(name)) == -1)
    {
        error("Error: binding address to socket");
    }

    /*
        listen for incoming connections
        socket
        backlog: maximum queue length
    */
    if (listen(listener_d, 10) == -1)
    {
        error("Error: listening");
    }

    puts("Waiting for connection");
    while (1)
    {
        // accept connection
        /* create a sockaddr struct for client data */
        struct sockaddr_storage client_addr;
        unsigned int address_size = sizeof(client_addr);

        // accepts connection from listener_d queue, creates a new connection socket and fills client_addr with the address of the peer socket
        int connect_d = accept(listener_d, (struct sockaddr *)&client_addr, &address_size);
        if (connect_d == -1)
        {
            error("Error: accepting incoming connection");
        }

        char *msg = days[rand() % 5];
        // send msg to connection socket
        send(connect_d, msg, strlen(msg), 0);

        // close connection
        close(connect_d);
    }
    return 0;
}
