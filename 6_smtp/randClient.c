/*
--------------------------------------------------------------------------------------------------------
Commentaire pour l'utilisation de ce code:

la ligne de commande à fournir est: /.mailclient expediteur@unige.ch recepteur@unige.ch contenu.txt

Attention, il faut sans doute changer l'adresse IP stockée dans la variable myIP

Un exemple de fichier texte pour le contenu du mail est fourni car le header est impératif (pour éviter d'être filtré)

ENJOY !

--------------------------------------------------------------------------------------------------------
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
#include <netdb.h>
/* une fonction qui fait arreter le programme quand il y a quelque chose qui ne va pas */
void die(char *issue) {
  perror(issue); 
  exit(EXIT_FAILURE); 
}

/* Prépare l'adresse du serveur */
void prepare_address( struct sockaddr_in *address, const char *host, int port ) {
  size_t addrSize = sizeof( address );
  memset(address, 0, addrSize);
  address->sin_family = AF_INET;
  inet_pton( AF_INET, (char*) host, &(address->sin_addr) );
  address->sin_port = htons(port);
}

/* Construit le socket client */
int makeSocket( const char *host, int port ) {
  struct sockaddr_in address;
  int sock = socket(PF_INET, SOCK_STREAM, 0);
  if( sock < 0 ) {
    die("Failed to create socket");
  }
  prepare_address( &address, host, port );
  if( connect(sock, (struct sockaddr *) &address, sizeof(address)) < 0) {
    die("Failed to connect with server");
  }
  return sock;
}

// Fonction qui vérifie si le serveur a bien recu le message
void erreurNombreRecu(char *mes_ser, char *message){
	int n;
	char mess[10000];
	char data[10000];
	
	//extraction des trois premiers chiffres du message serveur
	strncpy(mess,mes_ser,3);
	n=atoi(mess);

  	sprintf(data, "%s\n", "DATA");
	//vérification du nombre renvoyé
	if (!(n <= 300 && n>= 200) && strcmp(message, data)){
		
		printf("Message %s", message);
		die("Mauvaise recuperation du serveur.");
		
	}

}

//Fonction qui écrit sur le socket et récupère le message envoyé par le serveur en retour
void writeSocket(int sock, char *message){
    char message_server[10000]; /*message de retour du serveur*/
     ssize_t n=0;
	//écriture
    if( write(sock,message,strlen(message)) < strlen(message) ) {
    	die("Erreur dans l'envoi du message.");
  }
  //lecture
  n = read( sock, message_server, 10000);
	printf("messageServer: %s\n",message_server);
   
   if( n  < 0 ) {
    die("Erreur dans la reception du message.");
    }
    erreurNombreRecu(message_server,message);
}

//Fonction qui lit le contenu du mail depuis le fichier et l'écrit sur le socket 
void lireFichier(int sock, char* nomFich){
	FILE *fich;
	char contenu[1000000];
	char buff[10000];
	int succ;
	fich =	fopen(nomFich, "r");
	if(fich != NULL){
		//concatenation du contenu pour mettre dans Buffer
		while(fgets(buff,10000,fich) != NULL){
			strcat(contenu,buff);
			
		}
	
	}
	else{
		die("erreur dans lecture du contenu de mail");
	}
	//fermeture du fichier
	
	fclose(fich);	
	writeSocket(sock,contenu);
}

int main(int argc, char *argv[]) {

/* Initialisation */
  int sock;    /* Socket */
  char *exped;  /* adresse mail expediteur */
  char *recept;    /* adresse mail recepteur */
  char nomFich[100000]; /* nom du fichier contenant le mail */
  char hostAdr[10000];
  char message[10000];
  char BUFF[10000];
//bonne ligne de commande
  if (argc != 4) {
    fprintf(stderr, "USAGE: %s <expediteur> <recepteur> <contenu>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  exped = argv[1];
  recept = argv[2];
  strcpy(nomFich, argv[3]);
  

 //récuperation de l'adresse IP du serveur
  struct hostent *host;

  printf("apres struct\n");
  host = gethostbyname("smtp.unige.ch");

  inet_ntop(AF_INET,host->h_addr,  hostAdr, 1000);
  printf("hostadr: %s\n",hostAdr);

   /* Connexion */	
  sock = makeSocket( hostAdr, 587 );
  read( sock, BUFF, 10000);
  

  /* Envoie de la requête */
   
  printf("le tout premier message: %s\n", BUFF);

  printf("CONNEXION etablie, REQUETE envoyee\n");
 
  printf("ENVOI DE HELO \n");
// adresse IP personnel
 char myIP[10000] = "129.194.245.54";

  sprintf(message, "HELO %s\n",myIP);
  writeSocket(sock,message);
 
    /* ecriture du expediteur sur socket */	
   bzero(message,10000);//remise à zero de la chaine de caractère
   printf("ENVOI DE exped \n");
   sprintf(message, "MAIL FROM:%s\n", exped);
   writeSocket(sock,message);

   /* ecriture du recepteur sur socket */
   bzero(message,10000);
   printf("ENVOI DE recepteur \n");
   sprintf(message, "RCPT TO:%s\n", recept);
   writeSocket(sock,message);

   /* ecriture du mot DATA sur socket */
   printf("DATA\n");
   bzero(message,10000);
   sprintf(message, "%s\n", "DATA");
   writeSocket(sock,message);

   /* ecriture du contenu du mail */
   printf("ENVOI DE contenu \n");
   lireFichier(sock,nomFich);

  /* fermeture de la connexion */
   printf("connection is ending \n");
   bzero(message,10000);
   sprintf(message, "%s\n", "QUIT");
   writeSocket(sock,message);
  
  //libèreation du socket
  close(sock);
  exit(EXIT_SUCCESS);
}        
