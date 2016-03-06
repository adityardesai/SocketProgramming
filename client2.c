//The same file as client with UDP B blocked, the serverB will not receive any data

  #include "constants.h"
  #define NODES 4
  #define INFINITE 9999
  
  
  
  char *result[MAXDATASIZE],*topology[MAXDATASIZE];
  char AdjMatrix[50];
  char AdjMatrixA[50];
  char AdjMatrixB[50];
  char AdjMatrixC[50];
  char AdjMatrixD[50];
  int linesA,linesB;
  int sendingUDPMessage[NODES][NODES];
  char sendingUDPListMessage[MAXDATASIZE];
  char sendMSGAPRINT[MAXDATASIZE];char sendMSGBPRINT[MAXDATASIZE];
  char sendMSGCPRINT[MAXDATASIZE];char sendMSGDPRINT[MAXDATASIZE];
  char sendMSGA[MAXDATASIZE];
  char sendMSGB[MAXDATASIZE];
  char sendMSGC[MAXDATASIZE];
  char sendMSGD[MAXDATASIZE];
  char sendingUDPMessageCLONEFORPRINT[MAXDATASIZE];
  char sendingUDPMessageCLONEFORSEND[MAXDATASIZE];
  char ipstr[INET6_ADDRSTRLEN];
  //char *displayOutputServ[10];
  //char *displayOutputdist[10];
  int costMatrix[NODES][NODES];
  struct network *pListA,*pListB,*pListC,*pListD;
  int mCost=0;
  //int countCD=0,countAB=0,countAC=0,countAD=0,countBC=0,countBD=0,flagA=0,flagB=0,flagC=0,flagD=0;
        
void sigchld_handler(int s)
{
          // waitpid() might overwrite errno, so we save and restore it:
          int saved_errno = errno;
          while(waitpid(-1, NULL, WNOHANG) > 0);
          errno = saved_errno;
}

     // get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
          if (sa->sa_family == AF_INET) {
            return &(((struct sockaddr_in*)sa)->sin_addr);
          }
          return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
        //Linked List Operation Modules
void display(struct network *q)
{
        int count=0;

        //printf("\n Linked List Data is as follows");
        if(q == NULL)
        {
          //printf("\nLinked List is empty in Display");
        }
        else
        {
         //printf("\n The Server %s has the following neighbor information: ",q->hostname);
          printf("\n Neighbor\tCost");
          while(q!=NULL)
          {
          printf("\n %s\t%d",q->peername,q->distance);
          q=q->next;
          count++;
        }
        }
        // printf("\n Total number of lines read from the file for Server %s  %d", q->hostname,count);
}//End Display Module
   // Init list module
void initlist(struct network **q,char serverHostName[5],char *serverPeer,int distance)
{
      struct network *temp,*newnode;
      temp=*q;

      char serverPeerName[MAXDATASIZE];
      strcpy(serverPeerName,serverPeer);
      if(*q == NULL)
      {
        //printf("\n List is empty, adding the elements for the first time");
        newnode = (struct network *) malloc(sizeof(struct network *));
        newnode->distance=distance;
        strcpy(newnode->peername,serverPeerName);
        strcpy(newnode->hostname,serverHostName);
        newnode->next = NULL;

      *q=newnode;
      }
      else
      {
      //printf("\n List is not empty");
      while(temp->next!=NULL) temp = temp->next;

      newnode = (struct network *) malloc(sizeof(struct network *));
      newnode->distance=distance;
      strcpy(newnode->peername,serverPeerName);
      strcpy(newnode->hostname,serverHostName);
      newnode->next = NULL;

      temp->next=newnode;
      
      }//end else
}//end init list
int sendAdjMatrix()
{
      //printf("ENTERING SENDING MESSAGE\n");
      char sendingUDPString[MAXDATASIZE],X[20];
      int i,j,n;
      for(i=0;i<=3;i++)
      {
        for(j=0;j<=3;j++)
        {
      // printf("ENTERING SENDING MESSAGE FOR LOOP\n");
          n=sprintf(X,"%d",sendingUDPMessage[i][j]);
          //printf("%d\n",sendingUDPMessage[i][j] );
          //printf("%s %d \n",X,n );
           strcat(AdjMatrix,X);
           //printf("%s \n",temp );
            }
          }
        // printf("\n Sending Matrix is  %s\n",AdjMatrix);
          //printf("\n OUTSIDE");
          return 1;
}
int FindMinCost(int key[], int mstSet[])
{
  int min = INFINITE, min_index,v;
  for (v= 0; v < NODES; v++)
    if (mstSet[v] == 0 && key[v] < min)
    min = key[v], min_index = v;
    return min_index;
}

