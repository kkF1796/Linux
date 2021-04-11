#include "options.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include<string.h>
#include "hash.h"

void option(int argc, char *argv[])
       {
	const int TAILLE_MAX = 100;
	char com[] = "SHA1";
	int isf = 0;  
	int ist = 0;
	int i;
	
	int opt;
	//utiliser getopt pour reorganiser les variables et voir quelles options ont été choisies par l'utilisateur et recupération de la commande digest
             while ((opt = getopt(argc, argv, "ft:")) != -1) {
               switch (opt) {
               case 'f':
                   isf = 1;
                   break;
               case 't':
                   strcpy(com,optarg);
                   break;
               default: /* '?' */
                   fprintf(stderr, "Usage: %s [-t nsecs] [-n] name\n",
                           argv[0]);
                   exit(EXIT_FAILURE);
               }
           }

	
	printf("\nDigest is: ");
		printf("\n");
//application de la fonction hash sur les fichiers ou le message donné en entrée
	if(isf){
		for (i = optind; i < argc; i++){
				
			hash(com, argv[i],isf);
			printf("	: %s \n", argv[i]);
		
		}
	}

	else{
	      char resultatConcat[10000] = "";
	      strcpy(resultatConcat,argv[optind]);
	      //concatenation pour recuperer le message complet(pas de -f)
	      for (i = (optind+ 1); i < argc; i++){	
						
			strcat(resultatConcat, " ");
                        strcat(resultatConcat, argv[i]);
		

		}
	
	     hash(com, resultatConcat,isf);
	     printf("	: %s \n","string");
	

	}

	   

           if (optind >= argc) {
               printf("Expected argument after options\n");
               exit(EXIT_FAILURE);
           }
		
           /* Other code omitted */
      
       }
