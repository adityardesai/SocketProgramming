    //Acting as a Client basically, so the code below is of the client
    #include "constants.h"
    int linesA;
    char *result[MAXDATASIZE];
    char ipstr[INET6_ADDRSTRLEN];

    int countCD=0,countAB=0,countAC=0,countAD=0,countBC=0,countBD=0,flagA=0,flagB=0,flagC=0,flagD=0;
    void *get_in_addr(struct sockaddr *sa)
    {
      if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
      }
      return &(((struct sockaddr_in6*)sa)->sin6_addr);
    }
    void displayPeerMessage(int sockfd)
    {
      
      struct sockaddr_in peer;
      int peer_len;

      peer_len = sizeof(peer);
      if (getpeername(sockfd, (struct sockaddr *)&peer, &peer_len) == -1) 
      {
      perror("getpeername() failed");
      
      }
      //printf("Peer's IP address is: %s\n", inet_ntoa(peer.sin_addr));
      //printf("Peer's port is: %d\n", (int) ntohs(peer.sin_port));
      printf("\nThe serverC has received the network topology from the Client with UDP port number %d and IP address %s", (int)ntohs(peer.sin_port),inet_ntoa(peer.sin_addr)); 
    }
  char *readfile(char *filename)
  {

      FILE *fp;
      char serverPeerName[MAX],distance[MAX],message[MAXDATASIZE];
      char *identifier;
      identifier="*";

      fp=fopen(filename,"r");
      if(fp == NULL) {
        fprintf(stderr, "\n Can't open input file !\n");
          exit(1);
      }

      if((strcmp(filename,"serverC.txt")) == 0)
      {
        printf("\n The Server C is up and running\n The Server C has the following neighbor information:");
        strcat(message,"C");
        strcat(message,"+");
        fscanf(fp,"%s %s",serverPeerName,distance);
        // printf(" %s   %s\n",serverPeerName,distance);
        while(!feof(fp))
        {
          strcat(message,identifier);
          strcat(message,serverPeerName);
          strcat(message,identifier);
          strcat(message,distance);
          strcat(message,identifier);
          strcat(message,"+");
          printf(" %s   %s\n",serverPeerName,distance);
          fscanf(fp,"%s %s",serverPeerName,distance);
        }
         printf(" %s   %s\n",serverPeerName,distance);
       }//end A
      fclose(fp);
      return message;
    }
void printAdjMatrix(char message[MAXDATASIZE])
  {
    int i,j;
    //char temp[100]="0110101111000100";
    char temp[20][20]={0};
    printf("Adjaceny matrix for the Network is as follows\n");
    printf("\nServer\tA\tB\tC\tD\n");
    for(i=0;i<16;i++)
    {

      for(j=0;j<4;j++)
      temp[i][j] = message[4*i + j];
    }
    for(i=0;i<4;i++)
    {
      if(i==0)printf("A\t");
      if(i==1)printf("B\t");
      if(i==2)printf("C\t");
      if(i==3)printf("D\t");
      for(j=0;j<4;j++)
      printf("%c\t",temp[i][j]);
      printf("\n");
   }
  }
