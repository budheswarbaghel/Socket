#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define QUEUE_LIMIT 5

int main (int argc, char *argv[]) {

	if (argc != 2) printf("Usage: %s <port_number>\n", argv[0]);
	else {

		unsigned short port = atoi(argv[1]);
		int socket_id = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		if (socket_id < 0) {

			perror("Socket was not created");
			exit(EXIT_FAILURE);
		}

		struct sockaddr_in address;
		address.sin_family = AF_INET;
		address.sin_port = htons(port);
		address.sin_addr.s_addr = htonl(INADDR_ANY);

		if (bind(socket_id, (struct sockaddr*) &address, sizeof(address)) < 0) {

			perror("Can't bind with the given port number");
			exit(EXIT_FAILURE);
		}

		if (listen(socket_id, QUEUE_LIMIT) < 0) {

			perror("Can't listen for incomming connection");
			exit(EXIT_FAILURE);
		}

		struct sockaddr_in client;
		int client_len = sizeof(client);

		int temp_sock = accept(socket_id, (struct sockaddr*) &client, (socklen_t*) &client_len);
		if (temp_sock < 0) {

			perror("Connection is not accepted");
			exit(EXIT_FAILURE);
		} else {

			printf("Connection is established with client := %d\n", temp_sock);
		}

		char *greetings = "Welcome To levithan server!";
		unsigned short size = strlen(greetings);

		if (send(temp_sock, greetings, size, 0) < 0) {

			perror("Greeting message is not sent");
			exit(EXIT_FAILURE);
		} else {

			printf("Greeting message was delivered to client := %d\n\n", temp_sock);
		}

		char buffer[256] = {0};
		size = 256;

		while (1) {

			if (recv(temp_sock, buffer, size, 0) < 0) {

				perror("Can't recieve the message");
				exit(EXIT_FAILURE);
			} else {

				printf("Client: %s", buffer);
			}

			if (strncmp("bye!", buffer, 4) == 0) break;
			else bzero(buffer, size);
			
			printf("Server: ");
			fgets(buffer, size, stdin);
			printf("\n");

			if (send(temp_sock, buffer, size, 0) < 0) {

				perror("Can't send the message");
				exit(EXIT_FAILURE);
			}

			if (strncmp("bye!", buffer, 4) == 0) break;
			else bzero(buffer, size);
		}

		if (close(socket_id) < 0) {

			perror("Socket was not closed");
			exit(EXIT_FAILURE);
		}
	}

	return 0;
}