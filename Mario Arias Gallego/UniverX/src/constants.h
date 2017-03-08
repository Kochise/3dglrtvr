
#ifndef CONSTANTS_H
#define CONSTANTS_H


// Orbits
#define PLANET_FILE "sistema.kep"
#define PI 3.1415926535897932384626433832795
#define PIMID (PI/2)
#define L 149597870691.0               //1 UA en metros

// GLUT
#if !defined(GLUT_WHEEL_UP)
#  define GLUT_WHEEL_UP   3
#  define GLUT_WHEEL_DOWN 4
#endif

// Timer
#define TIMER_DELAY 25

// Titulo de la ventana
#define TITULO_VENTANA "Simulador del Sistema Solar"

#define VENT_WIDTH 900
#define VENT_HEIGHT 700

#define VENT_POSX 100
#define VENT_POSY 100

// Para la camara
#define ANCHO 680
#define ALTO 480
#define PROFUNDIDAD 500

#define MAX_ZOOM 50.0
#define MIN_ZOOM 0.01

/* Posicion y valor de las luces */
// Luz del Sol
#define C0S 0.0
#define C0D 0.6
#define C0A 0.2

// Luz extra del sol para planetas grandes
#define C3S 0.0
#define C3D 0.6
#define C3A 0.8

// Luz blanca direccional desde la camara
#define C1S 0.0
#define C1D 0.08
#define C1A 0.01
#define DIST 80.0

// Luz extra de la camara para planetas grandes
#define C2S 0.0 
#define C2D 0.8
#define C2A 0.6

#define MIN_PLANETRAD_LIGHT3 0.0001 

// Materiales
#define MATPLANETSPE 0.2
#define MATPLANETDIF 0.2
#define MATPLANETAMB 0.1

#define MATSOLSPE 0.3
#define MATSOLDIF 0.3
#define MATSOLAMB 0.6

// Transformar coordenadas para texto fijo
#define TEXT_POS(a) ( a *(zoom/MAX_ZOOM))

#define TRIP(a) a,a,a

// Mensajes por pantalla
#define MENSAJE(args...) sprintf(mensaje,args); printf(args); putchar('\n'); msg_timestamp=time(NULL);
#define CADUCA_MENSAJE 4
#define ACTIVO(flag) ( flag ? "ON" : "OFF")

// Geometria
#define TAM_ESF_EXT 100.0
#define N_ESF_EXT 20
#define CARAS 50


#define N_ANILLOS 50
#define ANILLOS_RADIO_INT 1.5
#define ANILLOS_RADIO_EXT 2.4 
#define LONG_EJE 1.5

// Capturas
#define MAXCAP 999
#define CAPNAME "univerx_"

#endif
