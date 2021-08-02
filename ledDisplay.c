
#include "ledDisplay.h"

tipo_pantalla pantalla_inicial = {
	.matriz = {
	{0,0,0,0,0,0,0},
	{0,1,1,0,1,0,0},
	{0,1,1,0,0,1,0},
	{0,0,0,0,0,1,0},
	{0,0,0,0,0,1,0},
	{0,1,1,0,0,1,0},
	{0,1,1,0,1,0,0},
	{0,0,0,0,0,0,0},
	}
};

tipo_pantalla pantalla_final = {
	.matriz = {
	{0,0,0,0,0,0,0},
	{0,0,1,0,0,1,0},
	{0,1,1,0,1,0,0},
	{0,0,0,0,1,0,0},
	{0,0,0,0,1,0,0},
	{0,1,1,0,1,0,0},
	{0,0,1,0,0,1,0},
	{0,0,0,0,0,0,0},
	}
};

// Maquina de estados: lista de transiciones
// {EstadoOrigen, CondicionDeDisparo, EstadoFinal, AccionesSiTransicion }
fsm_trans_t fsm_trans_excitacion_display[] = {
	{ DISPLAY_ESPERA_COLUMNA, CompruebaTimeoutColumnaDisplay, DISPLAY_ESPERA_COLUMNA, ActualizaExcitacionDisplay },
	{-1, NULL, -1, NULL },
};

//------------------------------------------------------
// PROCEDIMIENTOS DE INICIALIZACION DE LOS OBJETOS ESPECIFICOS
//------------------------------------------------------

void InicializaLedDisplay (TipoLedDisplay *led_display) {
	// A completar por el alumno...


			// Comenzamos excitacion por primera columna
			led_display->columna_actual = 0;

			//las filas se apagan a nivel alto
			pinMode (GPIO_LED_DISPLAY_ROW_1, OUTPUT);
			digitalWrite (GPIO_LED_DISPLAY_ROW_1, HIGH);

			pinMode (GPIO_LED_DISPLAY_ROW_2, OUTPUT);
			digitalWrite (GPIO_LED_DISPLAY_ROW_1, HIGH);

			pinMode (GPIO_LED_DISPLAY_ROW_3, OUTPUT);
			digitalWrite (GPIO_LED_DISPLAY_ROW_1, HIGH);

			pinMode (GPIO_LED_DISPLAY_ROW_4, OUTPUT);
			digitalWrite (GPIO_LED_DISPLAY_ROW_1, HIGH);

			pinMode (GPIO_LED_DISPLAY_ROW_5, OUTPUT);
			digitalWrite (GPIO_LED_DISPLAY_ROW_1, HIGH);

			pinMode (GPIO_LED_DISPLAY_ROW_6, OUTPUT);
			digitalWrite (GPIO_LED_DISPLAY_ROW_1, HIGH);

			pinMode (GPIO_LED_DISPLAY_ROW_7, OUTPUT);
			digitalWrite (GPIO_LED_DISPLAY_ROW_1, HIGH);

			//las columnas se apagan a nivel bajo
			pinMode (GPIO_LED_DISPLAY_COL_2, OUTPUT);
			digitalWrite (GPIO_LED_DISPLAY_COL_2, LOW);

			pinMode (GPIO_LED_DISPLAY_COL_3, OUTPUT);
			digitalWrite (GPIO_LED_DISPLAY_COL_3, LOW);

			pinMode (GPIO_LED_DISPLAY_COL_1, OUTPUT);
			digitalWrite (GPIO_LED_DISPLAY_COL_1, HIGH);


			tmr_startms((tmr_t*)(led_display->tmr_refresco_display), TIMEOUT_COLUMNA_DISPLAY);

			printf("\nSystem init complete! display ready to process the code!!!\n");
			fflush(stdout);
}

//------------------------------------------------------
// OTROS PROCEDIMIENTOS PROPIOS DE LA LIBRERIA
//------------------------------------------------------

void ApagaFilas (TipoLedDisplay *led_display){
	// A completar por el alumno...
	// ...
	digitalWrite(GPIO_LED_DISPLAY_ROW_1, HIGH);
	digitalWrite(GPIO_LED_DISPLAY_ROW_2, HIGH);
	digitalWrite(GPIO_LED_DISPLAY_ROW_3, HIGH);
	digitalWrite(GPIO_LED_DISPLAY_ROW_4, HIGH);
	digitalWrite(GPIO_LED_DISPLAY_ROW_5, HIGH);
	digitalWrite(GPIO_LED_DISPLAY_ROW_6, HIGH);
	digitalWrite(GPIO_LED_DISPLAY_ROW_7, HIGH);

}