void printMatrix()
{
         // printf("Inside double Matrix %d ",**p);
          printf("\n Adj Matrix at Client\n");
          printf("\nServer\tA\tB\tC\tD\n");
          int row,col;
          for(row=0;row<4;row++)
          { 
            if(row==0)printf("A\t");
            if(row==1)printf("B\t");
            if(row==2)printf("C\t");
            if(row==3)printf("D\t");
            for(col=0;col<4;col++)
              printf("%d\t",sendingUDPMessage[row][col]);
            printf("\n");
          }
}
void checkTopology(char message[MAXDATASIZE])
    {
        int j=0,i=0;
        char *line;
        char *line_delim = "+";
        char *line_save;
    
        char *word;
        char *word_delim = "*";
        char *word_save;
        
        line = strtok_r(message, line_delim, &line_save);
        while (line) 
        {
            word = strtok_r(line, word_delim, &word_save);
            while (word) {
                //printf("WORD IS %s\n", word);
                topology[i++]=word;
                word = strtok_r(NULL, word_delim, &word_save);
            }//end word
            //printf("\n");
            line = strtok_r(NULL, line_delim, &line_save); 
        }//end line
        linesB=i;
}
  
void processData()
{
          int i=0,j=0,row=0,col=0,k;
          char *distance,*peerName,AString;
          int dist,recvCnt=0;

          pListA=NULL;
          pListB=NULL;
          pListC=NULL;
          pListD=NULL;

          
          sendingUDPMessage[0][0] = 0;
          sendingUDPMessage[1][1] = 0;
          sendingUDPMessage[2][2] = 0;
          sendingUDPMessage[3][3] = 0;

          if(strcmp(result[0],"A") == 0) {
            //printf("%s\n","A Server Details" );
            strcat(sendingUDPListMessage,"A");
            strcat(sendingUDPListMessage,"*");
            for(i=1,j=2;i<=linesA-1,j<=linesA-1;)
            {
            //printf("RES %s  %s\n",result[i],result[j]);
            peerName=result[i];
            distance=result[j];
            dist=atoi(distance);
            i=i+2;j=j+2;

            costMatrix[0][0]=0;
            if(strcmp(peerName,"serverB") == 0) { sendingUDPMessage[0][1] = 1;costMatrix[0][1]=dist;}
            if(strcmp(peerName,"serverC") == 0) { sendingUDPMessage[0][2] = 1;costMatrix[0][2]=dist;}
            if(strcmp(peerName,"serverD") == 0) { sendingUDPMessage[0][3] = 1;costMatrix[0][3]=dist;}
            initlist(&pListA,"A",peerName,dist);

            
            strcat(sendingUDPListMessage,peerName);
            strcat(sendingUDPListMessage,"*");
            strcat(sendingUDPListMessage,distance);
            strcat(sendingUDPListMessage,"*");
            //printf("PEERNAME %s DISTANCE %d\n",peerName,dist);
            }
            display(pListA);
          }//if A end
          if(strcmp(result[0],"B") == 0) {
            //printf("%s\n","B Server Details" );
            strcat(sendingUDPListMessage,"B");
            strcat(sendingUDPListMessage,"*");
            for(i=1,j=2;i<=linesA-1,j<=linesA-1;)
            {
            //printf("RES %s  %s\n",result[i],result[j]);
            peerName=result[i];
            distance=result[j];
            dist=atoi(distance);
            i=i+2;j=j+2;

            //recvCnt=recvCnt+1;
            
            costMatrix[1][1]=0;
            if(strcmp(peerName,"serverA") == 0) {sendingUDPMessage[1][0] = 1;costMatrix[1][0]=dist;}
            if(strcmp(peerName,"serverC") == 0) {sendingUDPMessage[1][2] = 1;costMatrix[1][2]=dist;}
            if(strcmp(peerName,"serverD") == 0) {sendingUDPMessage[1][3] = 1;costMatrix[1][3]=dist;}
            initlist(&pListB,"B",peerName,dist);
            //printf("PEERNAME %s DISTANCE %d\n",peerName,dist);
            
            strcat(sendingUDPListMessage,peerName);
            strcat(sendingUDPListMessage,"*");
            strcat(sendingUDPListMessage,distance);
            strcat(sendingUDPListMessage,"*");
            }
          display(pListB);
          }//if B end
          if(strcmp(result[0],"C") == 0) {
            //printf("%s\n","C Server Details" );
            strcat(sendingUDPListMessage,"C");
            strcat(sendingUDPListMessage,"*");
            for(i=1,j=2;i<=linesA-1,j<=linesA-1;)
            {
            //printf("RES %s  %s\n",result[i],result[j]);
            peerName=result[i];
            distance=result[j];
            dist=atoi(distance);
            i=i+2;j=j+2;
           
            costMatrix[2][2]=0;
            if(strcmp(peerName,"serverA") == 0) {sendingUDPMessage[2][0] = 1;costMatrix[2][0]=dist;}
            if(strcmp(peerName,"serverB") == 0) {sendingUDPMessage[2][1] = 1;costMatrix[2][1]=dist;}
            if(strcmp(peerName,"serverD") == 0) {sendingUDPMessage[2][3] = 1;costMatrix[2][3]=dist;}
            initlist(&pListC,"C",peerName,dist);
            //printf("PEERNAME %s DISTANCE %d\n",peerName,dist);
          
            strcat(sendingUDPListMessage,peerName);
            strcat(sendingUDPListMessage,"*");
            strcat(sendingUDPListMessage,distance);
            strcat(sendingUDPListMessage,"*");
            }
          display(pListC);
          }//if C
          if(strcmp(result[0],"D") == 0) {
            printf("%s\n","D Server Details" );
            strcat(sendingUDPListMessage,"D");
            strcat(sendingUDPListMessage,"*");
            for(i=1,j=2;i<=linesA-1,j<=linesA-1;)
            {
            //printf("RES %s  %s\n",result[i],result[j]);
            peerName=result[i];
            distance=result[j];
            dist=atoi(distance);
            i=i+2;j=j+2;
           
            costMatrix[3][3]=0;
            if(strcmp(peerName,"serverA") == 0) {sendingUDPMessage[3][0] = 1;costMatrix[3][0]=dist;}
            if(strcmp(peerName,"serverB") == 0) {sendingUDPMessage[3][1] = 1;costMatrix[3][1]=dist;}
            if(strcmp(peerName,"serverC") == 0) {sendingUDPMessage[3][2] = 1;costMatrix[3][2]=dist;}
            initlist(&pListD,"D",peerName,dist);
            //printf("PEERNAME %s DISTANCE %d\n",peerName,dist);

            strcat(sendingUDPListMessage,peerName);
            strcat(sendingUDPListMessage,"*");
            strcat(sendingUDPListMessage,distance);
            strcat(sendingUDPListMessage,"*");
            }
          display(pListD);
          }//if D
}

