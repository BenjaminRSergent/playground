#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>

#define PORT 23001
#define BUFSIZE 2048 

struct sockaddr_in remaddr;                      /* remote address */      
socklen_t addrlen;
int fd;                                          /* our socket */          
int write(const void *buf_in, size_t leng) {
    ssize_t remain = leng;
    const uint8_t *buf = reinterpret_cast<const uint8_t *>(buf_in);

    ssize_t rleng = 0;


    rleng = ::sendto(fd, buf, remain, 0, (struct sockaddr *)&remaddr, addrlen);

    if (rleng <= 0) {
    	std::cout << "Error " << rleng << "\n";
        return rleng;
    }
    
    buf += rleng;
    remain -= rleng;

    usleep(1000);
    return leng - remain;
}

int main(int argc, char **argv) {                  
    struct sockaddr_in myaddr;                       /* our address */         
    addrlen = sizeof(remaddr);             /* length of addresses */ 
    int recvlen;                                     /* # bytes received */    
    
    unsigned char buf[BUFSIZE]; /* receive buffer */ /* create a UDP socket */ 
    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {                           
      perror("cannot create socket\n");                                        
      return 0;                                                                
    } /* bind the socket to any valid IP address and a specific port */      

    memset((char *)&myaddr, 0, sizeof(myaddr));                                
    myaddr.sin_family = AF_INET;                               
    myaddr.sin_addr.s_addr = inet_addr("169.254.1.56");
    myaddr.sin_port = htons(PORT);                        



    if (bind(fd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {            
      perror("bind failed");                                                   
      return 0;                                                                
    } /* now loop, receiving data and printing what we received */             

	printf("waiting on port %d\n", PORT);                                    
    recvlen = recvfrom(fd, buf, BUFSIZE, 0, (struct sockaddr *)&remaddr,     
                     &addrlen);                       
  printf("received %d bytes\n", recvlen);                                  
  if (recvlen > 0) {                                                       
    buf[recvlen] = 0;                                                      
    printf("received message: \"%s\"\n", buf);                             
  }                                 

   fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK);

  char data[72];

  for(int index = 0; index < 72; index++) {
		data[index] = (index+1);
  }

    for (;;) {                                                                 
        write(data, sizeof(data));
    } /* never exits */                                                        
  }