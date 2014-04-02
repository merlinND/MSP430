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

2.

3.

4.

5.

6.

7.

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