void displayPeerMessage(int sockfd,char Name)
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
      printf("\nThe Client receives neighbor information from the Server %c with TCP portnumber %d and IP address %s\n",Name,(int)ntohs(peer.sin_port),inet_ntoa(peer.sin_addr)); 

}
void checkData(char message[MAXDATASIZE])
{
          int j=0,i=0;
          char *resultB[MAXDATASIZE],*resultC[MAXDATASIZE],*resultD[MAXDATASIZE];

          char *line;
          char *line_delim = "+";
          char *line_save;
         
          char *word;
          char *word_delim = "*";
          char *word_save;
         // printf("\n Message received in checkData %s\n",message);

          line = strtok_r(message, line_delim, &line_save);
            while (line) {
                //printf("LINE IS %s\n", line);
         
                word = strtok_r(line, word_delim, &word_save);
                while (word) {
                    //printf("WORD IS %s\n", word);
                    result[i++]=word;
                    word = strtok_r(NULL, word_delim, &word_save);
                }//end word
                //printf("\n");
                line = strtok_r(NULL, line_delim, &line_save); 
            }//end line
            linesA=i;
            /*printf("I Value %d\n", i);
            for(j=0;j<i;j++)
            printf("RESULT IS %s \n",result[j]);*/
            //end A server details
}
void displayAllUDPMessages(char sendingUDPListMessage[MAXDATASIZE])
{
  int i,j,servCount=0,k;
    checkTopology(sendingUDPListMessage);
    int Aindex,Bindex,Cindex,Dindex;
    int AindexP=0,BindexP,CindexP,DindexP;
    char *token;

    char *A ="A";
    char *B ="B";
    char *C ="C";
    char *D ="D";
    const char s[2] = "*";
    
      for(i=0;i<linesB;i++)
      {

      if(strcmp("A",topology[i]) == 0 ) {Aindex = i; AindexP=0;}
      if(strcmp("B",topology[i]) == 0 ) {Bindex = i; BindexP=i-1;}
      if(strcmp("C",topology[i]) == 0 ) {Cindex = i; CindexP=i-1;}
      if(strcmp("D",topology[i]) == 0 ) {Dindex = i; DindexP=i-1;}
      }
      printf("\n Server A::");
      for(j=Aindex+1;j<=BindexP;j++)
      {printf(" %s ", topology[j]);}
      printf("\n Server B::");
      for(j=Bindex+1;j<=CindexP;j++)
      {printf(" %s ", topology[j]);}
      printf("\n Server C::");
      for(j=Cindex+1;j<=DindexP;j++)
      {printf(" %s ", topology[j]);}
      printf("\n Server D::");
      for(j=Dindex+1;j<linesB;j++)
      {printf(" %s ", topology[j]);}
      printf("\n");
}
  /*void sendUDPList()
      {
        struct network *tempA,*tempB,*tempC,*tempD;
        tempA=pListA;tempB=pListB;tempC=pListC;tempD=pListD;
        int n; 
        char distString[20];

        if(tempA == NULL )
        {
          printf("\n A list is empty and cannot form UDP message to send");
        }
        if(tempB == NULL )
        {
          printf("\n B list is empty and cannot form UDP message to send");
        }
        if(tempC == NULL )
        {
          printf("\n C list is empty and cannot form UDP message to send");
        }
        if(tempD == NULL )
        {
          printf("\n D list is empty and cannot form UDP message to send");
        }
        else
        {
          while(tempA->next!=NULL)
          {
            strcat(sendingUDPListMessage,tempA->hostname);
            strcat(sendingUDPListMessage,tempA->peername);
            n=sprintf(distString,"%d",tempA->distance);
            strcat(sendingUDPListMessage,distString);
            tempA=tempA->next;
          }//end While ListA

          while(tempB->next!=NULL)
          {
            strcat(sendingUDPListMessage,tempB->hostname);
            strcat(sendingUDPListMessage,tempB->peername);
            n=sprintf(distString,"%d",tempB->distance);
            strcat(sendingUDPListMessage,distString);
            tempB=tempB->next;
          }//end While ListB

          while(tempC->next!=NULL)
          {
            strcat(sendingUDPListMessage,tempC->hostname);
            strcat(sendingUDPListMessage,tempC->peername);
            n=sprintf(distString,"%d",tempC->distance);
            strcat(sendingUDPListMessage,distString);
            tempC=tempC->next;
          }//end While ListC
          while(tempD->next!=NULL)
          {
            strcat(sendingUDPListMessage,tempD->hostname);
            strcat(sendingUDPListMessage,tempD->peername);
            n=sprintf(distString,"%d",tempD->distance);
            strcat(sendingUDPListMessage,distString);
            tempD=tempD->next;
          }//end While ListD
        }//end else loop
        printf("\n Sending UDP List : %s",sendingUDPListMessage);
  }*/
