#include <string.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

#define RESPONSE_LENGTH 256
#define MESSAGE_LENGTH 256
#define MAX_CLIENTS 10

int main()
{
  int i = 0, max = 0, len = 0, num_clients = 0, activity = 0;
  int server_sockfd, client_sockfd[MAX_CLIENTS];
  int server_len, client_len;
  struct sockaddr_in server_address;
  struct sockaddr_in client_address;
  char ch;
  char message[RESPONSE_LENGTH], response[MESSAGE_LENGTH];
  struct timeval tv;
  strcpy(response, "It's a good day to die!");
  tv.tv_sec = 2;
  tv.tv_usec = 500000;
  fd_set master;
  fd_set readers;

  /*  Remove any old socket and create an unnamed socket for the server.  */
  server_sockfd = socket(AF_INET, SOCK_STREAM, 0);

  /*  Name the socket.  */
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);
  server_address.sin_port = htons(9734);
  server_len = sizeof(server_address);
  bind(server_sockfd, (struct sockaddr *)&server_address, server_len);



  /*  Create a connection queue and wait for clients.  */
  listen(server_sockfd, 5);
  FD_ZERO(&readers);
  FD_ZERO(&master);
  FD_SET(server_sockfd, &master);
  max = server_sockfd;

  while(1)
  {
    readers = master;
    tv.tv_usec = 500000;
    tv.tv_sec = 2;
    if(select(max+1, &readers, NULL, NULL, &tv) == -1)
    {
      perror("select failed.");
    }

    if(FD_ISSET(server_sockfd, &readers))
    {
      /*  Accept a connection.  */
      client_len = sizeof(client_address);
      client_sockfd[num_clients] = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);
      FD_SET(client_sockfd[num_clients], &master);
      if(client_sockfd[num_clients] > max) max = client_sockfd[num_clients];
      num_clients++;
      /*  We can now read/write to client on client_sockfd.  */
    }
    else 
    {
      for(i = 0; i < num_clients; i++)
      {
	if(FD_ISSET(client_sockfd[i], &readers))
	{
	  activity = 1;

	  if((len = read(client_sockfd[i], message, MESSAGE_LENGTH)) <= 0)
	  {
	    if(len < 0)
	      perror("failure during read!");
	    else
	    {
              num_clients--;
	      printf("connection closed.\n");
	      close(client_sockfd[i]);
              FD_CLR(client_sockfd[i], &master);
	    }
	  }
	  else
	  {
            if(strcmp(message, "die") == 0)
            {
	      write(client_sockfd[i], "server died", RESPONSE_LENGTH);
              
	      printf("Received termination signal...\n");
              for(i = 0; i < num_clients; i++)
              {
                close(client_sockfd[i]);
                FD_CLR(client_sockfd[i], &master);  
              }
              
              num_clients = 0;
              exit(0);
            }
            else
            {
	       printf("message from client %d: \"%s\"\n", i, message);

	       printf("send a string to client %d: \"%s\"\n", i, response);
	       write(client_sockfd[i], response, RESPONSE_LENGTH);
            }
	  }
	}
      }

      if(activity == 0) printf("no data...\n");
      activity = 0;
    }
  }
}        


//        read(client_sockfd, &ch, 1);
//	printf("read a character from client %c\n", ch);

//        ch++;
//	printf("send a character to client %c\n", ch);
//        write(client_sockfd, &ch, 1);

//        close(client_sockfd);


