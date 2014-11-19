/*
Name: Shashank Mucheli Sukumar
ID: 01442857
Course: CIS 570
Instructor: Dr. Paul Gracia

Instructions to compile the code: 
	1. Open terminal in an unix environment.
	2. type: gcc client.c
	3. type: ./ncp <source_file_name> <dest_file_name>@<comp_name>
*/

#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include <netdb.h>
#include <sys/time.h>
#include <arpa/inet.h>

#define BUF_SIZE 5000

int duration (struct timeval *start,struct timeval *stop,struct timeval *delta)
{
    suseconds_t microstart, microstop, microdelta;
    
    microstart = (suseconds_t) (100000*(start->tv_sec))+ start->tv_usec;
    microstop = (suseconds_t) (100000*(stop->tv_sec))+ stop->tv_usec;
    microdelta = microstop - microstart;
    
    delta->tv_usec = microdelta%100000;
    delta->tv_sec = (time_t)(microdelta/100000);
    
    if((*delta).tv_sec < 0 || (*delta).tv_usec < 0)
        return -1;
    else
        return 0;
}

int main(int argc,char *argv[])
{
  struct sockaddr_in serverAdd,clientAdd;
  char fileBuffer[BUF_SIZE];
  int msgCnt;
  int socketId = socket(AF_INET,SOCK_DGRAM,0);
  socklen_t serverAddLen;
  struct hostent *server;
  char *strPtr;
  char *split[2];
  
  if(argc != 3)
  {
    printf("Argument should contain source file name, destination file name and pc name where server is located i.e ./ncp <source_file_name> <dest_file_name>@<comp_name> \n");
    return 1;
  }
  
  strPtr = strtok(argv[2],"@");
    
  int i;
  for(i = 0;i < 2;i++)
  {
     split[i] = strPtr;
     strPtr = strtok(NULL,"");
  }
  
  if(socketId < 0)
  {
     printf("Socket Error...!!!\n");
     return -1;
  }

  memset(&serverAdd,'0',sizeof(serverAdd));

  serverAdd.sin_family = AF_INET;

  //serverAdd.sin_addr.s_addr = INADDR_ANY;
  
  server = gethostbyname(split[1]);
  if(server == 0)
  {
    printf("Unknown host\n");
	return 0;
  }
  
  bcopy((char *)server->h_addr,(char *)&serverAdd.sin_addr.s_addr,server->h_length);
  
  serverAdd.sin_port = htons(5781);

  serverAddLen = sizeof(struct sockaddr_in);

  FILE *ptrFile;
  
  ptrFile = fopen(argv[1],"r");
  
  if(ptrFile == NULL)
  {
     printf("File : %s is not present..!!!\n",argv[1]);
     return 1;
  }
   int index = 0;
   char ch ;

   struct timeval t1, t2, delta;
   double elapsedTime;
   gettimeofday(&t1, NULL);
   int fileNameCnt = strlen(split[0]);
  
   //bzero(fileBuffer,BUF_SIZE);
   while(*split[0] != '\0')
   {
     fileBuffer[index++] = *split[0]++;
   }
   fileBuffer[index++] = '@';
   while(1)
   {
     ch = getc(ptrFile);
     if(ch != EOF)
     {
        fileBuffer[index++] = ch;
     } 
     else
     {
       fileBuffer[index++] = '\0';
        break;
     }
   }
   
   msgCnt = sendto(socketId,fileBuffer,strlen(fileBuffer),0,(struct sockaddr *)&serverAdd,serverAddLen);
   // bzero(fileBuffer,BUF_SIZE);

  if(msgCnt <= 0)
  {
    printf("Sending Error...!!!");
  }
  

  gettimeofday(&t2, NULL);
  duration(&t1,&t2,&delta);
  double actualSize = (double)strlen(fileBuffer) - (double)fileNameCnt - 1;
  double sizeBits = actualSize * 8;
  double megaBits = sizeBits / (1024 * 1024);

  printf("Size of file bits : %5.6f\n",sizeBits);
  printf("Size of file in Mbits: %5.6f\n",megaBits);
  
  elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
  elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
  double f = (t2.tv_sec - t1.tv_sec);

  if(elapsedTime == 0.00000)
  {			   
	elapsedTime = 0.001;
  }
  printf("Time required for the transfer %5.10f seconds\n",(elapsedTime / 1000),elapsedTime);
 
  double tranRate = megaBits/(elapsedTime / 1000);
    
  printf("Average Transfer Rate %5.10f Mbits/sec",tranRate);

  close(socketId);
  close(ptrFile);
  return 0;
}