void displayMessage(int sockfd,char Name)
{

  int z;
  struct sockaddr_in tempClientPortDetails;
  socklen_t length;
  length=sizeof(tempClientPortDetails);
  z=getsockname ( sockfd, (struct sockaddr *)&tempClientPortDetails, &length);  
  if(z == -1)
  {
    perror("getSockName serverUDPA");
  } 
  printf("\nFor this connection with the Server %c ,the Client has UDP port number %d and IP address %s \n",Name, ntohs(tempClientPortDetails.sin_port),ipstr);
}


void CheckMST(int Predecessor[], int n, int Matrix[NODES][NODES])
{
  int i;
  printf("\nSourceServer Node and DestinationServer MST is as follows \n");
  printf("0 indicates ServerA, 1 indicates ServerB\n2 indicates ServerC 3 indicates ServerD\n");
  for (i = 1; i < NODES; i++)
  {
    mCost=Matrix[i][Predecessor[i]]+mCost;
    printf("Edge between Server Node %d and Server Node %d with the cost %d \n", Predecessor[i], i, Matrix[i][Predecessor[i]]);
    //printf("%d - %d\n", Predecessor[i], i);
  }
  printf("The client has calculated the tree and the cose is %d\n",mCost);
}
void printMST()
{
  int Predecessor[NODES];
  int key[NODES]; 
  int mstSet[NODES]; 
  int count=0,i=0,v;
  int Matrix[NODES][NODES]={0};
  int j,u;

  //copy the Adj Matrix to the local copy of Matrix
  for(i=0;i<NODES;i++)
    for(j=0;j<NODES;j++)
      Matrix[i][j]=costMatrix[i][j];

  for (i = 0; i < NODES; i++)
    key[i] = INFINITE, mstSet[i] = 0;


  key[0] = 0;  
  Predecessor[0] = -1; 

  for (count = 0; count < NODES-1; count++)
  {
    u = FindMinCost(key, mstSet);
    mstSet[u] = 1;
    for (v = 0; v < NODES; v++)
    if (Matrix[u][v] && mstSet[v] == 0 && Matrix[u][v] < key[v])
      Predecessor[v] = u, key[v] = Matrix[u][v];
  }
  CheckMST(Predecessor, NODES, Matrix);
}

