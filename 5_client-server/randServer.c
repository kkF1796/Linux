/*
---------------------------------------
Utilisation:
	client Rand : 127.0.0.1 numDePort nbDeBytesAenvoyer
	Client Rand : 127.0.0.1 12345
	Serveur Rand : 12345
			'num de port'

---------------------------------------
*/


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


#define MAX_FULLPATH 1024
#define MAX_NAME 255
#define MAX_PENDING 256


// Fonction qui prépare l'adresse au bon format
void prepare_address( struct sockaddr_in *address,  int port ) {
  size_t addrSize = sizeof( address );
  memset(address, 0, addrSize); // remplir espace mémoire de l'adresse de 0
  address->sin_family = AF_INET; 
  address->sin_addr.s_addr = htonl(INADDR_ANY); // obtenir une adresse numérique valide tout en se liant avec toutes les interfaces
						//réseaux de la machine
  address->sin_port = htons(port); // convertir un int en numéro de port valide
}

// Fonction qui crée le socket, fait le binding d'adresse et socket et attend le client
int makeSocket( int port ) {
  struct sockaddr_in address; // structure représentant une adresse de socket IP
  int sock = socket(AF_INET, SOCK_STREAM, 0); // création du socket 
  if( sock < 0 ) {
    die("Failed to create socket");
  }
//preparation de l'adresse
  prepare_address( &address, port ); // préparer une adresse valide
//binding de socket et adresse
  if( bind( sock, 
	    (struct sockaddr *) &address,
	    sizeof(address)
	    ) < 0  ) // attacher le socket serveur à une adresse
    {
      die("Failed to bind the server socket");
    }
//attente de connection
  if (listen(sock, MAX_PENDING) < 0) { // écoute le début d'une connection
    die("Failed to listen on server socket");
  }
  return sock;
}

// Fonction qui lit le nombre de bytes à envoyer et qui les envoie par paquets au clients
void handleClient( int clientSock ) {
   
	ssize_t n;
	ssize_t nombre_envoye = 0;

	//la lecture du nombre de byte à envoyer
	read( clientSock, &nombre_envoye, sizeof(int) );

	//initialisation
	n = nombre_envoye;
	printf("nombre recu:%d \n", n);
	printf("getting random Data\n");
	//ouverture du fichier urandom
	int randomData = open("/dev/urandom", O_RDONLY);

	const ssize_t BUFFtaille = 800;
	char BUFF[BUFFtaille];
	
	//test pour voir si l'ouverture du fichier a éte bien effectuée
	if (randomData < 0)
	{
		printf("random data pas remplie!");
		 exit(EXIT_FAILURE);
	}
	else{
	

	ssize_t s;
	 int comp = 0;
    	while(n > 0){
		//remplissage du buffer avec les données aléatoires pour chaque paquet
    		ssize_t result = read(randomData, BUFF, sizeof BUFF);

   	 	if (result < 0)
   	 	{
			printf("random data problème!");
			 exit(EXIT_FAILURE);
   	 	}
		/* Envoie du paquet */
		else{
			//si le nombre de bytes restants à envoyer est supérieur à la taille du buff
			if(n >= sizeof BUFF){
				//on vide le buff
				s = write(clientSock, BUFF,sizeof(BUFF)); // on écrit sur le socket client
				//le nombre de bytes restants à envoyer
				n = n - sizeof(BUFF);
				
			}
			//si le nombre de bytes restants à envoyer est inférieur à la taille du buff
			else{
				s = write(clientSock, BUFF,n);
				n = 0;
			}

			
   	 	}
		//l'affichage du paquet envoyé
		printf("envoie du paquet: %d de la taille:%d\n", comp++,s);
		for( int i=0; i < s; i++ ) {
    			printf("%x", BUFF[i] & 0xff );
 		 }
 		 printf("\n");

	}
}
	

	close( clientSock );
}
//la fonction qui gère la communication entre le serveur et client
void run( int serverSock) {
  while( 1 ) {
    struct sockaddr_in clientAddress;
    unsigned int clientLength = sizeof(clientAddress);
    int clientSock;
    printf( "Waiting for incoming connections\n");
	//accepter la connection avec client
    clientSock = 
      accept(serverSock, (struct sockaddr *) &clientAddress, &clientLength ); // accept connexion avec le client
    if( clientSock < 0 ) {
      die("Failed to accept client connection");
    }
    printf( "Client connected: %s\n", inet_ntoa(clientAddress.sin_addr));

//envoie des paquets
    handleClient(clientSock);
  }
}


int main( int argc, char **argv ) {
  int servSock;
  char *path;
  int port;
  if (argc != 2) {
    fprintf(stderr, "USAGE: %s <port> \n", argv[0]);
    exit(EXIT_FAILURE);
  }

  port = atoi(argv[1]);

  servSock = makeSocket( port ); 
  
  printf( "Server running on port %d\n \n", port);
  
  run( servSock);
 /* Libération des resources */
  close(servSock);

  return EXIT_SUCCESS;
}
