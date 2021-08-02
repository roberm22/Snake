#ifndef _SYSTEMLIB_H_
#define _SYSTEMLIB_H_

#include <time.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/time.h>
#include "pseudoWiringPi.h"

#include "kbhit.h" // para poder detectar teclas pulsadas sin bloqueo y leer las teclas pulsadas
#include "fsm.h"
#include "tmr.h"


#define CLK_MS 					1

// ATENCION: Valores a modificar por el alumno
// INTERVALO DE GUARDA ANTI-REBOTES
#define	DEBOUNCE_TIME			200
#define TIMEOUT_ACTUALIZA_JUEGO 2000

// A 'key' which we can lock and unlock - values are 0 through 3
//	This is interpreted internally as a pthread_mutex by wiringPi
//	which is hiding some of that to make life simple.

// CLAVES PARA MUTEX
// ATENCION: Valores a modificar por el alumno
#define	KEYBOARD_KEY		0
#define	SYSTEM_FLAGS_KEY	1
#define	MATRIX_KEY			2
#define	STD_IO_BUFFER_KEY	3

// Distribucion de pines GPIO empleada para el teclado y el display
// ATENCION: Valores a modificar por el alumno
#define GPIO_KEYBOARD_COL_1 	0
#define GPIO_KEYBOARD_COL_2 	1
#define GPIO_KEYBOARD_COL_3 	2
#define GPIO_KEYBOARD_COL_4 	3
#define GPIO_KEYBOARD_ROW_1 	5
#define GPIO_KEYBOARD_ROW_2 	6
#define GPIO_KEYBOARD_ROW_3 	12
#define GPIO_KEYBOARD_ROW_4 	13

#define GPIO_LED_DISPLAY_COL_1	11
#define GPIO_LED_DISPLAY_COL_2	14
#define GPIO_LED_DISPLAY_COL_3	17
#define GPIO_LED_DISPLAY_COL_4	18
#define GPIO_LED_DISPLAY_ROW_1	4
#define GPIO_LED_DISPLAY_ROW_2	7
#define GPIO_LED_DISPLAY_ROW_3	8
#define GPIO_LED_DISPLAY_ROW_4	10
#define GPIO_LED_DISPLAY_ROW_5	22
#define GPIO_LED_DISPLAY_ROW_6	23
#define GPIO_LED_DISPLAY_ROW_7	24

// FLAGS FSM CONTROL DE SERPIENTE Y GESTION JUEGO
// ATENCION: Valores a modificar por el alumno
#define FLAG_MOV_ARRIBA 	0x01
#define FLAG_MOV_ABAJO		0x02
#define FLAG_MOV_DERECHA 	0x04
#define FLAG_MOV_IZQUIERDA 	0x08
#define FLAG_TIMER_JUEGO	0x10
#define FLAG_BOTON 			0x20
#define FLAG_FIN_JUEGO		0x40

extern int valores; //mejoras
extern int tiempoMS; //v5
extern int tiempoSec; //v5
extern int tiempoMin; //v5
extern int jugando; //v5
extern int puntuaciones[4][3];
extern char  nombres[4][1];
extern char nombreactual;

extern int aux;
extern int posicionMan[2];

enum fsm_state {
	WAIT_START,
	WAIT_PUSH,
	WAIT_END};

extern int flags;

#endif /* SYSTEMLIB_H_ */