int main(void)
{
        
          //TCP variables
  int sockfd, new_fd,sendCheck=0; // listen on sock_fd, new connection on new_fd
  struct addrinfo hints, *servinfo, *p;
  struct sockaddr_storage their_addr; // connector's address information
  socklen_t sin_size;
  struct sigaction sa;
  char s[INET6_ADDRSTRLEN],buf[MAXDATASIZE];
  int rv,recieveCount=0;

  //UDP Variables/*
  int sockfdUDPA;
  struct addrinfo hintsUDPA,*servinfoUDPA,*pUDPA;
  int rvUDPA;
  char messageUDPA[MAXDATASIZE];
   
  int sockfdUDPB;
  struct addrinfo hintsUDPB,*servinfoUDPB,*pUDPB;
  int rvUDPB;
  char messageUDPB[MAXDATASIZE];

  int sockfdUDPC;
  struct addrinfo hintsUDPC,*servinfoUDPC,*pUDPC,*pC;
  int rvUDPC;
  char messageUDPC[MAXDATASIZE];
  
  int sockfdUDPD;
  struct addrinfo hintsUDPD,*servinfoUDPD,*pUDPD,*pD;
  int rvUDPD;
  char messageUDPD[MAXDATASIZE];
 
  //Common Variables
  int numbytes,yes,serverNameCheck,i;
  void *addr; char *ipver;
  int sendCount=0;

  //Setting 0s for TCP Socket
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE; // use my IP
       
  //Setting )x for UDP Sockets
  memset(&hintsUDPA, 0, sizeof hintsUDPA);
  hintsUDPA.ai_family = AF_UNSPEC;
  hintsUDPA.ai_family = AF_UNSPEC;
  hintsUDPA.ai_socktype = SOCK_DGRAM;
  hintsUDPA.ai_flags = AI_PASSIVE;

  memset(&hintsUDPB, 0, sizeof hintsUDPB);
  hintsUDPB.ai_family = AF_UNSPEC;
  hintsUDPB.ai_socktype = SOCK_DGRAM;
  hintsUDPB.ai_flags = AI_PASSIVE;

  memset(&hintsUDPC, 0, sizeof hintsUDPC);
  hintsUDPC.ai_family = AF_UNSPEC;
  hintsUDPC.ai_socktype = SOCK_DGRAM;
  hintsUDPC.ai_flags = AI_PASSIVE;

  memset(&hintsUDPD, 0, sizeof hintsUDPD);
  hintsUDPD.ai_family = AF_UNSPEC;
  hintsUDPD.ai_socktype = SOCK_DGRAM;
  hintsUDPD.ai_flags = AI_PASSIVE;

  strcpy(messageUDPA,"Sri Sai Baba");

  //TCP Port details
  if ((rv = getaddrinfo("localhost", PORTCLTCP, &hints, &servinfo)) != 0) {
            fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
            return 1;
           }
  // loop through all the results and bind to the first we can
  for(p = servinfo; p != NULL; p = p->ai_next) {

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
        printf("The Client with TCP Port number %s and IP Address %s:\n",PORTCLTCP,ipstr);
      if ((sockfd = socket(p->ai_family, p->ai_socktype,
              p->ai_protocol)) == -1) {
              perror("TCP Client: socket");
           continue;
            }
              
      if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("TCP Client: bind");
           continue;
          }
          break;
          }
      if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
              sizeof(int)) == -1) {
              perror("setsockopt TCP");
              exit(1);
          }
  
      if (p == NULL) {
            fprintf(stderr, "TCP Client: failed to bind\n");
            exit(1);
          }
      if (listen(sockfd, BACKLOG) == -1) {
            perror("listen TCP");
            exit(1);
        }
       
      sa.sa_handler = sigchld_handler; // reap all dead processes
          sigemptyset(&sa.sa_mask);
          sa.sa_flags = SA_RESTART;
      if (sigaction(SIGCHLD, &sa, NULL) == -1) {
            perror("sigaction");
            exit(1);
       }
          //printf("TCP Client: waiting for connections...\n");

            //UDP Operations
      if ((rvUDPA = getaddrinfo("localhost", UDPPORTA, &hintsUDPA, &servinfoUDPA)) != 0) {
          fprintf(stderr, "getaddrinfoA: %s\n", gai_strerror(rvUDPA));
           return 1;
            }

      if ((rvUDPB = getaddrinfo("localhost", UDPPORTB, &hintsUDPB, &servinfoUDPB)) != 0) {
            fprintf(stderr, "getaddrinfoB: %s\n", gai_strerror(rvUDPB));
            return 1;
            }

      if ((rvUDPC = getaddrinfo("localhost", UDPPORTC, &hintsUDPC, &servinfoUDPC)) != 0) {
            fprintf(stderr, "getaddrinfoC: %s\n", gai_strerror(rvUDPC));
            return 1;
            }
      if ((rvUDPD = getaddrinfo("localhost", UDPPORTD, &hintsUDPD, &servinfoUDPD)) != 0) {
            fprintf(stderr, "getaddrinfoD: %s\n", gai_strerror(rvUDPD));
            return 1;
            }

       for(pUDPA = servinfoUDPA; pUDPA != NULL; pUDPA = pUDPA->ai_next) {
              if ((sockfdUDPA = socket(pUDPA->ai_family, pUDPA->ai_socktype,
              pUDPA->ai_protocol)) == -1) {
            perror("Talker UDPA Client: socket");
            continue;
            }
            break;
          }// end for loop A
       /*   printf("\n addrinfo strucutre has the following details\n");
          printf("\n Aai_family : %d", pUDPA->ai_family);
          printf("\n Aai_socktype : %d", pUDPA->ai_socktype);
          printf("\n Aai_addr : %c",(struct sockaddr *) (pUDPA->ai_addr)->sa_data);
          printf("\n Aai_addrlen : %d", pUDPA->ai_addrlen);*/

        for(pUDPB = servinfoUDPB; pUDPB != NULL; pUDPB = pUDPB->ai_next) {
            if ((sockfdUDPB = socket(pUDPB->ai_family, pUDPB->ai_socktype,
              pUDPB->ai_protocol)) == -1) {
            perror("Talker UDPB Client: socket");
            continue;
            }
            break;
          }// end for loop B
