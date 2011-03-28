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
    int sockfd, len, result, i;
    struct sockaddr_in address;
    char message[MESSAGE_LENGTH], response[RESPONSE_LENGTH], buf[MESSAGE_LENGTH];

    if(argc > 1)
    {
       strcpy(message, "");
       for(i = 1; i < argc-1; i++)
       {
          strcat(message, argv[i]);
          strcat(message, " ");
       }
       strcat(message, argv[argc-1]);
    }

    /* Create a socket for the client. */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    /* Name the socket, as agreed with the server. */
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(54321);
    len = sizeof(address);

/*  Now connect our socket to the server's socket.  */

    result = connect(sockfd, (struct sockaddr *)&address, len);
    if(result == -1) {
        perror("connect failed on client: ");
        exit(1);
    }

    do
    {
       printf("What message would you like to send?\n");
       fgets(buf, MESSAGE_LENGTH, stdin);
       buf[strlen(buf)-1] = '\0';

       strcpy(message, buf);       
       if((len = write(sockfd, message, MESSAGE_LENGTH)) <= 0)
       {
         if(len == 0)
         {
            perror("0 bytes written: ");
         }
         else
         {
            perror("server dead on write: ");
         }
       }
       else if(len != MESSAGE_LENGTH)
       {
         perror("incorrect amount written: ");
       }
       
       /* We expect a response */
       if((len = read(sockfd, response, RESPONSE_LENGTH)) <= 0)
       {
          if(len == 0)
          {
            printf("connection closed.\n");
          }
          else
          {
            perror("server died when read expected: ");
          }
       }

       printf("read string from server = \"%s\"\n", response);

       if(strcmp(message, "die") == 0)
       {
         if(strcmp(response, "server died") == 0)
         {
           printf("Successful termination.\n");
           break;
         }
         else
         {
           perror("error; server should have died");
         }
       }
       else
       {
         if(strcmp(response, "server died") == 0)
         {
           perror("error; server died unexpectedly");
         }
       }

       printf("Send another message? (y/n)\n");
       fgets(buf, MESSAGE_LENGTH, stdin);
       buf[strlen(buf)-1] = '\0';
    }while((strcasecmp(buf, "y") == 0) || (strcasecmp(buf, "yes") == 0));

    close(sockfd);
    exit(0);
}
