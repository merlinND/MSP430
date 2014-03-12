# Architecture des Ordinateurs — MSP430 — TP 2
Réalisation d'un pilote LCD
Merlin NIMIER-DAVID & Robin RICARD

## Initialisation du contrôleur

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


## Découverte de l'écran

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


## Passage de paramètres, exécution pas-à-pas, et examen de la pile

16. Code assembleur implémentant l'appel `lcd_display_seven_digits(1, 2, 3, 4, 5, 6, 7)` :
		
		// Passage des arguments
		// Empilement (sur la stack) des variables qui ne tiennent pas dans les registres
		push.w    #0x7
		push.w    #0x6
		push.w    #0x5
		// Écriture dans des registres CPU du maximum de paramètres possible
		mov.w     #0x4, R15
		mov.w     #0x3, R14
		mov.w     #0x2, R13
		mov.w     #0x1, R12
		// Saut vers le code assembleur de la fonction
		call      #lcd_display_seven_digits
		
	Les commentaires ont été rédigés à l'aide de [MSP430.pdf | chap3.4]

17. À la ligne `lcd_display_digit(6, f);`, la pile a le contenu suivant :

	| Adesse | Valeur | Commentaire                                                                      |
	| ------ | ------ | -------------------------------------------------------------------------------- |
	| 0x30F2 | 0x0004 | Valeur du paramètre b (sauvegardée depuis un registre)                           |
	|     +2 | 0x0002 | Valeur du paramètre d (sauvegardée depuis un registre)                           |
	|     +4 | 0x31E2 | Adresse à laquelle revenir à la fin de l'exécution de `lcd_display_seven_digits` |
	|     +6 | 0x0005 | Valeur du paramètre e (à charger dans un registre)                               |
	|     +8 | 0x0006 | Valeur du paramètre f (à charger dans un registre)                               |
	|    +10 | 0x0007 | Valeur du paramètre g (à charger dans un registre)                               |
	|    +12 | 0x3108 | Adresse à laquelle revenir à la fin de l'exécution de `main`                     |

## Générateur pseudo-aléatoire

### Représentation des entiers

18. D'après [compiler.pdf, p. 170], la variable `n`, de type `unsigned int` est représentée sur 16 bits.

19. L'opération `30000 * 40000 = 1200000000 > 2^16 - 1 = 65535`. Le résultat affiché est `35840`, qui correspond à l'opération modulo `2^16`. Il s'agit d'un dépassement de capacité (*overflow*).

20. Pour stocker un tel nombre, on aurait du utilier le type `unsigned long`, qui peut contenir des valeurs jusqu'à `2^32 - 1 = 4294967295`.

21. Pour éviter le tronquage à 16 bits, on cast l'un des deux opérandes en `unsigned long`. Le calcul devient :
	
		unsigned long c = (unsigned long)a * b;
	
	Dans le debugger, on constate que le résultat du calcul (sur 32 bits) a été placé dans les registres R12 (16 bits de poids faible) et R13 (16 bits de poids fort). Le résultat est cette fois-ci correct.

22. Le code du générateur aléatoire devient :

		uint16_t alea()
		{
			static uint16_t n = 1;

			n = n * 3 + 5;
			return n;
		}

### Différentes manières de multiplier

23. Dans la fonction `alea`, la multiplication a été traduite par le compilateur par la suite d'instructions suivante :
	
		mov.w    &n,R15        // Copie de `n` vers un registre de travail
		mov.w    R15,R14       // Sauvegarde de `n` dans un autre registre
		rla.w    R15           // Décalage à gauche arithmétique (équivalent à `n * 2`)
		add.w    R14,R15       // Ajout de `n` dans le registre de travail
		add.w    #0x5,R15      // Ajout de `5` dans le registre de travail
		mov.w    R15,&n        // Sortie du résultat

	Lorsqu'une "vraie" multiplication est utilisée (par exemple, en utilisant une seconde opérande variable), la suite d'instructions est la suivante :

		push.w   SP
		dint
		nop
		mov.w   &n,&0x130
		mov.w   0x2(SP),&0x138
		mov.w   &0x13A,R15
		pop.w   SR
		add.w   #0x5,R15
		mov.w   R15,&n

24. On vérifie les adresses utilisées dans le manuel [msp430.pdf, chapitre 8.3] :
	
	| Adresse | Nom du registre | Fonction                                                             |
	| ------- | --------------- | -------------------------------------------------------------------- |
	| 0x130   | MPY             | Opérande 1 pour multiplication non signée                            |
	| 0x138   | OP2             | Seconde opérande                                                     |
	| 0x13A   | RESLO           | 16 bits de poids faible du résultat                                  |
	| 0x13C   | RESHI           | 16 bits de poids fort du résultat (**ignorés dans notre programme**) |