/*          printf("\n addrinfo strucutre has the following details\n");
          printf("\n Aai_family : %d", pUDPB->ai_family);
          printf("\n Aai_socktype : %d", pUDPB->ai_socktype);
          printf("\n Aai_addr : %s",(struct sockaddr *) (pUDPB->ai_addr)->sa_data);
          printf("\n Aai_addrlen : %d", pUDPB->ai_addrlen);

*/        for(pUDPC = servinfoUDPC; pUDPC != NULL; pUDPC = pUDPC->ai_next) {
            if ((sockfdUDPC = socket(pUDPC->ai_family, pUDPC->ai_socktype,
              pUDPC->ai_protocol)) == -1) {
            perror("Talker UDPC Client: socket");
            continue;
            }
            break;
          }// end for loop C
/*          printf("\n addrinfo strucutre has the following details\n");
          printf("\n Aai_family : %d", pUDPC->ai_family);
          printf("\n Aai_socktype : %d", pUDPC->ai_socktype);
          printf("\n Aai_addr : %s",(struct sockaddr *) (pUDPC->ai_addr)->sa_data);
          printf("\n Aai_addrlen : %d", pUDPC->ai_addrlen);
*/
        for(pUDPD = servinfoUDPD; pUDPD != NULL; pUDPD = pUDPD->ai_next) {
            if ((sockfdUDPD = socket(pUDPD->ai_family, pUDPD->ai_socktype,
              pUDPD->ai_protocol)) == -1) {
            perror("Talker UDPD Client: socket");
            continue;
            }
            break;
          }// end for loop D
