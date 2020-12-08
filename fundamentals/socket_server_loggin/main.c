/*
	Program:
        Simple server login
        Example of accept, bind, catch_signal, close, fork, inet_ntoa,  listen, memset,ntohs, recv, send, sigaction, setsockopt, sockaddr_in, socket, strcmp, struct sigaction, sigemptyset, recv
	Compile:
		gcc main.c -o server
	-------------------------------------
	Run:
		./server
*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <unistd.h>
#include <signal.h>



int socket_receive_message(int socket, char *buffer, int buffer_length);

int socket_send_message(int socket, const char *message);

void socket_exit_if_value(int value, int value_to_test, const char *msg);

struct sockaddr_in socket_init_name(char *server_ip, int server_port);

int catch_signal(int sig, void (*handler)(int));

void shutdown_handler(int sig);

void client_handler(int socket_client, struct sockaddr_in *client_address, char *valid_user, char *valid_pass);

int socket_listener;

int main()
{
    char *valid_user = "john doe";
    char *valid_pass = "123456";

    int catch_signal_result = catch_signal(SIGINT, shutdown_handler);
    socket_exit_if_value(-1, catch_signal_result, "Can't set interrupt handler");

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
    socket_listener = socket(PF_INET, SOCK_STREAM, 0);
    socket_exit_if_value(-1, socket_listener, "Error: cannot create socket");

    // reuse ip address
    int reuse = 1;
    int set_sock_opt_result = setsockopt(socket_listener, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(int));
    socket_exit_if_value(-1, set_sock_opt_result , "Error: cannot set reuse option");

    // set socket name
    struct sockaddr_in name = socket_init_name(NULL, 30000);

    // bind address to socket (Traditionally called “assigning a name to a socket”)
    int bind_result = bind(socket_listener, (struct sockaddr *)&name, sizeof(name));
    socket_exit_if_value(-1, bind_result , "Error: cannot bind address to socket");

    // listen for clients
    int listen_result = listen(socket_listener, 10);
    socket_exit_if_value(-1, listen_result, "Error: cannot listen");

    puts("Server waiting for clients:");
    while(1)
    {
        // struct for client data
        struct sockaddr_in client_address;
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

            client_handler(socket_client, &client_address, valid_user, valid_pass);

            // child exit
            exit(0);
        }
        close(socket_client);
    }
    return 0;
}

void client_handler(int socket_client, struct sockaddr_in *client_address, char *valid_user, char *valid_pass)
{
    char *ip = inet_ntoa(client_address->sin_addr);
    int port = ntohs(client_address->sin_port);
    printf("Client address -> %s:%i\n", ip, port);

    char message_received[256];

    int result = socket_send_message(socket_client, "User:\n");
    socket_exit_if_value(-1, result, "Can't send 'User' request'");

    result = socket_receive_message(socket_client, message_received, sizeof(message_received));
    socket_exit_if_value(-1, result, "Can't receive 'User' from client");

    if(strcmp(valid_user, message_received) != 0)
    {
        socket_send_message(socket_client, "Invalid user\r\n");
        exit (0);
    }

    result = socket_send_message(socket_client, "Pass: ");
    socket_exit_if_value(-1, result, "Can't send 'Password' request");

    result = socket_receive_message(socket_client, message_received, sizeof(message_received));
    socket_exit_if_value(-1, result, "Can't receive 'Password' from client");

    if(strcmp(valid_pass, message_received) != 0)
    {
        socket_send_message(socket_client, "Invalid password\r\n");
        exit (0);
    }
    // logged in successfully
    socket_send_message(socket_client, "You are logged!\r\n");
}

void shutdown_handler(int sig)
{
    if (socket_listener)
    {
        close(socket_listener);
    }
    fprintf(stderr, "\rProgram interrupted\n");
    exit(0);
}

int catch_signal(int sig, void (*handler)(int))
{
    struct sigaction action;
    action.sa_handler = handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    return sigaction(sig, &action, NULL);
}

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