25. Instructions assembleur commentées en ayant pris connaissance des fonctions du multiplieur :

	push.w   SP             // Empilement de la seconde opérande dans la pile
	dint                    // Masquage des interruptions pour garantir une exécution atomique
	nop                     // Pas d'opération
	mov.w   &n,&0x130       // Copie de l'opérande `n` vers le multiplieur (projeté en mémoire)
	mov.w   0x2(SP),&0x138  // Copie de la seconde opérande vers le multiplieur (projeté en mémoire)
	mov.w   &0x13A,R15      // Copie du résultat du multiplieur dans le registre 15
	pop.w   SR              // Dépilement de la seconde opérande
	add.w   #0x5,R15        // Ajout de 5
	mov.w   R15,&n          // Sortie du résultat

26. On compile avec les droits modes d'accès au multiplieur et on observe le code généré.

	* *Direct Access* : comme précédemment, le compilateur inscrit directement l'adresse des registres dans le code assembleur. Les instructions de placement des opérandes et de récupération du résultat sont explicites. La multiplication est effectuée par le multiplieur.

	* *Library Calls* : les instructions d'accès au multiplieur sont placées dans une fonction `Mul16Hw`, qui est appelée depuis notre fonction `alea`. Le suffixe "Hw" indique que la multiplication est effectuée par le multiplieur *hardware*.

	* Désactivé : cette fois, un fonction `Mul16` est appelée : elle implémente un algorithme de multiplication *software*, avec les instructions assembleur disponibles.

27. On compare la compilation de `mul(42, 170)` avec les trois modes :

	| Mode          | Nombre d'instructions générées | Nombre de cycles (programme complet) |
	| ------------- | ------------------------------ | ------------------------------------ |
	| Direct Access | 9                              | 68                                   |
	| Library Calls | 10                             | 75                                   |
	| Désactivé     | 25                             | 110                                  |

28. L'utilisation du multiplieur matériel est toujours plus avantageuse en temps d'exécution ET en nombre d'instructions. L'autorisation de l'écriture d'accès direct permet d'économiser en sauts (moins de cycles), mais implique une répétition des instructions à chaque multiplication du code. La dernière solution, la multiplication *software*, permet de remplacer le multiplieur *hardware* s'il n'est pas disponible.

### Programmation en assembleur

29. On teste le code suivant :

		lcd_init();
		uint16_t a = mul(42, 170);
		lcd_display_number(a);
		for (;;);

	Dans l'état actuel, la fonction `mul` écrite en assembleur est vide. Pourtant, la valeur `42` est affichée sur l'écran LCD. En exécutant le programme pas à pas, on remarque que les arguments de la fonction `mul` sont chargés dans les regitres R12 (valeur `42`) et R13 (valeur `170`). Or la fonction `lcd_display_number` utilise directement le registre R12 comme argument. La documentation [Compiler.pdf p.95] explique que la valeur de retour est stockée dans le registre R12.

30. On implémente la multiplication par une simple boucle itérative :

		PUBLIC mul ; export `mul` to the outside world
		RSEG  CODE ; this is a relocatable segment containing code

		mul:       ; entry point to the function
		  TST R13
		  JZ mulendnull ; jump if second operand is null
		  MOV R12,R14 ; working copy
		  DEC R13 ; we already got the first operand one time
		mulloop:
		  TST R13
		  JZ mulend ; return when the second operand is null
		  ADD R14,R12 ; add the working copy to the accumulator
		  DEC R13 ; decrement the counter
		  JMP mulloop ; loop
		mulend:
		  RET ; return R12
		mulendnull:
		  CLR R12
		  RET ; return 0
		END

31. Notre implémentation de la multiplication est très peu efficace car l'algorithme utilisé est naïf et n'utilise pas le multiplieur matériel. L'algorithme `Mul16` proposé par le compilateur est beaucoup plus efficace.

	| Mode          | Nombre d'instructions générées | Nombre de cycles (programme complet) |
	| ------------- | ------------------------------ | ------------------------------------ |
	| Direct Access | 9                              | 68                                   |
	| Library Calls | 10                             | 75                                   |
	| Désactivé     | 25                             | 110                                  |
	| `mul`         | 15                             | 345                                  |