void checkData(char message[MAXDATASIZE])
{
        int j=0,i=0;
        char *line;
        char *line_delim = "+";
        char *line_save;
    
        char *word;
        char *word_delim = "*";
        char *word_save;
        

       // printf("\n Message received in checkData %s\n",message);
      line = strtok_r(message, line_delim, &line_save);
  while (line) 
  {
  //printf("LINE IS %s\n", line);
    word = strtok_r(line, word_delim, &word_save);
    while (word) 
            {
                //printf("WORD IS %s\n", word);
                result[i++]=word;
                word = strtok_r(NULL, word_delim, &word_save);
            }//end word
            //printf("\n");
            line = strtok_r(NULL, line_delim, &line_save); 
  }//end line
      linesA=i;
}
void formatUDPMessage(char message[MAXDATASIZE])
    {
       int i,j=0;
      int Aindex,Bindex,Cindex,Dindex;
      int AindexP,BindexP,CindexP,DindexP;
      char *token;

      char *A ="A";
      char *B ="B";
      char *C ="C";
      char *D ="D";
      const char s[2] = "+";
  
      checkData(message);

      for(i=0;i<linesA;i++)
      {
      if(strcmp("A",result[i]) == 0 ) {/*printf("\n  A is %d\n",i);*/ Aindex = i; AindexP=0;}
      if(strcmp("B",result[i]) == 0 ) {/*printf("\n  B is %d\n",i);*/ Bindex = i; BindexP=i-1;}
      if(strcmp("C",result[i]) == 0 ) {/*printf("\n  C is %d\n",i);*/ Cindex = i; CindexP=i-1;}
      if(strcmp("D",result[i]) == 0 ) {/*printf("\n  D is %d\n",i);*/ Dindex = i; DindexP=i-1;}
      }
      //printf("\n Serv B is between %d %d\n",BindexP,CindexP);
      //printf("\n Serv C is between %d %d\n",CindexP,DindexP);
      //printf("\n Serv D is between %d %d\n",DindexP,i-1);
      printf("\n");
      //printf("Server A has following neighbor information \n");
      printf("\n Server A::");
      for(j=Aindex+1;j<=BindexP;j++)
      {
      printf(" %s ", result[j]);
      }
      printf("\n Server B::");
      for(j=Bindex+1;j<=CindexP;j++)
      {
      printf(" %s ", result[j]);
      }
      printf("\n Server C::");
      for(j=Cindex+1;j<=DindexP;j++)
      {
      printf(" %s ", result[j]);
      }
      printf("\n Server D::");
      for(j=Dindex+1;j<linesA;j++)
      {
      printf(" %s ", result[j]);
      }
      printf("\n");
    }

 void displayMessage(int sockfd,int check)
    {

      if(check == 1)
      {  
        int z;
        struct sockaddr_in tempClientPortDetails;
        socklen_t length;
        length=sizeof(tempClientPortDetails);
        z=getsockname ( sockfd, (struct sockaddr *)&tempClientPortDetails, &length);  
        if(z == -1)
        {
          perror("getSockName serverC");
        } 
        printf("\nFor this connection with the Client,the Server C has TCP port number %d and IP address %s \n",ntohs(tempClientPortDetails.sin_port),ipstr);
      }
      if(check == 2)
      {  
        int z;
        struct sockaddr_in tempClientPortDetails;
        socklen_t length;
        length=sizeof(tempClientPortDetails);
        z=getsockname ( sockfd, (struct sockaddr *)&tempClientPortDetails, &length);  
        if(z == -1)
        {
          perror("getSockName serverUDPC");
        } 
        printf("\nFor this connection with the Client,the Server C has UDP port number %d and IP address %s \n",ntohs(tempClientPortDetails.sin_port),ipstr);
      }
     } 

    int main(int argc, char *argv[])
    {
      
      //TCP Declerations
      int sockfd, numbytes;
      char buf[MAXDATASIZE],msg[MAXDATASIZE];
      struct addrinfo hints, *servinfo, *p;
      int rv;
      char s[INET6_ADDRSTRLEN];
      char *message,sendMessage[MAXDATASIZE];
      char filename1[MAX] = "serverC.txt";

      //UDP Program Declerations
      int sockfdUDP;
      struct addrinfo hintsUDP, *servinfoUDP, *pUDP;
      int rvUDP;
      int numbytesUDP;
      struct sockaddr_storage their_addrUDP;
      socklen_t addr_lenUDP;
      memset(&hints, 0, sizeof hints);
      hintsUDP.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
      hintsUDP.ai_socktype = SOCK_DGRAM;
      hintsUDP.ai_flags = AI_PASSIVE; // use my IP


      struct sockaddr_in *test = (struct sockaddr_in *)malloc(sizeof (struct sockaddr_in));

      strcpy(msg,"Sai Baba");
      /*if (argc != 2) {
        fprintf(stderr,"usage: client hostname\n");
        exit(1);
      }*/

      //TCP variables data are set to zero 
      memset(&hints, 0, sizeof hints);
      hints.ai_family = AF_UNSPEC;
      hints.ai_socktype = SOCK_STREAM;
      
      //Function to read the file contents and send it to the client over TCP/*
      message=readfile(filename1);
      strcpy(sendMessage,message);
      //printf("TEST MESSAGE and Sending Message : %s %s \n",message,sendMessage);

      //Get the address details for TCP
      if ((rv = getaddrinfo("localhost", PORTCLTCP, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
      }
     
        //Search the linked list for the TCP address details obtained from the previous step
        for(p = servinfo; p != NULL; p = p->ai_next) 
        {

          /*IP Address Details 13th Nov 2015*/
        void *addr; char *ipver;

        // get the pointer to the address itself,
        // different fields in IPv4 and IPv6:
        if (p->ai_family == AF_INET) { // IPv4
        struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;  
        addr = &(ipv4->sin_addr);
        ipver = "IPv4";
        } 
        else 

          { // IPv6
        struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;  
        addr = &(ipv6->sin6_addr);
        ipver = "IPv6";
        }

        // convert the IP to a string and print it:
        inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
        printf(" The ServerC finishes sending its neighbor information to Client with TCP Port number %s and IP Address %s:\n",PORTCLTCP,ipstr);
        
          if ((sockfd = socket(p->ai_family, p->ai_socktype,
            p->ai_protocol)) == -1) 
          {
            perror("clientTCPC: socket");
            continue;
          }
          if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1)
          {
          close(sockfd);
          perror("clientTCPC: connect");
          continue;
          }
            break;
        }//end for p=servinfo

        if (p == NULL) {
          fprintf(stderr, "client clientTCPC: failed to connect\n");
          return 2;
        }

          displayMessage(sockfd,1);   
      //TCP Client Connections    
      inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),s, sizeof s);
      //printf("TCP clientC: connecting to %s\n", s);
      freeaddrinfo(servinfo); //free the node

      //send the TCP Message to the TCP Client . This message has details about the neighbours
      numbytes=send(sockfd,sendMessage,sizeof(sendMessage), 0); 
     // printf("\n %d bytes sent to server",numbytes);
      close(sockfd);//close the socket*/


        //Nowonwards this machine will act as a UDP and listenes to UDP port
        //Get the address details for UDP
        if ((rvUDP = getaddrinfo(NULL, UDPPORTC, &hintsUDP, &servinfoUDP)) != 0) {
        fprintf(stderr, "getaddrinfoUDPC: %s\n", gai_strerror(rvUDP));
        return 1;
        }

        // loop through all the results obtained in the previous step for UDP connection
        for(pUDP = servinfoUDP; pUDP != NULL; pUDP = pUDP->ai_next) {
          if ((sockfdUDP = socket(pUDP->ai_family, pUDP->ai_socktype,
              pUDP->ai_protocol)) == -1) {
            perror("UDP ListenerC: socket");
            continue;
          }
          if (bind(sockfdUDP, pUDP->ai_addr, pUDP->ai_addrlen) == -1) {
            close(sockfdUDP);
            perror("UDP ListenerC: bind");
            continue;
          }
          break;
        }//end for p=servinfoUDP
        if (pUDP == NULL) {
          fprintf(stderr, "UDP ListenerC: failed to bind socket\n");
          return 2;
        }
        

        //printf("listenerB: waiting to recvfrom...\n");
        addr_lenUDP = sizeof their_addrUDP;
          /*printf("\n addrinfo strucutre has the following details\n");
          printf("\n Aai_family : %d", p->ai_family);
          printf("\n Aai_socktype : %d", p->ai_socktype);
          printf("\n Aai_addr : %s",(struct sockaddr *) (p->ai_addr)->sa_data);
          printf("\n Aai_addrlen : %d", p->ai_addrlen);*/

      
        while(1){
        if ((numbytes = recvfrom(sockfdUDP, buf, MAXDATASIZE-1 , 0,
        (struct sockaddr *)&their_addrUDP, &addr_lenUDP)) == -1) {
        perror("recvfrom");
        exit(1);
        }//end numbytes

        /*printf("UDP ListenerC: got packet from %s\n",
        inet_ntop(their_addrUDP.ss_family,
          get_in_addr((struct sockaddr *)&their_addrUDP),
          s, sizeof s));*/

        //printf("UDP ListenerC: packet is %d bytes long\n", numbytes);
        buf[numbytes] = '\0';
        //printf("UDP ListenerC: packet contains \"%s\"\n", buf);
        printf("The serverC has received the network topology from the Client\n");
        printf("\n Edge------------Cost");
        formatUDPMessage(buf);
        test=(struct sockaddr_in *)&their_addrUDP;
        
        printf("\nThe serverC has received the network topology from the Client with UDP port number %d and IP address %s", ntohs(test->sin_port),ipstr); 
        //displayPeerMessage(sockfdUDP);
        displayMessage(sockfdUDP,2);


        if ((numbytes = recvfrom(sockfdUDP, buf, MAXDATASIZE-1 , 0,
        (struct sockaddr *)&their_addrUDP, &addr_lenUDP)) == -1) {
        perror("recvfrom");
        exit(1);
        }//end numbytes
        buf[numbytes] = '\0';
        //printf("UDP ListenerA: Matrix \"%s\"\n", buf);
        printAdjMatrix(buf);
      }//end while
      freeaddrinfo(servinfoUDP);
      free(test);
    }

