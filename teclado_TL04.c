
#include "teclado_TL04.h"

char tecladoTL04[4][4] = {
	{'1', '2', '3', 'C'},
	{'4', '5', '6', 'D'},
	{'7', '8', '9', 'E'},
	{'A', '0', 'B', 'F'}
};

// Maquina de estados: lista de transiciones
// {EstadoOrigen, CondicionDeDisparo, EstadoFinal, AccionesSiTransicion }
fsm_trans_t fsm_trans_excitacion_columnas[] = {
	{ TECLADO_ESPERA_COLUMNA, CompruebaTimeoutColumna, TECLADO_ESPERA_COLUMNA, TecladoExcitaColumna },
	{-1, NULL, -1, NULL },
};

fsm_trans_t fsm_trans_deteccion_pulsaciones[] = {
	{ TECLADO_ESPERA_TECLA, CompruebaTeclaPulsada, TECLADO_ESPERA_TECLA, ProcesaTeclaPulsada},
	{-1, NULL, -1, NULL },
};

//------------------------------------------------------
// PROCEDIMIENTOS DE INICIALIZACION DE LOS OBJETOS ESPECIFICOS
//------------------------------------------------------

void InicializaTeclado(TipoTeclado *p_teclado) {
	// A completar por el alumno...
	// ...
	if (wiringPiSetupGpio() < 0) {
		    fprintf (stderr, "Unable to setup wiringPi: %s\n", strerror (errno)) ;
		}

		// Comenzamos excitacion por primera columna
		p_teclado->columna_actual = COLUMNA_1;

		// Inicialmente no hay tecla pulsada
		p_teclado->teclaPulsada.col = -1;
		p_teclado->teclaPulsada.row = -1;

		pinMode (GPIO_KEYBOARD_ROW_1, INPUT);
		pullUpDnControl(GPIO_KEYBOARD_ROW_1, PUD_DOWN);
		wiringPiISR (GPIO_KEYBOARD_ROW_1, INT_EDGE_RISING, teclado_fila_1_isr);

		pinMode (GPIO_KEYBOARD_ROW_2, INPUT);
		pullUpDnControl(GPIO_KEYBOARD_ROW_2, PUD_DOWN);
		wiringPiISR (GPIO_KEYBOARD_ROW_2, INT_EDGE_RISING, teclado_fila_2_isr);

		pinMode (GPIO_KEYBOARD_ROW_3, INPUT);
		pullUpDnControl(GPIO_KEYBOARD_ROW_3, PUD_DOWN);
		wiringPiISR (GPIO_KEYBOARD_ROW_3, INT_EDGE_RISING,teclado_fila_3_isr);

		pinMode (GPIO_KEYBOARD_ROW_4, INPUT);
		pullUpDnControl(GPIO_KEYBOARD_ROW_4, PUD_DOWN);
		wiringPiISR (GPIO_KEYBOARD_ROW_4, INT_EDGE_RISING,teclado_fila_4_isr);

		pinMode (GPIO_KEYBOARD_COL_2, OUTPUT);
		digitalWrite (GPIO_KEYBOARD_COL_2, LOW);

		pinMode (GPIO_KEYBOARD_COL_3, OUTPUT);
		digitalWrite (GPIO_KEYBOARD_COL_3, LOW);

		pinMode (GPIO_KEYBOARD_COL_4, OUTPUT);
		digitalWrite (GPIO_KEYBOARD_COL_4, LOW);

		pinMode (GPIO_KEYBOARD_COL_1, OUTPUT);
		digitalWrite (GPIO_KEYBOARD_COL_1, HIGH);


		tmr_startms((tmr_t*)(p_teclado->tmr_duracion_columna), TIMEOUT_COLUMNA_TECLADO);

		printf("\nSystem init complete! keypad ready to process the code!!!\n");
		fflush(stdout);

}

//------------------------------------------------------
// OTROS PROCEDIMIENTOS PROPIOS DE LA LIBRERIA
//------------------------------------------------------

