# Projet IOT MAKER

L'objectif de ce projet est de réaliser un robot intéractif en modélisant les pièces en 3D et en utilisant des composants arduino pour gérer la partie intelligente.

## Fonctionnalitées : 

Wall E peut :
- Se déplacer
- Ouvrir et fermer sa trappe
- Déplacer ses bras d'avant en arrière
- Ecran intéractif

## Matériel : 

- 2 moteurs DC
- 1 carte de contrôle moteur L298N
- 4 batteries 18650
- 2 servo-moteur SG90
- 1 servo-moteur MG996R
- 1 capteur ultrason HC-SR04
- 1 écran LCD

## Impression 3D

- Face avant du robot (espace vide pour la porte), contient le servo de la porte
- Porte avant
- Face arrière
- Plaque gauche
- Plaque droite
- Sol
- Plaque du haut
- Tête
- Cache moteurs
- Support pour les roues non motrices

## Approche de la conception

Le projet repose sur sa composante modulable.
En effet, les différentes parties sont fixés entre elles grâce à des vis M4, permettant de démonter et modifier certaines parties facilement.
La face avant contient la porte et se visse au sol et plaques droite/gauche.
La seule exception repose pour les parties en dessous du robot et la tête, celles-ci sont fixées à l'aide de colles (élément non destiné à être changés).

Ainsi, cette approche permet la modification et le dépannage facile en permettant de démonter les plaques du chassis centrales contenant les différents composants.
