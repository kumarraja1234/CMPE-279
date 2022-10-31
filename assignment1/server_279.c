// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define PORT 8080

int drop_privilages(){
    
    struct passwd* pwd;
    
    pid_t child_Process_ID,Process_id;

    child_Process_ID = fork();

    if(child_Process_ID ==0){
        //success
        printf("\n fork sucessful \n");
        //getting user id with restricted privilages
        pwd = getpwnam("nobody");
/* use correct user ID for nobody user and switch to that user ID */
        Process_id = setuid(pwd->pw_uid);
        if(Process_id==0){
            return 1;
        }

        return 0;

    }

}

int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";

     printf("execve=0x%p\n", execve);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR,
	&opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    if (bind(server_fd, (struct sockaddr *)&address,
	sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                       (socklen_t*)&addrlen))<0)
/*new socket corresponds to the client that connected and writing to new_socket goes out of network to the client */
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    //drop privilages 
    if(drop_privilages()){
         //message processing
            valread = read(new_socket, buffer, 1024);
/*message sent by client will be stored in buffer */
            printf("%s\n",buffer);
            send(new_socket, hello, strlen(hello), 0);
            printf("Hello message sent\n");
         
    }

    wait(NULL);
    return 0;
}