void ActualizaExcitacionTecladoGPIO (int columna) {
	// A completar por el alumno
	// ...

	switch(columna){
		case COLUMNA_1:

			digitalWrite (GPIO_KEYBOARD_COL_4, LOW);
			digitalWrite (GPIO_KEYBOARD_COL_1, HIGH);
			/*printf("columna %i \n", teclado.teclaPulsada.col);
			fflush(stdout);*/

			break;
		case COLUMNA_2:


					digitalWrite (GPIO_KEYBOARD_COL_1, LOW);
					digitalWrite (GPIO_KEYBOARD_COL_2, HIGH);
					/*printf("columna %i \n", teclado.teclaPulsada.col);
					fflush(stdout);*/

					break;
		case COLUMNA_3:



					digitalWrite (GPIO_KEYBOARD_COL_2, LOW);
					digitalWrite (GPIO_KEYBOARD_COL_3, HIGH);
					/*printf("columna %i \n", teclado.teclaPulsada.col);
					fflush(stdout);*/


					break;
		case COLUMNA_4:


					digitalWrite (GPIO_KEYBOARD_COL_3, LOW);
					digitalWrite (GPIO_KEYBOARD_COL_4, HIGH);
					/*printf("columna %i \n", teclado.teclaPulsada.col);
					fflush(stdout);*/

					break;
	}
}

//------------------------------------------------------
// FUNCIONES DE ENTRADA O DE TRANSICION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

int CompruebaTimeoutColumna (fsm_t* this) {
	int result = 0;
	TipoTeclado *p_teclado;
	p_teclado = (TipoTeclado*)(this->user_data);

	// A completar por el alumno...
	// ...
	piLock (KEYBOARD_KEY);
	result = (p_teclado->flags & FLAG_TIMEOUT_COLUMNA_TECLADO);
	piUnlock (KEYBOARD_KEY);
	return result;
}

int CompruebaTeclaPulsada (fsm_t* this) {
	int result = 0;
	TipoTeclado *p_teclado;
	p_teclado = (TipoTeclado*)(this->user_data);

	// A completar por el alumno
	// ...
	piLock (KEYBOARD_KEY);
	result = (p_teclado->flags & FLAG_TECLA_PULSADA);
	piUnlock (KEYBOARD_KEY);

	return result;
}

//------------------------------------------------------
// FUNCIONES DE SALIDA O DE ACCION DE LAS MAQUINAS DE ESTADOS
//------------------------------------------------------

void TecladoExcitaColumna (fsm_t* this) {
	TipoTeclado *p_teclado;
	p_teclado = (TipoTeclado*)(this->user_data);

	// A completar por el alumno
	// ...
	piLock (KEYBOARD_KEY);
	teclado.flags &= (~ FLAG_TIMEOUT_COLUMNA_TECLADO);
	piUnlock (KEYBOARD_KEY);

	ActualizaExcitacionTecladoGPIO (p_teclado->columna_actual);
//printf("comienzo columna %i \n", p_teclado->columna_actual);
//fflush(stdout);
	teclado.teclaPulsada.col= p_teclado->columna_actual;

	if( (p_teclado->columna_actual) >2)
		p_teclado->columna_actual=0;
	else{
		p_teclado->columna_actual=p_teclado->columna_actual +1;
	}

	tmr_startms((tmr_t*)(p_teclado->tmr_duracion_columna), TIMEOUT_COLUMNA_TECLADO);

	// Llamada a ActualizaExcitacionTecladoGPIO con columna a activar como argumento
}

void ProcesaTeclaPulsada (fsm_t* this) {
	TipoTeclado *p_teclado;
	p_teclado = (TipoTeclado*)(this->user_data);

	// A completar por el alumno
	// ...
	piLock (KEYBOARD_KEY);

	teclado.flags &= (~FLAG_TECLA_PULSADA);

	piUnlock (KEYBOARD_KEY);
	//printf("%d \n",tecladoTL04[p_teclado->teclaPulsada.row][p_teclado->teclaPulsada.col]);
	//fflush(stdout);
	char teclado = tecladoTL04[p_teclado->teclaPulsada.row][p_teclado->teclaPulsada.col];
	switch(teclado) {

					case '0':
						exit(0);
						break;


					case '2':

						piLock (SYSTEM_FLAGS_KEY);
						 flags |= FLAG_MOV_ARRIBA;
						piUnlock (SYSTEM_FLAGS_KEY);
						//printf("\n Tecla 2 pulsada!\n");
						//fflush(stdout);
						break;

					case '4':

						piLock (SYSTEM_FLAGS_KEY);
						 flags |= FLAG_MOV_IZQUIERDA;
						 piUnlock (SYSTEM_FLAGS_KEY);
						//printf("\n Tecla 4 pulsada!\n");
						//fflush(stdout);
						break;

					case '6':

						piLock (SYSTEM_FLAGS_KEY);
						 flags |= FLAG_MOV_DERECHA;
						 piUnlock (SYSTEM_FLAGS_KEY);
						//printf("\n Tecla 6 pulsada!\n");
						//fflush(stdout);
						break;

					case '8':

						piLock (SYSTEM_FLAGS_KEY);
						 flags |= FLAG_MOV_ABAJO;
						 piUnlock (SYSTEM_FLAGS_KEY);
						//printf("\n Tecla 8 pulsada!\n");
						//fflush(stdout);
						break;

					case '1':

						piLock (SYSTEM_FLAGS_KEY);
						 flags |= FLAG_BOTON;
						 piUnlock (SYSTEM_FLAGS_KEY);
						//printf("\n Tecla 1 pulsada!\n");
						//fflush(stdout);
						break;

					default:
						//printf("\n !ERROR KEY!!!\n");
						break;
				}
	/*switch(p_teclado->teclaPulsada.col){
		case COLUMNA_1:
		case COLUMNA_2:
		case COLUMNA_3:
		case COLUMNA_4:
			printf("\nKeypress \"%c\"...\n",
					tecladoTL04[p_teclado->teclaPulsada.row][p_teclado->teclaPulsada.col]);
			fflush(stdout);
			break;

		default:
			printf("\nERROR!!!! invalid number of column (%d)!!!\n", p_teclado->teclaPulsada.col);
			fflush(stdout);

			p_teclado->teclaPulsada.row = -1;
			p_teclado->teclaPulsada.col = -1;

			break;
	}*/



}


