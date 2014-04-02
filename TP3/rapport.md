# Architecture des Ordinateurs — MSP430 — TP 3

_Programmation par interruptions_

B3145 | Merlin NIMIER-DAVID & Robin RICARD

## Remise en jambe

1. On met le premier bit du port `P1` [motherboard p19] dans la direction de lecture et on lit dedans à chaque cycle pendant l'attente [msp430 11.2.3]. Il correspond à la broche 87 du MSP [datasheet p3].

		P1DIR = P1DIR & 0xFC; // Configure push buttons as input
		for (;;)
		{
			// While button is not pressed
			while ( (P1IN & 0x01) != 0x00 );
			// While button has not been released
			while ( (P1IN & 0x01) == 0x00 );

			// ...
		}

## Interruption sur *timer*

### Configuration du timer

2. Le `Timer_A` propose les quatre modes de fonctionnement suivants, dont le fonctionnement est détaillé dans [MSP430.pdf | chap 15.2.3 et 15.2.4]

	- **Stop** : Le timer est arrêté, il ne se passe rien.	- **Up** : Le timer compte de 0 à une valeur au choix, à spécifier dans le champ `TACCR0`. Lorsque la valeur maximale est atteinte, le compte recommence à 9. Remarque : lorsque le timer est passé en mode Up alors que le registre `TAR` a une valeur supérieure à `TACCR0`, il est directement passé à 0. Une interruption `CCIFG` est générée lorsque le compteur atteint `TACCR0`, et une interruption `TAIFG` lorsqu'il repasse à 0 [MSP430.pdf | page 15-6]	- **Continuous** : Le timer compte de 0 à `FFFFh`, et reprend à 0 lorsque cette valeur maximale est atteinte. L'utlisateur peut configurer différents intervalles de temps indépendants. Une interruption à la fin de chaque intervalle. La période du prochain intervalle est communiquée au moment de l'interruption [MSP430.pdf | page 15-8].	- **Up/down** : Le timer compte de 0 à une valeur au choix (à spécifier dans le champ `TACCR0`), puis de cette valeur à 0. Deux interruptions sont générées par période : lorsque la valeur maximale est atteinte, puis lorsque 0 est atteint [MSP430.pdf | page 15-9].

3. D'après [MSP430.pdf | page 15-4], le `Timer_A` peut utiliser les sources d'horloge : `ACLK`, `SMCLK`, ou une horloge externe vie `TACLK` ou `INCLK`. La source est configurée via le registre `TASSEL`. On peut également préciser un diviseur d'horloge (2, 4, ou 8) via le champ `ID` du registre `TACTL`.

4. Les fréquences des horloges sont :

	- `ACLK` : `32.768 kHz` (d'après [Motherboard.pdf | p.7])
	- `SMCLK` : horloge configurable, sourcée par défaut sur l'horloge interne `DCOCLK` avec un multiplicateur de 32. On a donc ici une fréquence de `1.048576 MHz` (d'après [MSP430.pdf | chapitre 5.2]).

5. On souhaite mesurer un intervalle temporel de `10 ms`.

	| Source   | Fréquence      | Nombre de cycles pour `10 ms` |
	| -------- | -------------- | ----------------------------- |
	| `ACLK`   | `32.768 kHz`   | 327.68 cycles                 |
	| `SMCLK`  | `1.048576 MHz` | 10485.76 cycles               |

6. Nous ne pouvons compter qu'un nombre entier de cycles. On aura donc, à chaque intervalle temporel mesuré, une erreur sur le temps mesuré.

	| Source   | Cycles mesurés | Erreur par intervalle |  Erreur    |
	| -------- | -------------- | --------------------- |  --------- |
	| `ACLK`   | 327 cycles     | 0.68 cycles           |  20.752 µs |
	| `SMCLK`  | 10485 cycles   | 0.76 cycles           |  0.725 µs  |

	On pourrait réduire légèrement l'erreur en arrondissant le nombre de cycles mesurés au plus proche :

	| Source   | Cycles mesurés | Erreur par intervalle |  Erreur    |
	| -------- | -------------- | --------------------- |  --------- |
	| `ACLK`   | 328 cycles     | 0.32 cycles           |  9.766 µs  |
	| `SMCLK`  | 10486 cycles   | 0.24 cycles           |  0.229 µs  |
	
	Bien que ces valeurs semblent faibles, l'erreur cumulée pourrait s'avérer gênante. En effet, après 10 secondes avec la source `ACLK`, l'erreur cumulée est de `9 ms`.

7. On choisit la source `ACLK` avec un diviseur de 1. On place le `Timer_A` en mode **Up**. On configure la valeur maximale (`TACCR0`) à 328 (d'après le calcul réalisé à la question précédente).

		void init_timer (long period)
		{
			// Reset the timer (clear any previous configuration)
			TACTL = TACTL | (1 << 2);
			// Set clock source to ACLK
			// Clock divider is 1 by default
			TACTL = TACTL | (1 << 8);

			// Enable Up mode
			TACTL = TACTL | (1 << 4);
			// Set the maximum value for Up mode (328 cycles)
			TACCR0 = 0x148;

			// Enable interruptions
			// Note: timer interrupt vector is TAIV
			TACTL = TACTL | (1 << 1);
		}


### Traitement de l'interruption

8. On utilise le signal d'interruption `TACCR0` comme vu à la question 2. Il correspond à la source Timer_A3 à l'adresse `0FFECh` [datasheet p13].

9. L'instruction de préprocesseur `#pragma opt=value` est équivalente à l'instruction préprocesseur `#define OPT _Pragma("opt=value")` qui définit une option spécifique à la plateforme (_Pragma directive_). [compiler p255].  Dans notre cas, on utilise la _Pragma Directive_ `vector` qui définit quel vecteur d'interruption on va modifier. [compiler p240].

10. `__interrupt` est lui aussi une directive de préprocesseur définissant à quel _handler_ va se rapporter l'interruption. On doit lui passer la signature de la fonction à appeler. Généralement, on utilise de façon associée `#pragma` et `__interrupt`. [compiler p221]

		// [msp430fg4618.h l.2284]
		#pragma vector=TIMERA0_VECTOR
		__interrupt void mon_traitement_interruption_timer(void);

11.

12.

13.

14.

## Étude du mécanisme d'interruption

15.

16.

17.

18.

19.

## Interruption sur bouton poussoir

20.

21.

22.

23.
