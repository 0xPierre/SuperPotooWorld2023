# Super Potoo World ++
Super Potoo World is a platform Game made for a school rush in 5 days ([ESIEA](https://esiea.fr)).
We worked on main feature, the creative mode, allowing to create a map and then play it.
We also worked a little bit on the gameplay himself.
### Game Demo :
https://github.com/0xPierre/SuperPotooWorld2023/assets/64857347/772d41dc-2ee3-44c7-acb6-7a6e772a4c85


### Creative Demo :

https://github.com/0xPierre/SuperPotooWorld2023/assets/64857347/fcdcb456-1326-4bea-b859-d9aca04da836

## Readme for School :
Depuis le menu, vous pouvez choisir soit "Play", soit "Creative".
Play permet de jouer au jeu.
Creative permet 







de créer une map en partant de zéro ou d'éditer une map déjà existante.

## Mode créatif 👻
- Edition d'un monde existant
- Création d'un nouveau monde
- Enregistrement du nouveau monde possible pour le jouer ensuite.
- Bouton permettant de changer le fond d'écran du jeu.
- Bouton permettant de changer le mode de vue
	- Mode éditeur ( mis de base ). 
		- Permet de bouger dans le monde avec la souris.
		- Permet de zoomer/dezoomer avec la molette.
	- Mode Personnage
		- Permet de mettre en vue personnage, pour essayer le créatif comme un vrai joueur.
		- La physique des blocks/objets rajouté pendant le mode créatif est directement créé/détruite pour que l'utilisateur puisse essayer.

- Sélectionneur d'objet/block pouvant être placer dans le monde.
	- Cliquer sur un objet le sélectionne
		- Une bordure blanche entoure le bloc sélectionné
	- Le block blanc : 
		- visible seulement en créatif, permet de faire des block invisible et sans physique stoppant les plateformes bougeantes.
- Le Clique molette sur un block/entité du monde permet de le sélectionner pour le placer
- Possibilité de création de tous les block/Ennemy/Collectable/Bonus/Drapeau de fin
	- Le drapeau de fin ne peut être placer qu'à un seul endroit. Donc lorsque il est placé, l'ancien est supprimé.
- Enregistrement des monde dans Assets/Level
	- Pour enregistrer appuyer sur échap, puis "Save and Quit"


		
## Player 👾
- Saut long quand resté appuyer
- Préshot de saut possible 200ms avant d'attendre le saut.
- Animation de marche.
- Animation de mort.
- Animation de dégat ( immunité ).
- Animation de compression au retour de jump .
- Player se retourne quand il va à gauche.
	
## Nut 🥥
- animation de mort.
- Se déplace de manière autonome.
- Vol vers le bas.
	
## Autre 🧰
- Blockage de la caméra.
- Auto détection des blocks GROUND. ( auto complétion des blocks en dessous de pentes/ à coté de vide.
- 6 types de pente.
- Block Bonus ( Faire apparaître un coeur quand tappé du dessous ).
- Block Brick ( cassable du dessous ).
	- 5% de chance de faire apparaître un coeur.
	- 45% de chance de faire apparaître une luciole.
- Gestion des coeurs.
- Animation de la Luciole.
- Descente de pente progressif en fonction de la taille de pente.
- Détection automatique des niveaux dans Assets/Level.
- Ajout d'un bouton retour sur les menus/quitter.
- Ajout de plateforme bougeante autonomie
	- La plateforme détecte un block et change de sens
