# Architecture des Ordinateurs — MSP430 — TP 2
Réalisation d'un pilote LCD
Merlin NIMIER-DAVID & Robin RICARD

**Initialisation du contrôleur**

1. La fonction `lcd_init()` écrit dans :
  - `P5DIR` et `P5SEL` pour désactiver la fonction GPIO sur les pins P5.2, P5.3 et P5.4. Ces pins sont multiplexés avec les fonctions COM1,   COM2 et COM3, que l'on souhaite utiliser.
  - `LCDACTL` pour configurer le contrôleur LCD
  - `LCDAPCTL0` et `LCDAPTCL1` pour configurer les ports à utiliser pour écrire sur l'écran (*mapping*). Les segments 32 à 39 ne sont pas   mappés sur notre écran LCD.
  - Les registres `LCDMEM[j]` pour effectivement écrire les données.

2. D'après la documentation MSP430 :
  - `P5DIR` et `P5SEL` appartiennent au contrôleur GPIO [MSP430.pdf | chap11.2]
  - `LCDACTL`, `LCDAPCTL0`, `LCDAPCTL1` et `LCDMEM1..20` appartiennent au contrôleur LCD_A [MSP430.pdf | chap26.3]

3. D'après [datasheet.pdf | p26-28], les registres sont projetés en :

  | Nom         | Taille | Adresse      |
  | ----------- | ------ | ------------ |
  | `P5DIR`     | 8      | 032h         |
  | `P5SEL`     | 8      | 033h         |
  | `LCDACTL`   | 8      | 090h         |
  | `LCDAPCTL0` | 8      | 0ACh         |
  | `LCDAPCTL1` | 8      | 0ADh         |
  | `LCDMEM[j]` | 8      | 091h - 0A4h  |	

4. D'après [datasheet.pdf | p.16], la mémoire vive du MSP430FG4618 est projetée dans la plage d'adresse O30FFh - 01100h (jusqu'à 01900h pour la plage étendue).

5. Les lignes 42 et 43 de `msp430fg4618.h` sont :

		#define DEFC(name, address) __no_init volatile unsigned char name @ address;
		#define DEFW(name, address) __no_init volatile unsigned short name @ address;
	Chaque ligne définit une macro servant à placer une variable en mémoire, à une adresse précise. L'opérateur `@` permet d'indiquer directement l'adresse. Le mot-clé `__no_init` permet d'indiquer qu'il ne s'agit pas d'une valeur constante initialisée à la déclaration 	(d'après [compiler.pdf | p196]). La première ligne permet de placer une variable de la taille d'un `char`, la seconde ligne de la taille d'un `short`.

6. D'après [MSP430.pdf | chap26.2.4], la fréquence `fLCD` est basée sur l'oscillateur ACLK. On fait l'hypothèse que ACLK oscille à 32kHz. On souhaite augmenter fFrame afin de supprimer le scintillement. On a :

		fLCD = 2 * mux * fFrame
	Avec, ici, `mux = 4`.

7. Le champ LCDFREQ (bits 5-7) du registre LCDACTL permet de spécifier le diviseur de ACLK à utiliser pour calculer la fréquence du LCD. Dans le code de `lcd_init()` donné, le diviseur sélectionné était 7d = 111b, ce qui correspond à une division de la fréquence par 512 (d'après [MSP430.pdf | chap26.3-LCDACTL]).
	
	On remplace cette valeur par 011b = 3d, ce qui correspond à une division de la fréquence par 128. On obtient ainsi :

		fFrame = (32000/128)/(2*4) = 31.25kHz


**Découverte de l'écran**

8. On passe l'ensemble des registres `LCDMEM[0..19]` à 0h plutôt qu'à FFh.


9. 


**Affichage de nombres**

**Passage de paramètres, exécution pas-à-pas, et examen de la pile**

**Générateur pseudo-aléatoire**