/*          printf("\n addrinfo strucutre has the following details\n");
          printf("\n Aai_family : %d", pUDPD->ai_family);
          printf("\n Aai_socktype : %d", pUDPD->ai_socktype);
          printf("\n Aai_addr : %c",(struct sockaddr *) (pUDPD->ai_addr)->sa_data);
          printf("\n Aai_addrlen : %d", pUDPD->ai_addrlen);
*/
        if (setsockopt(sockfdUDPA, SOL_SOCKET, SO_REUSEADDR, &yes,
              sizeof(int)) == -1) {
              perror("setsockopt UDPA");
              exit(1);
          }

        if (setsockopt(sockfdUDPB, SOL_SOCKET, SO_REUSEADDR, &yes,
              sizeof(int)) == -1) {
              perror("setsockopt UDPB");
              exit(1);
          }

        if (setsockopt(sockfdUDPC, SOL_SOCKET, SO_REUSEADDR, &yes,
              sizeof(int)) == -1) {
              perror("setsockopt UDPC");
              exit(1);
          }

        if (setsockopt(sockfdUDPD, SOL_SOCKET, SO_REUSEADDR, &yes,
              sizeof(int)) == -1) {
              perror("setsockopt UDPD");
              exit(1);
          }

        recieveCount=4;serverNameCheck=0;
        while(recieveCount >0)  { // main accept() loop
            sin_size = sizeof their_addr;
            new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
            if (new_fd == -1) {
              perror("accept TCP");
              continue;
            }
                      
        if(serverNameCheck == 0)
            {
             displayPeerMessage(new_fd,'A');
             printf("For this Connection with Server A the Client with TCP Port number %s and IP Address %s:\n",PORTCLTCP,ipstr);
             
            }
                 
        if(serverNameCheck == 1)
            { 
              displayPeerMessage(new_fd,'B');
              printf("For this Connection with Server B the Client with TCP Port number %s and IP Address %s:\n",PORTCLTCP,ipstr);
             
            }
            
        if(serverNameCheck == 2)
            {
             displayPeerMessage(new_fd,'C');
             printf("For this Connection with Server C the Client with TCP Port number %s and IP Address %s:\n",PORTCLTCP,ipstr);
             
            }
            
        if(serverNameCheck == 3)
            {
             displayPeerMessage(new_fd,'D');
             printf("For this Connection with Server D the Client with TCP Port number %s and IP Address %s:\n",PORTCLTCP,ipstr);
             
            }
            serverNameCheck=serverNameCheck+1;

            //inet_ntop(their_addr.ss_family,get_in_addr((struct sockaddr *)&their_addr),s, sizeof s);
            //printf("Client TCP: got connection from %s\n", s);
          numbytes = recv(new_fd,buf,MAXDATASIZE-1, 0);
        if (numbytes == -1) {
            perror("recv TCP");
            exit(1);
            }
             
            recieveCount=recieveCount-1;
            
            buf[numbytes] = '\0';
            //printf("Client TCP: received %s \n",buf);
            checkData(buf);
            processData();
            //
          }//end while*/
          
          //sendUDPList();
            //printf("\n UDP LIST SENDING %s\n",sendingUDPListMessage );
            strcpy(sendMSGA,sendingUDPListMessage);
            strcpy(sendMSGB,sendingUDPListMessage);
            strcpy(sendMSGC,sendingUDPListMessage);
            strcpy(sendMSGD,sendingUDPListMessage);
            //strcpy(sendingUDPMessageCLONEFORPRINT,sendingUDPListMessage);
            //strcpy(sendingUDPMessageCLONEFORSEND,sendingUDPListMessage);

            strcpy(sendMSGAPRINT,sendingUDPListMessage);
            strcpy(sendMSGBPRINT,sendingUDPListMessage);
            strcpy(sendMSGCPRINT,sendingUDPListMessage);
            strcpy(sendMSGDPRINT,sendingUDPListMessage);

             //Print the Adjacency Matrix and send this to servers
        printMatrix();
        sendAdjMatrix();
        printMST();


        //strcpy(messageUDPA,AdjMatrix);

        if ((numbytes = sendto(sockfdUDPA, sendMSGA, strlen(sendMSGA), 0,
        pUDPA->ai_addr, pUDPA->ai_addrlen)) == -1) {
        perror("UDPA Client send msg to all: sendtoA");
        exit(1);
        }
        /*if ((numbytes = sendto(sockfdUDPB, sendMSGB, strlen(sendMSGB), 0,
           pUDPB->ai_addr, pUDPB->ai_addrlen)) == -1) {
        perror("UDPB Client send msg to all: sendtoB");
        exit(1);
        }*/
        if ((numbytes = sendto(sockfdUDPC, sendMSGC, strlen(sendMSGC), 0,
           pUDPC->ai_addr, pUDPC->ai_addrlen)) == -1) {
        perror("UDPC Client send msg to all: sendtoC");
        exit(1);
        }
        if ((numbytes = sendto(sockfdUDPD, sendMSGD, strlen(sendMSGD), 0,
           pUDPD->ai_addr, pUDPD->ai_addrlen)) == -1) {
        perror("UDPD Client send msg to all: sendtoD");
        exit(1);
        }
        //printf("UDP messages send %d\n", numbytes);
       
        printf("\nThe Client has sent the network topology to the ServerA with UDP port number %s and IP address %s\n",UDPPORTA,ipstr);
        printf("Edge------------Cost\n");
        displayAllUDPMessages(sendMSGAPRINT);
        displayMessage(sockfdUDPA,'A');
        // printf("\nThe Client has sent the network topology to the ServerB with UDP port number %s and IP address %s\n",UDPPORTB,ipstr);
        // printf("Edge------------Cost\n");
        // displayAllUDPMessages(sendMSGBPRINT);
        // displayMessage(sockfdUDPB,'B');
        printf("\nThe Client has sent the network topology to the ServerC with UDP port number %s and IP address %s\n",UDPPORTC,ipstr);
        printf("Edge------------Cost\n");
        displayAllUDPMessages(sendMSGCPRINT);
        displayMessage(sockfdUDPC,'C');
        printf("\nThe Client has sent the network topology to the ServerD with UDP port number %s and IP address %s\n",UDPPORTD,ipstr);
        printf("Edge------------Cost\n");
        displayAllUDPMessages(sendMSGDPRINT);
        displayMessage(sockfdUDPD,'D');
 
       

        strcpy(AdjMatrixA,AdjMatrix);
        strcpy(AdjMatrixB,AdjMatrix);
        strcpy(AdjMatrixC,AdjMatrix);
        strcpy(AdjMatrixD,AdjMatrix);

        if ((numbytes = sendto(sockfdUDPA, AdjMatrixA, strlen(AdjMatrixA), 0,
           pUDPA->ai_addr, pUDPA->ai_addrlen)) == -1) {
        perror("UDPA Client send Matrix to all: sendtoA");
        exit(1);
        }


         /*if ((numbytes = sendto(sockfdUDPB, AdjMatrixB, strlen(AdjMatrixB), 0,
           pUDPB->ai_addr, pUDPB->ai_addrlen)) == -1) {
        perror("UDPB Client send Matrix to all: sendtoB");
        exit(1);
        }*/

         if ((numbytes = sendto(sockfdUDPC, AdjMatrixC, strlen(AdjMatrixC), 0,
           pUDPC->ai_addr, pUDPC->ai_addrlen)) == -1) {
        perror("UDPC Client send Matrix to all: sendtoC");
        exit(1);
        }


         if ((numbytes = sendto(sockfdUDPD, AdjMatrixD, strlen(AdjMatrixD), 0,
           pUDPD->ai_addr, pUDPD->ai_addrlen)) == -1) {
        perror("UDPD Client send Matrix to all: sendtoD");
        exit(1);
        }


        freeaddrinfo(servinfo);  
        freeaddrinfo(servinfoUDPA);
        freeaddrinfo(servinfoUDPB);
        freeaddrinfo(servinfoUDPC);
        freeaddrinfo(servinfoUDPD);

        close(sockfd);
        close(new_fd);
        close(sockfdUDPA);
        close(sockfdUDPB);
        close(sockfdUDPC);
        close(sockfdUDPD);

}