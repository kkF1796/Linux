# TP 1-2
#Elham Amin Mansour & Kathleen Favre

#!/bin/bash

# Création d'une variable pour chaque input
repDepart=$1
repArr=$2
resol=$3

# Fonction qui vérifie l'existence ou non du répertoire d'arrivée, et création si cela n'est pas le cas
function exists {
    if [ ! -d "$repArr" ]; then
        mkdir $repArr
    fi
}

# Fonction qui ne copie que les fichiers image du répertoire de départ dans celui d'arrivée
# Itération sur chacun des fichiers du répertoire de départ
function deplace {

    for VAR in $repDepart/*; do
       
        martin=$(file -i "$VAR" | cut -f 2 -d ':' | cut -f 1 -d '/')
       

        if [ "$martin" = " image" ]; then
       
            cp "$VAR" $repArr
        fi
    done       
                
           
}

# Fonction supprimant les espaces, ' et " (commande tr) dans le nom d'un fichier du répertoire d'arrivée
function supprimer {
       

        new=$(basename "$VAR")
        newName1=$(echo "$new" | tr -d  '"')
        newName2=$(echo "$newName1" | tr -d "'")
        newName3=$(echo "$newName2" | tr -d ' ')
        mv "$VAR" "$newName3"
   
}


# Fonction principale permettant de corriger le nom, de convertir et de redimensionner les fichiers image du répertoire d'arrivée
function principal2 {
    cd $repArr
    for VAR in $repArr/*; do
        supprimer   
    done
   
    for VAR in $repArr/*; do
       
        new1=$(basename "$VAR")
        new2=$(echo $new1 | cut -f 1 -d '.' ) 
	# retrait du format pour ne garder que le nom de fichier
        form=".png"
        new3="$new2$form"
        if [ -z "$resol" ] 
        then
                convert "$new1" "$new3" 
		# conversion si la variable resol est vide
        else
                convert "$new1" -resize $resol "$new3"
		# conversion et redimensionnement si la variable n'est pas vide et donc que la 			resolution a été fournie en paramètre d'entree
        fi  
 
	rm $new1 
	#supression du fichier jpeg
           
    done

}

# exécution des fonctions pour obtenir le résultat attendu
exists
deplace
principal2

#cd ..
#cd ..



