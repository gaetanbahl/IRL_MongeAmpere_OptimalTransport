# Introduction à la Recherche en Laboratoire. Solutions de viscosité et schémas numériques associés.

## Introduction

Ce dépot contient le code, le rapport, et les slides de l'introduction à la recherche en
laboratoire que j'ai effectuée au Laboratoire Jean Kuntzmann en 2016.

Il s'agit de l'implémentation en Scilab et en OpenCL de la résolution de l'équation de 
Monge-Ampère pour le transport optimal en 2D. La méthode a été trouvée dans cet article
: http://www.sciencedirect.com/science/article/pii/S0021999113008140

## Contenu

Lire l'article pour les détails.

L'article présente deux schémas aux différences finies (un précis, un monotone) pour la résolution numérique de
l'équation de Monge-Ampère pour le transport optimal sur l'intérieur d'un domaine. Le bord
du domaine est calculé par un autre schéma résolvant une équation de Hamilton-Jacobi. 

La méthode utilisée est une itération explicite (Euler).

### Scilab

J'ai implémenté les trois schémas sous Scilab. Le schéma monotone reste à débuguer. On arrive
cepandant à calculer des transports optimaux avec le schéma précis (cf rapport).

### OpenCL

Seul le schéma précis a été implémenté en OpenCL (pas de condition au bord).

Le code OpenCL a besoin du dossier "common" des exemples du SDK OpenCL d'Intel pour fonctionner.
Je n'ai pas inclus ce dossier dans le dépôt car je n'ai pas écrit ce code. 
Le dossier common peut être trouvé dans les exemples de cette page https://software.intel.com/en-us/intel-opencl-support/code-samples et doit être positionné à côté du dossier "opencl" de ce dépôt avant de lancer make.

## Travail restant

- Déboguer le schéma monotone scilab
- Implémenter les autres schémas en OpenCL
- Utiliser la méthode de Newton décrite dans l'article au lieu de la méthode d'Euler

## Exemples de calculs de déplacements

![Exemple de calcul de déplacement :gaussienne](/slides/depl1.png)
![Exemple de calcul de déplacements : disque](/slides/depl2.png)
