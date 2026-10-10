#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

int main()
{
    // 1. create server
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd == -1)
    {
        perror("socket");
        return 1;
    }

    // 2. set the server address
    sockaddr_in server_addr{};

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    // 3. bind ip and port
    if (bind(server_fd,
             reinterpret_cast<sockaddr*>(&server_addr),
             sizeof(server_addr)) == -1)
    {
        perror("bind");
        close(server_fd);
        return 1;
    }

    // 4. start listening
    if (listen(server_fd, 5) == -1)
    {
        perror("listen");
        close(server_fd);
        return 1;
    }

    std::cout << "Server listening on port 8080..." << std::endl;

    // 5. 等待一个客户端连接
    sockaddr_in client_addr{};
    socklen_t client_len =sizeof(client_addr);

    int client_fd = accept(
        server_fd,
        reinterpret_cast<sockaddr*>(&client_addr),
        &client_len
    );

    if (client_fd == -1)
    {
       perror("accept");
       close(server_fd);
       return 1;
     }

      //6.echo
      char buffer[1024];

      while(true)
      {
      ssize_t bytes_received = recv(client_fd,buffer,sizeof(buffer),0);

      if (bytes_received < 0)
      {
      perror("recv");
      break;
      }

      if (bytes_received == 0)
      {
      std::cout << "Client disconnected." << std::endl;
      break;
      }

      std::cout << "Received:"
                <<std::string(buffer,bytes_received)
                <<std::endl;

     ssize_t bytes_sent = send(client_fd,buffer,bytes_received,0);

     if(bytes_sent < 0 )
     {
      perror("send");
      break;
     }
     }

      //7. close connection
      close(client_fd);
      close(server_fd);

       return 0;

   }
