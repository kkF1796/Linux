#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "common.h"

/* Prépare l'adresse du serveur */
void prepare_address( struct sockaddr_in *address, const char *host, int port ) {
  size_t addrSize = sizeof( address );
  memset(address, 0, addrSize); // rempli espace mémoire de 0
  address->sin_family = AF_INET; // rempli le champ sin_family de la structure 
  inet_pton( AF_INET, (char*) host, &(address->sin_addr) ); //permet d'obtenir une adresse IP valide
  address->sin_port = htons(port); // converti un entier en numéro de port valide
}

/* Construit le socket client */
int makeSocket( const char *host, int port ) {
  struct sockaddr_in address; // structure qui représente une adresse de socket IP
  int sock = socket(PF_INET, SOCK_STREAM, 0);// création d'un socket
  if( sock < 0 ) {
    die("Failed to create socket");
  }
  prepare_address( &address, host, port ); // préapration de l'adresse
  if( connect(sock, (struct sockaddr *) &address, sizeof(address)) < 0) { // connection entre socket et serveur
    die("Failed to connect with server");
  }
  return sock;
}

int main(int argc, char *argv[]) {
  int sock;    /* Socket */
  char *host;  /* Adresse IP du serveur */
  int port;    /* Port du service */
  size_t numBytes; /* Nombre de bytes aléatoires demandés */
  char *data; /* Buffer de reception */
  size_t rcvd=0; /* Bytes reçus */
  ssize_t n=0;
  ssize_t BUFF = 800;

  /* Initialisation */

  
  if (argc != 4) {
    fprintf(stderr, "USAGE: %s <host> <port> <numBytes>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  host = argv[1];
  port = atoi(argv[2]);
  numBytes = atoi(argv[3]);

  /* Connection */
  
  sock = makeSocket( host, port );

  /* Envoie de la requête */
  
  if( write(sock,&numBytes,sizeof(numBytes)) < sizeof(numBytes) ) { // écrit sur socket du serveur le nombre de bytes à envoyer
    die( "Cannot send the filename to retrieve" );
  }

  /* Reception de la réponse */
  
  data = (char*) malloc( numBytes );
int i = 0;
  while( rcvd < numBytes ) {
    n = read( sock, data+rcvd, BUFF );

    if( n  < 0 ) {
      die( "Cannot receive data" );
    }	
	
    rcvd += n;
i++;
    printf( "reception du paquet:%d.\t",i );
printf("de taille: %d\n",n);
  }

  /* Décommenter pour afficher le résultats en hexadecimal */

  
  printf( "Received: " );
  for( int i=0; i < numBytes; i++ ) {
    printf("%x", data[i] & 0xff );
  }
  printf("\n");
printf("%d le nombre de bytes recus\n", rcvd);
  

  /* Libération des resources */
  free( data );
  close(sock);
  
  exit(EXIT_SUCCESS);
}        
