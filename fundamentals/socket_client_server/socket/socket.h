
int socket_receive_message(int socket, char *buffer, int buffer_length);

int socket_send_message(int socket, const char *message);

void socket_exit_if_value(int value, int value_to_test, const char *msg);

/*
   Initialize and return a struct sockaddr_in. If server_ip is NULL,
   INADDR_ANY is used (allow connections on all the IP's of the machine)
 */
struct sockaddr_in socket_init_name(char *server_ip, int server_port);
