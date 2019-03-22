#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

int main (int argc, char *argv[]) {

	if (argc != 3) printf("Usage: %s <ip_address> <port_number>\n", argv[0]);
	else {

		char *serv_ip = argv[1];
		unsigned short port = atoi(argv[2]);
		int socket_id = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		if (socket_id < 0) {

			perror("Socket was not created");
			exit(EXIT_FAILURE);
		}

		struct sockaddr_in address;
		address.sin_family = AF_INET;
		address.sin_port = htons(port);

		if (inet_pton(address.sin_family, serv_ip, &address.sin_addr) <= 0) {

			perror("Given address is invalid");
			exit(EXIT_FAILURE);
		}

		if (connect(socket_id, (struct sockaddr*) &address, sizeof(address)) < 0) {

			perror("Can't connect to the given address");
			exit(EXIT_FAILURE);
		}

		char buffer[256] = {0};
		unsigned short size = 256;

		if (recv(socket_id, buffer, size, 0) < 0) {

			perror("Message not recieved");
			exit(EXIT_FAILURE);
		} else {

			system("echo \"Welcome to the leviathan server\n\nThis server host a chat service!\nTo chat with the leviathan admin type something...\" | cowsay -f eyes | toilet -f term");
			printf("\n\n");
			bzero(buffer, size);
		}

		while (1) {

			printf("Client: ");
			fgets(buffer, size, stdin);

			if (send(socket_id, buffer, size, 0) < 0) {

				perror("Can't send the message");
				exit(EXIT_FAILURE);
			}

			if (strncmp("bye!", buffer, 4) == 0) break;
			else bzero(buffer, size);
			
			if (recv(socket_id, buffer, size, 0) < 0) {

				perror("Can't recieve the message");
				exit(EXIT_FAILURE);
			} else {

				printf("Server: %s", buffer);
			}
			printf("\n");

			if (strncmp("bye!", buffer, 4) == 0) break;
			else bzero(buffer, size);
		}

		if (close(socket_id) < 0) {

			perror("Socket was not closed");
			exit(EXIT_FAILURE);
		}
	}
}