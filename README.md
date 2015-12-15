# Projet_SE

Récupération des trames de données depuis un clavier par un Atmega328p puis envoi par UART sur un ordi pour compréhension des données récupérées.

Découpage du projet:
1. Programmation du microcontroleur : Martin
2. Récupération des données sur ordi puis enregistrement au format VCD : Amandine
3. Décodage du VCD et compréhension des données récupérées : Olivier


1. Programmation du microcontroleur
	Dossier codeAtmega
	sous linux Utiliser makefile -> "make all" pour compiler et televerser sur l'atmega
	
	
2. Recuperation des données sur ordi puis enregistrement au format VCD
	Dossier generate_VCD
	utiliser .pro pour QtCreator
	attention, code développé uniquement pour Windows (utilisation de fonctions windows pour la communication)
	
	release dans "build-generate_VCD-Desktop_Qt_5_4_1_MinGW_32bit-Release", cependant, cela nécessite les bonnes bibliothèques Qt pour le lancer.
	modifier fichier "init.txt" avec les bonnes données pour établir la communication série avec l'atmega
	Attention ! pour quitter, appuyer sur une touche du clavier pour générer le fichier VCD et arrêter le programme correctement
	
3. Décodage du VCD et compréhension des données récupérées 
	Dossier VCDanalyse
	ouverture du projet code::blocks avec .cbp
	attention, ne fonctionne certainement que pour Windows
	
	release dans dossier "bin"
	une fois lancé, demande le nom des fichiers d'initialisation (.ini), puis le fichier .vcd, et enfin le nom du fichier de sortie .txt
	attention, les extensions peuvent se mettre automatiquement si et seulement si le nom du fichier fait strictement plus de 3 caractères
	
	Dans fichier d'initialisation, indiquer l'identifiant de la clock (forcément sur front descendant pour le moment) ainsi que ceux des données à traiter sur cette horloge. Si data ininteressante, laisser vide.
	