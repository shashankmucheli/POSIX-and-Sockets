#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>     
#include <unistd.h>    
#include <arpa/inet.h> 
#include <getopt.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <pthread.h>

#define SIZE 1
#define NUMELEM 5
#define SENDING 0
#define RECEIVING 1

pthread_mutex_t mutex;
static struct sockaddr_in ipaddr;
static int sockfd = 0;
unsigned int buf_len = 32;
uint8_t *buf;
int fill = 0x5a;

void end_application (int num) {
	printf ("Exiting application\n");
	exit (num);
}

void getSocketData () {
	int recv_data_len, addr_len = sizeof (ipaddr);
	static unsigned int recv_counter = 0;
	recv_data_len = recvfrom(sockfd, buf, buf_len, 0, 
			(struct sockaddr *)&ipaddr, (socklen_t *)&addr_len);

	printf ("%d, received %d bytes from port %d\n", 
			recv_counter++, recv_data_len, ntohs (ipaddr.sin_port));
	/* printf ("Received : "); */
	/* for (unsigned int i = 0; i < buf_len; i++) */
	/* 	printf ("%02x ", buf[i]); */
	/* printf ("\n"); */
}

void setSocketData () {
	int send_data_len, addr_len = sizeof (ipaddr);
	static int unsigned send_counter = 0;

	/* printf ("Sending : "); */
	for (unsigned int i = 0; i < buf_len; i++)
		buf[i] = fill;
	/* 	printf ("%02X ", buf[i] = fill); */
	/* printf ("\n"); */

	send_data_len = sendto(sockfd, buf, buf_len,  0,
			(struct sockaddr *)&ipaddr, addr_len);

	printf ("%d, sent %d bytes on port %d\n", 
			send_counter++, send_data_len, ntohs (ipaddr.sin_port));
}                                                                     

void usage (char *name) {
	fprintf (stderr, "%s -[sr] -a <address> -p <port>\n", name);
	exit (0);
}

int main (int argc, char **argv) {
	char *ip = NULL;
	int port = 3333;
	int direction = 0, opt;
	
	pthread_mutex_init(&mutex, NULL);

	signal (SIGINT, end_application);

	while ((opt = getopt (argc, argv, "f:l:p:a:sr")) != -1) {
		switch (opt) {                                          
			case 'p':                                               
				port = atoi (optarg);                            
				break;                                              
			case 'f':                                               
				fill = atoi (optarg);                            
				break;                                              
			case 'a':                                               
				ip = strdup (optarg);                               
				break;                                              
			case 's':                                               
				direction = SENDING;
				printf ("Sending data...\n From port %d", port);
				break;                                              
			case 'r':                                               
				direction = RECEIVING;
				printf ("Receiving data...\n From port %d", port);
				break;                                              
			case 'l' :
				buf_len = atoi (optarg);
				break;
			default :
				usage(argv[0]);
		}
	}

	buf = (uint8_t *) calloc (buf_len, 1);

	if ((sockfd = socket (AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror ("sock descriptor");
		exit(-1);
	}

	ipaddr.sin_family = AF_INET;
	ipaddr.sin_port = htons (port);

	if (ip == NULL)
		ipaddr.sin_addr.s_addr = INADDR_ANY;
	else
		ipaddr.sin_addr.s_addr = inet_addr (ip);

	memset (ipaddr.sin_zero, 0, sizeof (ipaddr.sin_zero));
pthread_mutex_lock(&mutex);
	if (direction == RECEIVING) {
		if(bind(sockfd, (struct sockaddr*) &ipaddr, sizeof ipaddr) < 0) {
			perror ("bind socket");
			exit (-1);
		}

		while (1) {	
			getSocketData ();
			sleep (1);
		}
	}
pthread_mutex_unlock(&mutex);

	while (1) {
		setSocketData ();
		sleep (1);
	}
}