void ExcitaColumnas(int columna) {

	switch(columna){
			case 0:
				digitalWrite (GPIO_LED_DISPLAY_COL_1, LOW);
				digitalWrite (GPIO_LED_DISPLAY_COL_2, LOW);
				digitalWrite (GPIO_LED_DISPLAY_COL_3, LOW);
				break;
			case 1:

				digitalWrite (GPIO_LED_DISPLAY_COL_1, HIGH);
				digitalWrite (GPIO_LED_DISPLAY_COL_2, LOW);
				digitalWrite (GPIO_LED_DISPLAY_COL_3, LOW);
				break;

			case 2:

				digitalWrite (GPIO_LED_DISPLAY_COL_1, LOW);
				digitalWrite (GPIO_LED_DISPLAY_COL_2, HIGH);
				digitalWrite (GPIO_LED_DISPLAY_COL_3, LOW);
				break;

			case 3:

				digitalWrite (GPIO_LED_DISPLAY_COL_1, HIGH);
				digitalWrite (GPIO_LED_DISPLAY_COL_2, HIGH);
				digitalWrite (GPIO_LED_DISPLAY_COL_3, LOW);
				break;

			case 4:

				digitalWrite (GPIO_LED_DISPLAY_COL_1, LOW);
				digitalWrite (GPIO_LED_DISPLAY_COL_2, LOW);
				digitalWrite (GPIO_LED_DISPLAY_COL_3, HIGH);
				break;

			case 5:

				digitalWrite (GPIO_LED_DISPLAY_COL_1, HIGH);
				digitalWrite (GPIO_LED_DISPLAY_COL_2, LOW);
				digitalWrite (GPIO_LED_DISPLAY_COL_3, HIGH);
				break;

			case 6:

				digitalWrite (GPIO_LED_DISPLAY_COL_1, LOW);
				digitalWrite (GPIO_LED_DISPLAY_COL_2, HIGH);
				digitalWrite (GPIO_LED_DISPLAY_COL_3, HIGH);
				break;

			case 7:

				digitalWrite (GPIO_LED_DISPLAY_COL_1, HIGH);
				digitalWrite (GPIO_LED_DISPLAY_COL_2, HIGH);
				digitalWrite (GPIO_LED_DISPLAY_COL_3, HIGH);
				break;

		}
}

void ActualizaLedDisplay (TipoLedDisplay *led_display) {
	ApagaFilas(led_display);
	ExcitaColumnas(led_display->columna_actual);
	for(int j=0;j<NUM_FILAS_DISPLAY;j++) {
		int i = led_display->pantalla.matriz[led_display->columna_actual][j];

		if(i){
			switch(j){
				case 0:
					digitalWrite (GPIO_LED_DISPLAY_ROW_1, LOW);
					break;
				case 1:
					digitalWrite (GPIO_LED_DISPLAY_ROW_2, LOW);
					break;
				case 2:
					digitalWrite (GPIO_LED_DISPLAY_ROW_3, LOW);
					break;
				case 3:
					digitalWrite (GPIO_LED_DISPLAY_ROW_4, LOW);
					break;
				case 4:
					digitalWrite (GPIO_LED_DISPLAY_ROW_5, LOW);
					break;
				case 5:
					digitalWrite (GPIO_LED_DISPLAY_ROW_6, LOW);
					break;
				case 6:
					digitalWrite (GPIO_LED_DISPLAY_ROW_7, LOW);
					break;
			}
		}

	}

}

void PintaPantallaPorTerminal (tipo_pantalla *p_pantalla) {

	#ifdef __SIN_PSEUDOWIRINGPI__
	int i=0, j=0;

	printf("\n[PANTALLA]\n");
	fflush(stdout);
	for(j=0;j<NUM_FILAS_DISPLAY;j++) {
		for(i=0;i<NUM_COLUMNAS_DISPLAY;i++) {
			printf("%d", p_pantalla->matriz[i][j]);
			fflush(stdout);
		}
		printf("\n");
		fflush(stdout);
	}
	fflush(stdout);
	#endif

}

//------------------------------------------------------
// FUNCIONES DE ENTRADA O DE TRANSICION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

int CompruebaTimeoutColumnaDisplay (fsm_t* this) {
	int result = 0;
	TipoLedDisplay *p_ledDisplay;
	p_ledDisplay = (TipoLedDisplay*)(this->user_data);

	// A completar por el alumno...
	// ...

	piLock (MATRIX_KEY);
	result = (p_ledDisplay->flags & FLAG_TIMEOUT_COLUMNA_DISPLAY);
	piUnlock (MATRIX_KEY);

	return result;
}

//------------------------------------------------------
// FUNCIONES DE SALIDA O DE ACCION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

void ActualizaExcitacionDisplay (fsm_t* this) {
	TipoLedDisplay *p_ledDisplay;
	p_ledDisplay = (TipoLedDisplay*)(this->user_data);

	// A completar por el alumno...
	// ...
	piLock (MATRIX_KEY);
	p_ledDisplay->flags &= (~ FLAG_TIMEOUT_COLUMNA_DISPLAY);


	if(p_ledDisplay->columna_actual > 7)
		p_ledDisplay->columna_actual = 0;
	else{
		p_ledDisplay->columna_actual = p_ledDisplay->columna_actual +1;
	}


	ActualizaLedDisplay(p_ledDisplay);
	piUnlock (MATRIX_KEY);

	tmr_startms((tmr_t*)(p_ledDisplay->tmr_refresco_display), TIMEOUT_COLUMNA_DISPLAY);

}

//------------------------------------------------------
// SUBRUTINAS DE ATENCION A LAS INTERRUPCIONES
//------------------------------------------------------

void timer_refresco_display_isr (union sigval value) {
	// A completar por el alumno...
	// ...
	piLock (MATRIX_KEY);
	led_display.flags |= (FLAG_TIMEOUT_COLUMNA_DISPLAY);
	piUnlock (MATRIX_KEY);
}
