# Filtrage_Image_SDL
Filtrage d'image en C en utilisant la Simple DirectMedia Layer

Ce programme utilise la SDL 1.2 (portage vers SDL 2 en cours).
Il permet de réaliser des opérations simples de filtrage.

Il permet de:
* Inverser les couleurs (touche 'i')
* Changer la luminosité (touche 'l' + fleche haut/bas)
* Filtre moyenneur (touche 'm')
* Passer en noir & blanc (touche 'n')
* Binariser l'image (setup des deux couleurs + touche 'b' + fleche haut/bas)
* Quantification n-bits (setup du nb de bits + touche 'q' + fleche haut/bas)

# ToDo:
* Résolution spatiale (échantillonage spatiale)
* Masquage de zone (seuil et position 'a' paramètrables)
* Étirement de l'histogramme (correction de la dynamique)
* Égalisation d'histogramme (pour améliorer le contraste)
* Histogramme exponentiel
* Histogramme cubique
* Histogramme par couleur quantifée
* Constraste (augmentation et diminution)
* Floutage
* Détourage
* Représentation spectrale
* Bruit gaussien / sel et poivre
* Filtre moyenneur
* Filtre médian
