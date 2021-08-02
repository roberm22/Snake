
#include "snakePi.h"

int flags = 0;

int valores = 0;
int tiempoMS = 0;
int tiempoMin = 0;
int tiempoSec = 0;
int jugando = 0;

int aux = 2;

int posicionMan[2] = {0,0};

char nombres[4][1] = {
		{'R'},
		{'K'},
		{'J'},
		{'O'},
};
int puntuaciones[4][3] = {
		{ 2, 0, 44 },
		{ 1, 0, 32 },
		{ 0, 0, 5 },
		{ 0, 0, 0 },
};
char nombreactual = '-';

// Declaracion del objeto teclado
TipoTeclado teclado = {
	.columnas = {
		// A completar por el alumno...
		// ...
			GPIO_KEYBOARD_COL_1, GPIO_KEYBOARD_COL_2, GPIO_KEYBOARD_COL_3, GPIO_KEYBOARD_COL_4
	},
	.filas = {
		// A completar por el alumno...
		// ...
			GPIO_KEYBOARD_ROW_1,GPIO_KEYBOARD_ROW_2,GPIO_KEYBOARD_ROW_3,GPIO_KEYBOARD_ROW_4
	},
	.rutinas_ISR = {
		// A completar por el alumno...
		// ...
			teclado_fila_1_isr,teclado_fila_2_isr,teclado_fila_3_isr,teclado_fila_4_isr
	},
	.debounceTime ={
			0,0,0,0
	},
	.columna_actual= 0,
	.flags=0,
	.teclaPulsada ={
			-1,-1
	}
};



	// A completar por el alumno...
	// ...


// Declaracion del objeto display
TipoLedDisplay led_display = {
	.columnas = {
		GPIO_LED_DISPLAY_COL_1,GPIO_LED_DISPLAY_COL_2,GPIO_LED_DISPLAY_COL_3
	},
	.filas = {
			GPIO_LED_DISPLAY_ROW_1,GPIO_LED_DISPLAY_ROW_2,GPIO_LED_DISPLAY_ROW_3,GPIO_LED_DISPLAY_ROW_4,GPIO_LED_DISPLAY_ROW_5,GPIO_LED_DISPLAY_ROW_6,GPIO_LED_DISPLAY_ROW_7
	},
	.columna_actual = 0,
	.flags = 0

};



//------------------------------------------------------
// FUNCIONES DE CONFIGURACION/INICIALIZACION
//------------------------------------------------------

// int ConfiguracionSistema (TipoSistema *p_sistema): procedimiento de configuracion
// e inicializacion del sistema.
// Realizará, entra otras, todas las operaciones necesarias para:
// configurar el uso de posibles librerías (e.g. Wiring Pi),
// configurar las interrupciones externas asociadas a los pines GPIO,
// configurar las interrupciones periódicas y sus correspondientes temporizadores,
// la inicializacion de los diferentes elementos de los que consta nuestro sistema,
// crear, si fuese necesario, los threads adicionales que pueda requerir el sistema
// como el thread de exploración del teclado del PC
//int ConfiguraInicializaSistema (TipoSistema *p_sistema) {
	//int result = 0;
	// A completar por el alumno...
	// ...

		/*piLock (STD_IO_BUFFER_KEY);


	// Lanzamos thread para exploracion del teclado convencional del PC
	//result = piThreadCreate (thread_explora_teclado_PC);

	if (result != 0) {
		printf ("Thread didn't start!!!\n");
		piUnlock (STD_IO_BUFFER_KEY);
		return -1;
	}

	piUnlock (STD_IO_BUFFER_KEY);*/

	//return result;
//}

//------------------------------------------------------
// FUNCIONES LIGADAS A THREADS ADICIONALES
//------------------------------------------------------