//------------------------------------------------------
// SUBRUTINAS DE ATENCION A LAS INTERRUPCIONES
//------------------------------------------------------

void teclado_fila_1_isr (void) {
	// A completar por el alumno
	// ...
	if (millis () < teclado.debounceTime[FILA_1]) {
		teclado.debounceTime[FILA_1] = millis () + DEBOUNCE_TIME ;
		return;
	}



	teclado.teclaPulsada.row = FILA_1;
	//teclado.teclaPulsada.col = teclado.columna_actual;

	//printf("fila %i columna %i \n", teclado.teclaPulsada.row, teclado.teclaPulsada.col);
	//fflush(stdout);
	piLock (KEYBOARD_KEY);
	teclado.flags |= FLAG_TECLA_PULSADA ;
	piUnlock (KEYBOARD_KEY);

	teclado.debounceTime[FILA_1] = millis () + DEBOUNCE_TIME ;
}

void teclado_fila_2_isr (void) {
	// A completar por el alumno
	// ...
	if (millis () < teclado.debounceTime[FILA_2]) {
		teclado.debounceTime[FILA_2] = millis () + DEBOUNCE_TIME ;
		return;
	}


	teclado.teclaPulsada.row = FILA_2;
	//teclado.teclaPulsada.col = teclado.columna_actual;

	piLock (KEYBOARD_KEY);
	teclado.flags |= FLAG_TECLA_PULSADA ;
	piUnlock (KEYBOARD_KEY);

	teclado.debounceTime[FILA_2] = millis () + DEBOUNCE_TIME ;
}

void teclado_fila_3_isr (void) {
	// A completar por el alumno
	// ...
	if (millis () < teclado.debounceTime[FILA_3]) {
		teclado.debounceTime[FILA_3] = millis () + DEBOUNCE_TIME ;
		return;
	}



	teclado.teclaPulsada.row = FILA_3;
	//teclado.teclaPulsada.col = teclado.columna_actual;

	piLock (KEYBOARD_KEY);
	teclado.flags |= FLAG_TECLA_PULSADA ;
	piUnlock (KEYBOARD_KEY);

	teclado.debounceTime[FILA_3] = millis () + DEBOUNCE_TIME ;
}

void teclado_fila_4_isr (void) {
	// A completar por el alumno
	// ...
	if (millis () < teclado.debounceTime[FILA_4]) {
		teclado.debounceTime[FILA_4] = millis () + DEBOUNCE_TIME ;
		return;
	}


	teclado.teclaPulsada.row = FILA_4;
	//teclado.teclaPulsada.col = teclado.columna_actual;
	printf("fila %i columna %i \n", teclado.teclaPulsada.row, teclado.teclaPulsada.col);
	fflush(stdout);
	piLock (KEYBOARD_KEY);
	teclado.flags |= FLAG_TECLA_PULSADA ;
	piUnlock (KEYBOARD_KEY);

	teclado.debounceTime[FILA_4] = millis () + DEBOUNCE_TIME ;
}

void timer_duracion_columna_isr (union sigval value) {
	// A completar por el alumno
	// ...
	piLock (KEYBOARD_KEY);
	teclado.flags |= FLAG_TIMEOUT_COLUMNA_TECLADO;
	piUnlock (KEYBOARD_KEY);
}


