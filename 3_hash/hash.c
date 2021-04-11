#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include<string.h>
#include <openssl/evp.h>
#include "hash.h"


void hash(char com[], char chaine[],int isf)
        {
	
		const int TAILLE_MAX = 100;	
		
		EVP_MD_CTX *mdctx;
		const EVP_MD *md;
		unsigned char md_value[EVP_MAX_MD_SIZE];
		int md_len, i;

		char resultatConcat[] = "";
		char c;
		int k=0;
		
		FILE *fich = NULL;
               //lecture du fichier et recupération dans un string
		if(isf)
		{
			fich = fopen(chaine, "r");
			if (fich)
			{
				
				while( (c=getc(fich)) != EOF)
				{
					if(c == '\n')
					{
						resultatConcat[k]=' ';
					}
					else
					{
						resultatConcat[k]=c;
					}
					k++;

				}
				resultatConcat[k]='\0';
			}
			else
			{
				printf("erreur dans l'ouverture du fichier\n");
			}
			
		}
		else
		{

			strcpy(resultatConcat,chaine);
		}

	

		/*if(!argv[1]) {
		       printf("Usage: mdtest digestname\n");
		       exit(1);
		}*/

		md = EVP_get_digestbyname(com);

		if(!md) {
		       printf("Unknown message digest %s\n", com);
		       exit(1);
		}
                //hachage avec la commande(md) 
		mdctx = EVP_MD_CTX_new();
		EVP_DigestInit_ex(mdctx, md, NULL);
		EVP_DigestUpdate(mdctx, resultatConcat, strlen(resultatConcat));
		
		EVP_DigestFinal_ex(mdctx, md_value, &md_len);
		EVP_MD_CTX_free(mdctx);

		//affichage du digest
		for (i = 0; i < md_len; i++)
		       printf("%02x", md_value[i]);

		 //exit(0);
        }