PI_THREAD (thread_explora_teclado_PC) {  //FUNCION QUE IMPLEMENTA LA GESTION DEL TECLADO EN UN PROCESO PA
	int teclaPulsada;

	while(1) {
		delay(10); // Wiring Pi function: pauses program execution for at least 10 ms

		piLock (STD_IO_BUFFER_KEY);

		if(kbhit()) {
			teclaPulsada = kbread();

			switch(teclaPulsada) {

				case 's':

					piLock (SYSTEM_FLAGS_KEY);
					 flags |= FLAG_TIMER_JUEGO;
					 piUnlock (SYSTEM_FLAGS_KEY);
					printf("\n Tecla S pulsada!\n");
					fflush(stdout);
					break;

				case 'q':
					exit(0);
					break;


				case 'w':

					piLock (SYSTEM_FLAGS_KEY);
					 flags |= FLAG_MOV_ARRIBA;
					 piUnlock (SYSTEM_FLAGS_KEY);
					printf("\n Tecla W pulsada!\n");
					fflush(stdout);
					break;

				case 'a':

					piLock (SYSTEM_FLAGS_KEY);
					 flags |= FLAG_MOV_IZQUIERDA;
					 piUnlock (SYSTEM_FLAGS_KEY);
					printf("\n Tecla A pulsada!\n");
					fflush(stdout);
					break;

				case 'd':

					piLock (SYSTEM_FLAGS_KEY);
					 flags |= FLAG_MOV_DERECHA;
					 piUnlock (SYSTEM_FLAGS_KEY);
					printf("\n Tecla D pulsada!\n");
					fflush(stdout);
					break;

				case 'x':

					piLock (SYSTEM_FLAGS_KEY);
					 flags |= FLAG_MOV_ABAJO;
					 piUnlock (SYSTEM_FLAGS_KEY);
					printf("\n Tecla X pulsada!\n");
					fflush(stdout);
					break;

				case 'p':

					piLock (SYSTEM_FLAGS_KEY);
					 flags |= FLAG_BOTON;
					 piUnlock (SYSTEM_FLAGS_KEY);
					printf("\n Tecla X pulsada!\n");
					fflush(stdout);
					break;

				default:
					//printf("\n !INVALID KEY!!!\n");
					break;
			}
		}

		piUnlock (STD_IO_BUFFER_KEY);
	}
}

// wait until next_activation (absolute time)
void delay_until (unsigned int next) {
	unsigned int now = millis();
	if (next > now) {
		delay (next - now);
	}
}

void tmr_isr (union sigval value){
	flags |= FLAG_TIMER_JUEGO;
}


int main () {
	TipoSistema sistema;
	unsigned int next;
	//timer


	fsm_trans_t snakePi[] = {
		{ WAIT_START, CompruebaBotonPulsado, WAIT_PUSH, InicializaJuego },
		{ WAIT_PUSH, CompruebaTimeoutActualizacionJuego, WAIT_PUSH, ActualizarJuego },
		{ WAIT_PUSH, CompruebaMovimientoArriba, WAIT_PUSH, MueveSerpienteArriba },
		{ WAIT_PUSH, CompruebaMovimientoAbajo, WAIT_PUSH, MueveSerpienteAbajo },
		{ WAIT_PUSH, CompruebaMovimientoIzquierda, WAIT_PUSH, MueveSerpienteIzquierda },
		{ WAIT_PUSH, CompruebaMovimientoDerecha, WAIT_PUSH, MueveSerpienteDerecha },
		{ WAIT_PUSH, CompruebaFinalJuego, WAIT_END, FinalJuego },
		{ WAIT_END, CompruebaBotonPulsado, WAIT_START, ReseteaJuego },
		{-1, NULL, -1, NULL },
	};

	// Configuracion e inicializacion del sistema
	//ConfiguraInicializaSistema (&sistema);
	sistema.snakePi.p_pantalla=&(led_display.pantalla);
	fsm_t* snakePi_fsm = fsm_new (WAIT_START, snakePi, &(sistema.snakePi));
	fsm_t* teclado_fsm = fsm_new (TECLADO_ESPERA_COLUMNA, fsm_trans_excitacion_columnas, &(teclado));
	fsm_t* tecla_fsm = fsm_new (TECLADO_ESPERA_TECLA, fsm_trans_deteccion_pulsaciones, &(teclado));
	fsm_t* display_fsm = fsm_new (DISPLAY_ESPERA_COLUMNA, fsm_trans_excitacion_display, &(led_display)); //v4
	sistema.snakePi.tmr_refresco_snake = tmr_new(tmr_isr);
	teclado.tmr_duracion_columna = tmr_new(timer_duracion_columna_isr);
	led_display.tmr_refresco_display = tmr_new(timer_refresco_display_isr); //v4
	InicializaTeclado(&(teclado));
	InicializaLedDisplay(&(led_display)); //v4
	//puede que falte inicializar pantalla leddisplay
	// A completar por el alumno...
	// ...

	next = millis();
	while (1) {
		fsm_fire (snakePi_fsm);
		fsm_fire (teclado_fsm);
		fsm_fire (tecla_fsm);
		fsm_fire (display_fsm);//v4
		if (jugando) {
			tiempoMS++;
			tiempoSec = tiempoMS / 1000;
			if (tiempoSec > 59) {
				tiempoSec = 0;
				tiempoMS = 0;
				tiempoMin++;
			}
		}
		Clasificacion();


		// A completar por el alumno...
		// ...

		next += CLK_MS;
		delay_until (next);
	}

	fsm_destroy (snakePi_fsm);
	fsm_destroy (teclado_fsm);
	fsm_destroy (tecla_fsm);
	fsm_destroy (display_fsm); //v4

	tmr_destroy ((tmr_t*)(teclado.tmr_duracion_columna));//v4
	tmr_destroy ((tmr_t*)(led_display.tmr_refresco_display));//v4
	tmr_destroy ((tmr_t*)(sistema.snakePi.tmr_refresco_snake));
	return 0;
}
