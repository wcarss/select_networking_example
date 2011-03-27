#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

#define RESPONSE_LENGTH 256
#define MESSAGE_LENGTH 256

int main(int argc, char *argv[])
{
    int sockfd;
    int len, i;
    struct sockaddr_in address;
    int result;
    char ch = 'A';
    char message[MESSAGE_LENGTH], response[RESPONSE_LENGTH];
    if(argc == 1)
    {
      strcpy(message, "Good to see you, bond.");
    }
    else
    {
       strcpy(message, "");
       for(i = 1; i < argc-1; i++)
       {
          strcat(message, argv[i]);
          strcat(message, " ");
       }
       strcat(message, argv[argc-1]);
    }
/*  Create a socket for the client.  */

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

/*  Name the socket, as agreed with the server.  */

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(9734);
    len = sizeof(address);

/*  Now connect our socket to the server's socket.  */

    result = connect(sockfd, (struct sockaddr *)&address, len);

    if(result == -1) {
        perror("connect failed on client: ");
        exit(1);
    }

/*  We can now read/write via sockfd.  */
    printf("send \"%s\" to server\n", message);
    write(sockfd, message, MESSAGE_LENGTH);

    read(sockfd, response, RESPONSE_LENGTH);
    printf("read string from server = \"%s\"\n", response);

    if(strcmp(response, "server died") != 0)
    {
      strcpy(message, "farewell, bond!");
      printf("send %s to server\n", message);
      write(sockfd, message, MESSAGE_LENGTH);

      read(sockfd, response, RESPONSE_LENGTH);
      printf("read char from server = %s\n", response);
    }
    else
    {
      printf("Server terminated.\n");
    }

    close(sockfd);
    exit(0);
}
