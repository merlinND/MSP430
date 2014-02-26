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

9. D'après [LCD.pdf | p.3], le boîtier comporte 26 pins. Les pints 15-17 correspondent aux broches communes COM0-4, tandis que les autres broches servent aux segments.

10. On en déduit que l'écran comporte 22 * 4 = 88 segments.

11. D'après la matrice [LCD.pdf | p.3], la combinaison COM0 et SP26 permettent d'allumer le symbole `$`.

12. D'après le schéma électrique [Motherboard.pdf | p.15], les broches du MSP430 correspondantes sont respectivement les broches 51 (COM0) et 37 (S21).

13. En s'aidant [MSP430.pdf | chap26.2.1], on en déduit qu'il faut passer le bit 4 à 1 dans le registre LCDMEM12.

		void display_dollar()
		{
			// Power up the 26th pin with COM0 on the LCD component
			// which is connected to the motherboard to the 21th pin
		   // which corresponds to the 25th in the msp430. So, in the
		   // LCD memory, it's at the 12th register in the 2nd word
		   LCDMEM[12] = 0x10; 
		}

14. On écrit les fonctions suivantes :

		// Ordre des bits dans la mémoire LCD : .EGF DCBA

		// 0101 1111
		#define NUMBER0 0x5F
		// 0000 0110
		#define NUMBER1 0x06
		// 0110 1011
		#define NUMBER2 0x6B
		// 0010 1111
		#define NUMBER3 0x2F
		// 0011 0110
		#define NUMBER4 0x36
		// 0011 1101
		#define NUMBER5 0x3D
		// 0111 1101
		#define NUMBER6 0x7D
		// 0000 0111
		#define NUMBER7 0x07
		// 0111 1111
		#define NUMBER8 0x7F
		// 0011 1111
		#define NUMBER9 0x3F
		// Offset pour calculer à quel index de LCDMEM[] correspond la position souhaitée
		#define LCDMEM_OFFSET 2

		void lcd_clear()
		{
		  int j;
		  for( j=0 ; j<20 ; j++)
		  {
		    LCDMEM[j] = 0x00; // Shut down all the tiles
		  }
		}

		int get_word_from_digit(int digit)
		{
		  switch(digit)
		  {
		  case 0:
		    return NUMBER0;
		  case 1:
		    return NUMBER1;
		  case 2:
		    return NUMBER2;
		  case 3:
		    return NUMBER3;
		  case 4:
		    return NUMBER4;
		  case 5:
		    return NUMBER5;
		  case 6:
		    return NUMBER6;
		  case 7:
		    return NUMBER7;
		  case 8:
		    return NUMBER8;
		  case 9:
		    return NUMBER9;
		  default:
		    return 0x00;
		  }
		}

		void lcd_display_digit(int pos, int digit)
		{
		  int idx = pos + LCDMEM_OFFSET;
		  LCDMEM[idx] = get_word_from_digit(digit);
		}


**Passage de paramètres, exécution pas-à-pas, et examen de la pile**

16. Code assembleur implémentant l'appel `lcd_display_seven_digits(1, 2, 3, 4, 5, 6, 7)` :
		
		// Passage des arguments
		// Empilement (sur la stack)
		push.w    #0x7
		push.w    #0x6
		push.w    #0x5
		// Écriture dans des registres CPU
		mov.w     #0x4, R15
		mov.w     #0x3, R14
		mov.w     #0x2, R13
		mov.w     #0x1, R12
		// Saut vers le code assembleur de la fonction
		call      #lcd_display_seven_digits
		
	Les commentaires sont tirés de [MSP430.pdf | chap3.4]

**Générateur pseudo-aléatoire**
