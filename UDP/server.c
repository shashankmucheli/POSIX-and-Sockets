/*
Name: Shashank Mucheli Sukumar
ID: 01442857
Course: CIS 570
Instructor: Dr. Paul Gracia

Instructions to compile the code: 
	1. Open terminal in an unix environment.
	2. type: gcc server.c
	3. type: ./a.out
*/

#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<stdlib.h>

#define TRUE 1
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

int main()
{
  struct sockaddr_in serverAdd,clientAdd;
  socklen_t clientAddLen;
  int msgCnt;

  pid_t pid;
  int socketId = socket(AF_INET,SOCK_DGRAM,0);

  if(socketId < 0)
  {
     printf("Socket Error...!!!\n");
     return -1;
  }
  
  memset(&serverAdd,'0',sizeof(serverAdd));
  serverAdd.sin_family = AF_INET;
  serverAdd.sin_addr.s_addr = INADDR_ANY;
  serverAdd.sin_port = htons(5781);

  clientAddLen = sizeof(struct sockaddr_in);
  
  if(bind(socketId,(struct sockaddr *)&serverAdd,sizeof(serverAdd)) < 0)
  {
    printf("Binding Error...!!!\n");
    return -1;
  }

  FILE *ptr;
  
  char fileBuffer[BUF_SIZE];
  char destFile[256];
  //bzero(destFile,256);
  int flagFile = 1;
  int i = 0;
  char *split[2];
  char *strPtr;
  bzero(fileBuffer,BUF_SIZE);
  int dataSize = 0;
	 
  struct timeval t1, t2, delta;
  double elapsedTime;
  
  while(1)
  {   
       printf("Server is Running.........\n");
       
       msgCnt = recvfrom(socketId,fileBuffer,BUF_SIZE,0,(struct sockaddr *)&clientAdd,&clientAddLen);
	   gettimeofday(&t1, NULL);
	   int fileSize = strlen(fileBuffer);
	 
        strPtr = strtok(fileBuffer,"@");
        for(i = 0; i < 2 ; i++)
        {
             split[i] = strPtr;
             strPtr = strtok(NULL,"");
        }

         ptr = fopen(split[0],"r");
         if(ptr != NULL)
         { 
             printf("File Name : %s already exist on server.\nPlease give different file Name.\n",split[0]);
         }
         else
         {     
              ptr = fopen(split[0],"w");
              if(ptr == NULL)
              {
                printf("Error in creating a file : %s....!!!\n",split[0]);
              }
              else
              {
			     double sizeBits = strlen(split[1])  * 8;
                 fwrite(split[1],1,strlen(split[1]),ptr);
                 fclose(ptr);
	             gettimeofday(&t2, NULL);
                 duration(&t1,&t2,&delta);
 
                 double megaBits = sizeBits / (1024 * 1024);
                 printf("Size of file bits : %5.6f\n",sizeBits);
                 printf("Size of file in Mbits: %5.6f\n",megaBits);
  
                 elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
                 elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
                 double f = (t2.tv_sec - t1.tv_sec);
				
				 printf("Time required for the receiving %5.10f seconds\n",(elapsedTime / 1000));
 
				 double tranRate = megaBits/(elapsedTime / 1000);
    
                 printf("Average receiving Rate %5.10f Mbits/sec\n",tranRate);
                 bzero(fileBuffer,BUF_SIZE);
              }
         }
     }
  return 0; 
}