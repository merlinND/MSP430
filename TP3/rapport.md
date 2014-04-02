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

9.

10.

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
