# Architecture des Ordinateurs — MSP430 — TP 2
Réalisation d'un pilote LCD
Merlin NIMIER-DAVID & Robin RICARD

## Initialisation du contrôleur

1. La fonction `lcd_init()` écrit dans :
- P5DIR et P5SEL pour désactiver la fonction GPIO sur les pins P5.2, P5.3 et P5.4. Ces pins sont multiplexés avec les fonctions COM1, COM2 et COM3, que l'on souhaite utiliser.
- LCDACTL pour configurer le contrôleur LCD
- LCDAPCTL0 et LCDAPTCL1 pour configurer les ports à utiliser pour écrire sur l'écran (*mapping*). Les segments 32 à 39 ne sont pas mappés sur notre écran LCD.
- Les registres LCDMEM[j] pour effectivement écrire les données.

2. D'après la documentation MSP430 :
- P5DIR et P5SEL appartiennent au contrôleur GPIO [MSP430.pdf | chap11.2]
- LCDACTL, LCDAPCTL0, LCDAPCTL1 et LCDMEM1..20 appartiennent au contrôleur LCD_A [MSP430.pdf | chap26.3]

3. D'après [datasheet.pdf | p26-28], les registres sont projetés en :
Nom 			|	Taille		|	Adresse	
	--------	|	-------	|---------
P5DIR			|	8				|	032h			
P5SEL			|	8				|	033h			
LCDACTL		|	8				|	090h			
LCDAPCTL0	|	8				|	0ACh			
LCDAPCTL1 |	8	 			|	0ADh			
LCDMEM[j]	|	8				|	091h - 0A4h		

4. D'après [datasheet.pdf | p.16], la mémoire vive du MSP430FG4618 est projetée dans la plage d'adresse O30FFh - 01100h (jusqu'à 01900h pour la plage étendue).

## Découverte de l'écran

## Affichage de nombres

## Passage de paramètres, exécution pas-à-pas, et examen de la pile

## Générateur pseudo-aléatoire