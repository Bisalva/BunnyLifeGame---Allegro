#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <allegro.h>
#define P_AL 720
#define P_AN 1200
#define MAXFILAS 24
#define MAXCOL 30

// Dimension 30x30

////////////////////-JUGADOR
typedef struct
{
	char nombre[10];
	int posx;  // posicion X
	int posy;  // posicion Y
	int vel;   // velocidad de movimiento
	int pts;   // puntos
	int vidas; // vidas
	int dir;   // 0 Derecha , 1 Izquierda , 2 Arriba, 3 Abajo

} Jugador;
////////////////////

////////////////////-Enemigos
typedef struct
{

	int seguir;	   // 0 NO, 1 SI
	int posx;	   // posicion X
	int posy;	   // posicion Y
	int vel;	   // Velocidad de movimiento
	int direccion; // Para seguir el movimiento del Jugador desde los enemigos s
	int tipo;	   // Tipo de movimiento (izq,der,arriba,abajo)

} Enemigo;

////////////////////-Enemigo  (Seguimiento)
typedef struct
{

	int posx;//posicion x
	int posy;//posicon y
	int vel;// velocidad del enemigo
	int direccion;//direccion donde se mueve

} EnemigoINT;

////////////////////-TRAMPAS (Comida envenenada)
typedef struct
{

	int posx;
	int posy;
	bool toca;

} Trampa;


////////////////////-Ranking
typedef struct
{

	char nombre[10];
	int pts;

} tablero;

////////////////////

////////////////////-Objetos
typedef struct
{

	int posx;
	int posy;
	bool toca;
} Item;
////////////////////

///////////////////////////////////////////PROTOTIPOS/FUNCIONES//////////////////////////////////////////////////////////////////
void pantalla(Jugador p);// Imprime la pantalla y el jugador (su posicion)
void mapa(char mapa_matriz[MAXFILAS][MAXCOL], int ronda);// Lee el mapa
void dibuja(char mapa_matriz[MAXFILAS][MAXCOL]);// Dibuja el mapa y obstaculos
void dibuja_vida(Jugador player);// Dibuja la vida del personaje
void movimiento(char mapa_matriz[MAXFILAS][MAXCOL], Jugador *player);// Movimiento del personaje
int leer_enemigos(Enemigo npc[20], char mapa_matriz[MAXFILAS][MAXCOL]);// Lee los enemigos en el archivo .txt
int leer_enemigo_int(EnemigoINT npcINT[3], char mapa_matriz[MAXFILAS][MAXCOL]);//Lee enemigos s del archivo txt
void movimiento_enemigos(Enemigo npc[20], int cont_enemigos);// Movimiento de los enemigos
void colision_enemigos(Enemigo npc[20], char mapa_matriz[MAXFILAS][MAXCOL], int cont_enemigos);// Colision de los enemigos con los obstaculos
void colision_enemigos_jugador(Jugador *player, Enemigo npc[20], int cont_enemigos);// Colision de los enemigos con los jugadores
void col_enemigoINT(EnemigoINT npc[3], char mapa_matriz[MAXFILAS][MAXCOL]);//colision del enemigo  con el entorno
void movimiento_enemigo_int(Jugador player, EnemigoINT npcINT[3], int cont_enemigo_int, char mapa_matriz[MAXFILAS][MAXCOL]);//mov. del enemigo 
int leer_item_z(Item zanah[15], char mapa_matriz[MAXFILAS][MAXCOL]);// Lee items de archivo txt
void dibuja_item_z(Item zanah[15], int cont_items);// Dibuja los objetos de manera al azar en el mapa
int col_jugador_item(Jugador *player, Item zanah[15], int cont_items, char mapa_matriz[MAXFILAS][MAXCOL]); // colision de los objetos con el jugador
int leer_trampa(Trampa trampa[15], char mapa_matriz[MAXFILAS][MAXCOL]);//lee trampas de archivo txt
void dibuja_trampa(Trampa t[15], int conttrampa);//dibuja trampas en el juego
void col_jugador_trampa(Jugador *player, Trampa trampa[15], char mapa_matriz[MAXFILAS][MAXCOL], int conttrampa);//col del jugador con las trampas
void col_jugador_enemigoINT(Jugador *player, EnemigoINT npc[3]);//col. del player con el enemigo 
void fin(Jugador player);//fin del juego
void ranking(tablero rank[5]);//carga de ranking
void ordenarpts(tablero rank[5]);//ordenar el ranking con bubblesort
void guardarpts(Jugador player,tablero rank[5]);//guarda los puntos en estructura de ranking
void archivoRANK(tablero rank[5]);//guarda la estructura en un archivo sobreescribiendolo
void dibuja_tiempo(int tiempo, int auxtiempo, int auxtiempo2, int auxtiempo3);//dibuja el timepo/contador
void menu(int op,tablero rank[5]);//menu
void destroys(); // destruye los bitmaps necesarios
void init();	 // inicia los procesos de allegro
void deinit();	 // finaliza las funciones de allegro
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////-BITMAPS
BITMAP *fondo; //fondo del juego
BITMAP *pj; //conejo
BITMAP *malo;//zorro cafe
BITMAP *maloINT;//zorro rojo
BITMAP *bush;//murallas
BITMAP *borde;//murallas sin borde
BITMAP *item1;//zanahoria que son los puntos
BITMAP *vida;//corazones
BITMAP *vida_menos;//corazones grises
BITMAP *pausa;//pausa menu
BITMAP *trampaimg;//trampa envenenada
BITMAP *tiempoimg;//tiempo/cronometro
BITMAP *num1;//unidad tiempo
BITMAP *num2;//decena tiempo
BITMAP *num3;//centena tiempo
////////////////////

////////////////////Variables globales
int conteotoca = 0; //cuenta las veces que se agarran zanahorias
int n = 0; //sirve para el conteo de niveles
////////////////////

int main()
{

	init();
	
	int opmenu = 0;
	

	Jugador player;		 // renombra la estructura Jugador a "player"
	Enemigo enemigo[20]; // renombra la estructura Enemigo a un arreglo de 20 elementos llamado "enemigo"
	EnemigoINT enemigoINT[3];
	Item zanah[15]; // renombra la estructura de Item a un arreglo de 15 elementos llamado "zanah"
	Trampa trampa[15];
	tablero rank[5];
	
	player.posx = 60;  // posicion de origen del player X
	player.posy = 660; // posicion de origen del player Y
	player.vel = 3;	   // velocidad base
	player.pts = 0;	   // puntos iniciales
	player.vidas = 5;  // vidas de player
	player.dir = 0;

	ranking(rank);
	menu(opmenu,rank);
	printf("Ingrese Nombre (MAXIMO 10 LETRAS) : ");
	scanf("%s",player.nombre);

	srand(time(0)); // funcion para el uso de numeros random dentro de las funciones al azar (enemigos y objetos)

	int tiempo = 0, tiempo_real = 0, cont_enemigos = 0, cont_enemigo_int = 0, cont_items = 0, conttrampa = 0, auxtiempo = 0, auxtiempo2 = 0, auxtiempo3 = 0;
	// tiempo y tiempo real se usan en el conteo de tiempo en milisegundos
	// cont_enemigos y cont_items se usan para el conteo tanto de enemigos como items dentro de los archivos txt para iniciar
	int m = 0;
	int lee = 1;
	int op = 0;
	int mov = 0;

	char mapa_matriz[MAXFILAS][MAXCOL]; // mapa donde se desarrolla el juego en dimension de 30x30 pixeles cada bloque del mapa

	////////////////////-Inicializacion de los bitmaps
	pj = create_bitmap(30, 30);
	fondo = create_bitmap(P_AL, P_AN);
	bush = create_bitmap(30, 30);
	borde = create_bitmap(30, 30);
	malo = create_bitmap(30, 30);
	maloINT = create_bitmap(25, 25);
	trampaimg = create_bitmap(30, 30);
	item1 = create_bitmap(20, 20);
	vida = create_bitmap(60, 60);
	vida_menos = create_bitmap(60, 60);
	tiempoimg = create_bitmap(100, 100);

	pausa = create_bitmap(500, 400);
	////////////////////
	mapa(mapa_matriz, m); // lee el mapa
	
	cont_enemigos = leer_enemigos(enemigo, mapa_matriz); // lee los enemigos y guarda la cantidad
	cont_enemigo_int = leer_enemigo_int(enemigoINT, mapa_matriz);
	cont_items = leer_item_z(zanah, mapa_matriz); // lee los items y guarda la cantidad
	conttrampa = leer_trampa(trampa, mapa_matriz);

	do
	{
		////////////////////////////////////////////////////////////-Carga de imagenes
		pj = load_bitmap("media/bunnyder.bmp", NULL);
		fondo = load_bitmap("media/fondo.bmp", NULL);
		bush = load_bitmap("media/bush.bmp", NULL);
		borde = load_bitmap("media/borde.bmp", NULL);
		item1 = load_bitmap("media/item1.bmp", NULL);
		vida = load_bitmap("media/vida.bmp", NULL);
		vida_menos = load_bitmap("media/vidamenos.bmp", NULL);
		pausa = load_bitmap("media/PAUSA.bmp", NULL);
		trampaimg = load_bitmap("media/trampa.bmp", NULL);
		tiempoimg = load_bitmap("media/time.bmp", NULL);
		maloINT = load_bitmap("media/enemigoINT.bmp", NULL);

		///////////////////////////////////////////////////////////

		////////////////////-Pausa
		while (key[KEY_ENTER])
		{

			draw_sprite(screen, pausa, 210, 100);
			readkey();
		}
		////////////////////

		////////////////////////////////////////////////////////////-Funciones principales del juego
		if (m == 3 && lee == 1)
		{
			op++;
			mapa(mapa_matriz, op);								 // lee el mapa
			cont_enemigos = leer_enemigos(enemigo, mapa_matriz); // lee los enemigos y guarda la cantidad
			cont_items = leer_item_z(zanah, mapa_matriz);		 // lee los items y guarda la cantidad
			conttrampa = leer_trampa(trampa, mapa_matriz);

			player.posx = 690;
			player.posy = 570;
			lee = 0;
		}

		if (m == 6 && lee == 0)
		{
			op++;
			mapa(mapa_matriz, op);								 // lee el mapa
			cont_enemigos = leer_enemigos(enemigo, mapa_matriz); // lee los enemigos y guarda la cantidad
			cont_items = leer_item_z(zanah, mapa_matriz);		 // lee los items y guarda la cantidad
			conttrampa = leer_trampa(trampa, mapa_matriz);

			player.posx = 540;
			player.posy = 180;
			lee = 1;
		}

		dibuja(mapa_matriz);			  // dibuja el mapa
		dibuja_item_z(zanah, cont_items); // dibuja los objetos en el mapa
		dibuja_trampa(trampa, conttrampa);
		dibuja_vida(player);										   // dibuja la vida del jugador
		movimiento(mapa_matriz, &player);							   // movimiento del jugador
		m = col_jugador_item(&player, zanah, cont_items, mapa_matriz); // colision jugador con el item
		movimiento_enemigos(enemigo, cont_enemigos);				   // movimiento del enemigo
		movimiento_enemigo_int(player, enemigoINT, cont_enemigo_int, mapa_matriz);
		colision_enemigos(enemigo, mapa_matriz, cont_enemigos);		// colision de los enemigos con obstaculos
		colision_enemigos_jugador(&player, enemigo, cont_enemigos); // colision de jugador con enemigos
		col_jugador_enemigoINT(&player, enemigoINT);
		col_jugador_trampa(&player, trampa, mapa_matriz, conttrampa);

		////////////////////////////////////////////////////////////-Cronometro
		tiempo++;

		if (tiempo == 70)
		{
			tiempo_real++;
			tiempo = 0;
			auxtiempo++;
			if (tiempo_real % 10 == 0)
			{
				auxtiempo = 0;
				auxtiempo2++;
				if (auxtiempo2 % 10 == 0)
				{
					auxtiempo2 = 0;
					auxtiempo3++;
				}
			}
		}
		
		
		dibuja_tiempo(tiempo_real, auxtiempo, auxtiempo2, auxtiempo3);
		textprintf_ex(fondo, font, 1000, 180, makecol(0, 0, 0), -1, "%d  pts", player.pts);

		


		pantalla(player); // imprime el jugador y el fondo en la pantalla

		while (player.vidas <= 0)
		{
			fin(player);
			textprintf_ex(screen, font, 550, 650, makecol(255, 255, 255), -1, "%d  pts", player.pts);
			if (key[KEY_ESC])
			{
				break;
			}
		}

		destroys();
	} while (!key[KEY_ESC]);

	
	ordenarpts(rank);
	guardarpts(player,rank);
	ordenarpts(rank);
	archivoRANK(rank);
	
	deinit();
	return 0;
}
END_OF_MAIN()

////////////////////-Inicializacion de allegro
void init()
{
	int depth, res;
	allegro_init();
	depth = desktop_color_depth();
	if (depth == 0)
		depth = 32;
	set_color_depth(depth);
	res = set_gfx_mode(GFX_AUTODETECT_WINDOWED, P_AN, P_AL, 0, 0);
	if (res != 0)
	{
		allegro_message(allegro_error);
		exit(-1);
	}

	install_timer();
	install_keyboard();
	install_mouse();
}
////////////////////

////////////////////-Limpia
void deinit()
{
	clear_keybuf();
}
////////////////////

void ranking(tablero rank[5])
{

	int i;

	FILE *puntos;

	puntos = fopen("ranking.txt", "rt");

	if (puntos == NULL)
	{
		printf("\nNO SE PUDO ABRIR EL ARCHIVO RANKING");
	}

	for (i = 0; i < 5; i++)
	{

		fscanf(puntos, "%s", rank[i].nombre);
		fscanf(puntos, "%d", &rank[i].pts);
	}

	fclose(puntos);

	
}

void ordenarpts(tablero rank[5])//bubblesort
{

	int i, j,k;
	int aux = 0;
	char auxnombre[10];

	for (i = 0; i < 5; i++)
	{
		for (j = 0; j < 5; j++)
		{
			
			if (rank[j].pts > rank[j + 1].pts)
			{
				aux = rank[j].pts;
				rank[j].pts = rank[j + 1].pts;
				rank[j + 1].pts = aux;
				
				for(k=0;k<10;k++){
					auxnombre[k] = rank[j].nombre[k];
					rank[j].nombre[k] = rank [j+1].nombre[k];
					rank[j+1].nombre[k] = auxnombre[k];
				}
				
			}
		}
		
	}
	
	
}

void guardarpts(Jugador player,tablero rank[5]){
	
	int i;
	int aux;
	
	
	if(player.pts > rank[0].pts){
		
		rank[0].pts = player.pts;
	
		for(i=0;i<10;i++){
			rank[0].nombre[i] = player.nombre[i];
		}
		
	}else{
		printf("NO TIENES SUFICIENTES PUNTOS PARA ENTRAR AL RANKING");
	}

	

}

void archivoRANK(tablero rank[5]){
	
	int i;
	FILE *puntos;
	
	
	puntos = fopen("ranking.txt", "w");
	
	for(i=4;i>=0;i--){
		
		fprintf(puntos,"%s %d\n",rank[i].nombre,rank[i].pts);
		
	}
	
	fclose(puntos);
}
////////////////////-LEE EL MAPA
void mapa(char mapa_matriz[MAXFILAS][MAXCOL], int ronda)
{

	int i, j;	// Contadores
	FILE *MAPA; // Abrir el archivo de texto donde esta el mapa

	if (ronda == 0)
	{
		MAPA = fopen("MAPA1.txt", "r");
	}
	if (ronda == 1)
	{
		MAPA = fopen("MAPA2.txt", "r");
	}
	if (ronda == 2)
	{
		MAPA = fopen("MAPA3.txt", "r");
	}

	if (MAPA == NULL)
	{
		printf("ERROR AL LEER MAPA, NO SE PUDO ABRIR ARCHIVO"); // devolucion de "error" si es que no se lee el mapa correctamente
	}

	// lee
	for (i = 0; i < MAXFILAS; i++)
	{
		for (j = 0; j < MAXCOL; j++)
		{
			fscanf(MAPA, "%c", &mapa_matriz[i][j]); // for anidado en otro for para guardar los datos del archivo de texto en una matriz para el mapa
		}
	}

	fclose(MAPA); // Cerrar el archivo txt
}
////////////////////

////////////////////-DIBUJA EL MAPA
void dibuja(char mapa_matriz[MAXFILAS][MAXCOL])
{

	int i, j; // contadores

	// Ciclo for anidado en otro for para dibujar los obstaculos
	for (i = 0; i < MAXFILAS; i++)
	{
		for (j = 0; j < MAXCOL; j++)
		{

			// Si encuentra una 'x' en la matriz se dibuja un bloque de tierra superior
			if (mapa_matriz[i][j] == 'x')
			{

				draw_sprite(fondo, bush, j * 30, i * 30); // se multiplica la ubicacion en 30x30 pixeles
			}
			// Si encuentra una 'b' se dibuja un bloque de tierra base que normalmente se dibujara debajo de un bloque de tierra superior
			if (mapa_matriz[i][j] == 'b')
			{

				draw_sprite(fondo, borde, j * 30, i * 30); // se multiplica la ubicacion en 30x30 pixeles
			}
		}
	}
}
////////////////////

////////////////////-DIBUJA VIDA DEL JUGADOR
void dibuja_vida(Jugador player)
{

	switch (player.vidas)
	{

	case 0: // Dibuja 5 corazones grises (0 vidas)

		draw_sprite(fondo, vida_menos, 880, 100);
		draw_sprite(fondo, vida_menos, 940, 100);
		draw_sprite(fondo, vida_menos, 1000, 100);
		draw_sprite(fondo, vida_menos, 1060, 100);
		draw_sprite(fondo, vida_menos, 1120, 100);
		break;

	case 1: // Dibuja 4 corazones grises y 1 rojos (1 vidas)

		draw_sprite(fondo, vida, 880, 100);
		draw_sprite(fondo, vida_menos, 940, 100);
		draw_sprite(fondo, vida_menos, 1000, 100);
		draw_sprite(fondo, vida_menos, 1060, 100);
		draw_sprite(fondo, vida_menos, 1120, 100);
		break;

	case 2: // Dibuja 3 corazones grises y 2 rojos (2 vidas)

		draw_sprite(fondo, vida, 880, 100);
		draw_sprite(fondo, vida, 940, 100);
		draw_sprite(fondo, vida_menos, 1000, 100);
		draw_sprite(fondo, vida_menos, 1060, 100);
		draw_sprite(fondo, vida_menos, 1120, 100);
		break;

	case 3: // Dibuja 2 corazones grises y 3 rojos (3 vidas)

		draw_sprite(fondo, vida, 880, 100);
		draw_sprite(fondo, vida, 940, 100);
		draw_sprite(fondo, vida, 1000, 100);
		draw_sprite(fondo, vida_menos, 1060, 100);
		draw_sprite(fondo, vida_menos, 1120, 100);
		break;

	case 4: // Dibuja 1 corazon gris y 4 rojos (4 vidas)

		draw_sprite(fondo, vida, 880, 100);
		draw_sprite(fondo, vida, 940, 100);
		draw_sprite(fondo, vida, 1000, 100);
		draw_sprite(fondo, vida, 1060, 100);
		draw_sprite(fondo, vida_menos, 1120, 100);
		break;

	case 5: // Dibuja 5 corazones rojos (5 vidas)

		draw_sprite(fondo, vida, 880, 100);
		draw_sprite(fondo, vida, 940, 100);
		draw_sprite(fondo, vida, 1000, 100);
		draw_sprite(fondo, vida, 1060, 100);
		draw_sprite(fondo, vida, 1120, 100);
		break;

	default:;
		break;
	}
}
////////////////////

////////////////////-MOVIMIENTO DEL JUGADOR
void movimiento(char mapa_matriz[MAXFILAS][MAXCOL], Jugador *player)
{

	if (player->dir == 0)
	{
		pj = load_bitmap("media/bunnyder.bmp", NULL);
	}
	if (player->dir == 1)
	{
		pj = load_bitmap("media/bunnyizq.bmp", NULL);
	}
	if (player->dir == 2)
	{
		pj = load_bitmap("media/bunnyder.bmp", NULL);
	}
	if (player->dir == 3)
	{
		pj = load_bitmap("media/bunnyizq.bmp", NULL);
	}

	if (key[KEY_A])
	{ // MOVIMIENTO A LA IZQUIERDA

		player->dir = 1;

		if ((player->posx) % 30 != 0)
		{
			player->posx -= player->vel;
		}
		else
		{
			player->posx -= player->vel;
			if (mapa_matriz[(player->posy / 30)][((player->posx / 30))] == 'x' || mapa_matriz[(player->posy + 29) / 30][(player->posx / 30)] == 'x' || mapa_matriz[(player->posy / 30)][((player->posx / 30))] == 'b' || mapa_matriz[(player->posy + 29) / 30][(player->posx / 30)] == 'b')
				player->posx += player->vel;
		}
		pj = load_bitmap("media/bunnyizq2.bmp", NULL);
	}

	if (key[KEY_D])
	{ // MOVIMIENTO A LA DERECHA

		player->dir = 0;

		if ((player->posx) % 30 != 0)
		{
			player->posx += player->vel;
		}
		else
		{
			player->posx += player->vel;
			if (mapa_matriz[(player->posy / 30)][((player->posx) / 30) + 1] == 'x' || mapa_matriz[(player->posy + 29) / 30][(player->posx / 30) + 1] == 'x' || mapa_matriz[(player->posy / 30)][((player->posx) / 30) + 1] == 'b' || mapa_matriz[(player->posy + 29) / 30][(player->posx / 30) + 1] == 'b')
				player->posx -= player->vel;
		}
		pj = load_bitmap("media/bunnyder2.bmp", NULL);
	}

	if (key[KEY_W])
	{ // MOVIMIENTO ARRIBA

		player->dir = 2;

		if ((player->posy) % 30 != 0)
		{
			player->posy -= player->vel;
		}
		else
		{
			player->posy -= player->vel;
			if (mapa_matriz[(player->posy / 30)][(player->posx / 30)] == 'x' || mapa_matriz[(player->posy) / 30][(player->posx + 29) / 30] == 'x' || mapa_matriz[(player->posy / 30)][(player->posx / 30)] == 'b' || mapa_matriz[(player->posy) / 30][(player->posx + 29) / 30] == 'b')
				player->posy += player->vel;
		}
	}

	if (key[KEY_S])
	{ // MOVIMIENTO ABAJO

		player->dir = 3;

		if ((player->posy) % 30 != 0)
		{
			player->posy += player->vel;
		}
		else
		{
			player->posy += player->vel;
			if (mapa_matriz[(player->posy / 30) + 1][(player->posx / 30)] == 'x' || mapa_matriz[(player->posy / 30) + 1][(player->posx + 29) / 30] == 'x' || mapa_matriz[(player->posy / 30) + 1][(player->posx / 30)] == 'b' || mapa_matriz[(player->posy / 30) + 1][(player->posx + 29) / 30] == 'b')
				player->posy -= player->vel;
		}
	}
}
////////////////////

////////////////////-lEE ENEMIGOS DEL ARCHIVO TXT Y LOS INICIALIZA
int leer_enemigos(Enemigo npc[20], char mapa_matriz[MAXFILAS][MAXCOL])
{

	int i, j;	  // contadores
	int cont = 0; // contador de cantidad de enemigo que sera regresada en el retur a la variable en el main 'cont_enemigos'

	for (i = 0; i < MAXFILAS; i++)
	{ // Ciclo for anidado en un ciclo for para la inicializacion de los enemigos dependiendo de la matriz
		for (j = 0; j < MAXCOL; j++)
		{

			if (mapa_matriz[i][j] == 'e')
			{ // Si detecta una 'e' en el mapa comienza la inicializacion de los enemigos

				npc[cont].posx = j * 30;		   // posicion inicial enemigo X
				npc[cont].posy = i * 30;		   // posicion inicial enemigo y
				npc[cont].vel = 4.5;			   // velocidad del enemigo
				npc[cont].direccion = 1;		   // direccion donde iniciara moviendose el enemigo segun la colision
				npc[cont].tipo = (rand() % 2) + 1; // tipo de movimiento (Arriba,Abajo,Izq,Der) de manera al azar
				cont++;							   // incremento
			}
		}
	}

	return cont; // devolucion del valor cont para el uso de otra funcion
}
////////////////////

////////////////////-MOVIMIENTO DEL ENEMIGO
void movimiento_enemigos(Enemigo npc[20], int cont_enemigos)
{

	int i, j; // contadores

	for (i = 0; i < cont_enemigos; i++)
	{

		if (npc[i].tipo == 1)
		{ // Incrementa el movimiento positivo segun direccion en X
			npc[i].posx += npc[i].direccion * npc[i].vel;
			malo = load_bitmap("media/enemigoloop.bmp", NULL);
		}

		if (npc[i].tipo == 2)
		{ // Incrementa el movimiento positivo segun direccion en Y
			npc[i].posy -= npc[i].direccion * npc[i].vel;
			malo = load_bitmap("media/enemigo.bmp", NULL);
		}
		draw_sprite(fondo, malo, npc[i].posx, npc[i].posy);
	}
}
////////////////////

////////////////////-COLISION DEL ENEMIGO Y CAMBIO DE DIRECCION
void colision_enemigos(Enemigo npc[20], char mapa_matriz[MAXFILAS][MAXCOL], int cont_enemigos)
{

	int i; // contador

	for (i = 0; i < cont_enemigos; i++)
	{ // Ciclo for encargado de iniciar el funcionamiento de las colisiones

		if (npc[i].tipo == 1)
		{ // Los de TIPO 1 cambian a TIPO 2 al encontrar un obstaculo y estos cambian de direccion de movimiento

			if (mapa_matriz[(npc[i].posy / 30)][((npc[i].posx) / 30) + 1] == 'x' || mapa_matriz[(npc[i].posy + 29) / 30][(npc[i].posx / 30) + 1] == 'x' || mapa_matriz[(npc[i].posy / 30)][((npc[i].posx) / 30) + 1] == 'b' || mapa_matriz[(npc[i].posy + 29) / 30][(npc[i].posx / 30) + 1] == 'b')
			{

				npc[i].direccion = npc[i].direccion * -1;
			}

			if (mapa_matriz[(npc[i].posy / 30)][((npc[i].posx / 30))] == 'x' || mapa_matriz[(npc[i].posy + 29) / 30][(npc[i].posx / 30)] == 'x' || mapa_matriz[(npc[i].posy / 30)][((npc[i].posx / 30))] == 'b' || mapa_matriz[(npc[i].posy + 29) / 30][(npc[i].posx / 30)] == 'b')
			{

				npc[i].direccion = npc[i].direccion * -1;
			}
		}

		if (npc[i].tipo == 2)
		{ // Los de TIPO 2 cambian a TIPO 1 al encontrar un obstaculo y estos cambian de direccion de movimiento

			if (mapa_matriz[(npc[i].posy / 30)][(npc[i].posx / 30)] == 'x' || mapa_matriz[(npc[i].posy) / 30][(npc[i].posx + 29) / 30] == 'x' || mapa_matriz[(npc[i].posy / 30)][(npc[i].posx / 30)] == 'b' || mapa_matriz[(npc[i].posy) / 30][(npc[i].posx + 29) / 30] == 'b')
			{

				npc[i].direccion = npc[i].direccion * -1;
			}
			if (mapa_matriz[(npc[i].posy / 30) + 1][(npc[i].posx / 30)] == 'x' || mapa_matriz[(npc[i].posy / 30) + 1][(npc[i].posx + 29) / 30] == 'x' || mapa_matriz[(npc[i].posy / 30) + 1][(npc[i].posx / 30)] == 'b' || mapa_matriz[(npc[i].posy / 30) + 1][(npc[i].posx + 29) / 30] == 'b')
			{

				npc[i].direccion = npc[i].direccion * -1;
			}
		}
	}
}
////////////////////

////////////////////-COLISION DEL JUGADOR CON EL ENEMIGO
void colision_enemigos_jugador(Jugador *player, Enemigo npc[20], int cont_enemigos)
{

	int i; // Contadores

	for (i = 0; i < cont_enemigos; i++)
	{ // Ciclo for encargado de comprobar las colisiones del enemigo y el jugador

		if (player->posx / 30 == npc[i].posx / 30 && player->posy / 30 == npc[i].posy / 30)
		{ // Si el jugador colisiona con algun enemigo

			// Vuelve a la posicion de X e Y iniciales
			player->posx = 60;
			player->posy = 660;

			player->vidas--; // Se descuentan vidas

			player->pts = player->pts - 1; // Colisionar con enemigos reduce puntos
		}
		if (player->posx / 30 == npc[i].posx / 30 && (player->posy + 25) / 30 == npc[i].posy / 30)
		{ // Si el jugador colisiona con algun enemigo

			// Vuelve a la posicion de X e Y iniciales
			player->posx = 60;
			player->posy = 660;

			player->vidas--; // Se descuentan vidas

			player->pts = player->pts - 1; // Colisionar con enemigos reduce puntos
		}
		if ((player->posx + 15) / 30 == npc[i].posx / 30 && player->posy / 30 == npc[i].posy / 30)
		{ // Si el jugador colisiona con algun enemigo

			// Vuelve a la posicion de X e Y iniciales
			player->posx = 60;
			player->posy = 660;

			player->vidas--; // Se descuentan vidas

			player->pts = player->pts - 1; // Colisionar con enemigos reduce puntos
		}
	}

	if (player->pts <= 0)
	{ // limite de reduccion de puntos que sean 0
		player->pts = 0;
	}
}
////////////////////

////////////////////-LEE LOS OBJETOS DEL ARCHIVO TXT Y LOS INICIALIZA CON POSICION AL AZAR
int leer_item_z(Item zanah[15], char mapa_matriz[MAXFILAS][MAXCOL])
{

	int i, j;		   // contador
	int cont_item = 0; // contador de item para devolucion fuera de la funcion para el uso de otra funcion

	for (i = 0; i < MAXFILAS; i++)
	{ // Ciclo for anidado en otro for para inicializar las posiciones de los objetos
		for (j = 0; j < MAXCOL; j++)
		{

			if (mapa_matriz[i][j] == 'z')
			{ // si encuentra una 'z' que es un objeto representado como zanahoria

				zanah[cont_item].posx = (rand() % 22) + 1; // Al azar da un valor a la posicion x
				zanah[cont_item].posy = (rand() % 22) + 1; // Al azar da un valor a la posicion y

				// Limitante por si se encuentra un obstaculo recalcular numeros al azar para las posisiones
				while (mapa_matriz[zanah[cont_item].posy][zanah[cont_item].posx] == 'x' || mapa_matriz[zanah[cont_item].posy][zanah[cont_item].posx] == 'b' || mapa_matriz[zanah[cont_item].posy][zanah[cont_item].posx] == 'e')
				{
					zanah[cont_item].posx = (rand() % 22) + 1;
					zanah[cont_item].posy = (rand() % 22) + 1;
				}

				// Se multiplican las posiciones para que esten de acuerdo con los bloques que serian en 30x30
				zanah[cont_item].posx *= 30;
				zanah[cont_item].posy *= 30;

				zanah[cont_item].toca = false; // Inicializa en false para usarse en las colisiones con el jugador en otra funcion
				cont_item++;				   // contador para devolver el valor de cuantos objetos hay
			}
		}
	}

	return cont_item; // retorno del valor del contador de objetos en mapa
}
////////////////////

////////////////////-DIBUJA EL OBJETO EN PANTALLA
void dibuja_item_z(Item zanah[15], int cont_items)
{

	int i = 0; // contador

	for (i = 0; i < cont_items; i++)
	{ // ciclo for que usa el cont_items para dibujar los objetos hasta la cantidad de objetos que existen en el mapa

		if (zanah[i].toca == false)
		{ // Si no es colisionado por el jugador

			draw_sprite(fondo, item1, zanah[i].posx, zanah[i].posy); // se dibuja en la posicion correspondiente
		}
	}
}
////////////////////

////////////////////-COLISION DEL JUGADOR CON EL ITEM
int col_jugador_item(Jugador *player, Item zanah[15], int cont_items, char mapa_matriz[MAXFILAS][MAXCOL])
{

	int i = 0, j = 0; // contador
	int cont_enemigos;

	for (i = 0; i < cont_items; i++)
	{

		// Entra al if si la posicion del jugador es la misma que la del objeto y si este objeto no fue tocado con anterioridad
		if (player->posx / 30 == zanah[i].posx / 30 && player->posy / 30 == zanah[i].posy / 30 && zanah[i].toca == false)
		{ // abajo

			player->pts += 3;	  // suma puntos
			zanah[i].toca = true; // cambia a tocado el objeto
			conteotoca++;		  // conteo de cuantas toca en mapa para el reinicio de los objetos y reiniciar la cantidad
		}
		if (player->posx / 30 == zanah[i].posx / 30 && (player->posy + 15) / 30 == zanah[i].posy / 30 && zanah[i].toca == false)
		{ // abajo

			player->pts += 3;	  // suma puntos
			zanah[i].toca = true; // cambia a tocado el objeto
			conteotoca++;		  // conteo de cuantas toca en mapa para el reinicio de los objetos y reiniciar la cantidad
		}
		if ((player->posx + 25) / 30 == zanah[i].posx / 30 && player->posy / 30 == zanah[i].posy / 30 && zanah[i].toca == false)
		{ // abajo

			player->pts += 3;	  // suma puntos
			zanah[i].toca = true; // cambia a tocado el objeto
			conteotoca++;		  // conteo de cuantas toca en mapa para el reinicio de los objetos y reiniciar la cantidad
		}

		if (conteotoca == 9)
		{ // una vez se hayan tocado todos los objetos del mapa

			dibuja_item_z(zanah, cont_items);			  // los vuelve a dibujar en posiciones diferentes
			cont_items = leer_item_z(zanah, mapa_matriz); // vuelve a leer la cantidad original de los objetos
			conteotoca = 0;								  // se reinicia el conteo de item para repetir el proceso
			n++;
		}
	}
	return n;
}
////////////////////

////////////////////-DIBUJA AL JUGADOR Y EL FONDO
void pantalla(Jugador p)
{

	draw_sprite(fondo, pj, p.posx, p.posy);		 // dibuja el personaje
	blit(fondo, screen, 0, 0, 0, 0, P_AN, P_AL); // Imprime el fondo
}
////////////////////

void menu(int op,tablero rank[5])
{

	int inicia = 0;
	int vepuntos = 0;

	do
	{
		if (key[KEY_S] && op != 2)
		{
			op++;
			rest(80);
		}
		if (key[KEY_W] && op != 0)
		{
			op--;
			rest(80);
		}

		if (op == 0)
		{
			fondo = load_bitmap("media/menu1.bmp", NULL);
			blit(fondo, screen, 0, 0, 0, 0, P_AN, P_AL);

			if (key[KEY_SPACE])
			{
				inicia = 1;
			}
		}
		if (op == 1)
		{
			fondo = load_bitmap("media/menu2.bmp", NULL);
			blit(fondo, screen, 0, 0, 0, 0, P_AN, P_AL);

			if (key[KEY_SPACE])
			{

				fondo = load_bitmap("media/pizarra.bmp", NULL);
				blit(fondo, screen, 0, 0, 0, 0, P_AN, P_AL);
				
				textprintf_ex(screen, font, 550, 150, makecol(255, 255, 255), -1, "%s ~ %d pts",rank[0].nombre, rank[0].pts);
				textprintf_ex(screen, font, 550, 250, makecol(255, 255, 255), -1, "%s ~ %d pts",rank[1].nombre, rank[1].pts);
				textprintf_ex(screen, font, 550, 350, makecol(255, 255, 255), -1, "%s ~ %d pts",rank[2].nombre, rank[2].pts);
				textprintf_ex(screen, font, 550, 450, makecol(255, 255, 255), -1, "%s ~ %d pts",rank[3].nombre, rank[3].pts);
				textprintf_ex(screen, font, 550, 550, makecol(255, 255, 255), -1, "%s ~ %d pts",rank[4].nombre, rank[4].pts);
				
				rest(10000);
			}
		}
		if (op == 2)
		{
			fondo = load_bitmap("media/menu3.bmp", NULL);
			blit(fondo, screen, 0, 0, 0, 0, P_AN, P_AL);

			if (key[KEY_SPACE])
			{
				allegro_exit();
			}
		}

	} while (inicia != 1);
}

int leer_trampa(Trampa trampa[15], char mapa_matriz[MAXFILAS][MAXCOL])
{

	int i, j;			 // contador
	int cont_trampa = 0; // contador de item para devolucion fuera de la funcion para el uso de otra funcion

	for (i = 0; i < MAXFILAS; i++)
	{ // Ciclo for anidado en otro for para inicializar las posiciones de los objetos
		for (j = 0; j < MAXCOL; j++)
		{

			if (mapa_matriz[i][j] == 't')
			{ // si encuentra una 'z' que es un objeto representado como zanahoria

				trampa[cont_trampa].posx = (rand() % 22) + 1; // Al azar da un valor a la posicion x
				trampa[cont_trampa].posy = (rand() % 22) + 1; // Al azar da un valor a la posicion y

				// Limitante por si se encuentra un obstaculo recalcular numeros al azar para las posisiones
				while (mapa_matriz[trampa[cont_trampa].posy][trampa[cont_trampa].posx] == 'x' || mapa_matriz[trampa[cont_trampa].posy][trampa[cont_trampa].posx] == 'b' || mapa_matriz[trampa[cont_trampa].posy][trampa[cont_trampa].posx] == 'e' || mapa_matriz[trampa[cont_trampa].posy][trampa[cont_trampa].posx] == 'z')
				{
					trampa[cont_trampa].posx = (rand() % 22) + 1;
					trampa[cont_trampa].posy = (rand() % 22) + 1;
				}

				// Se multiplican las posiciones para que esten de acuerdo con los bloques que serian en 30x30
				trampa[cont_trampa].posx *= 30;
				trampa[cont_trampa].posy *= 30;

				trampa[cont_trampa].toca = false; // Inicializa en false para usarse en las colisiones con el jugador en otra funcion
				cont_trampa++;					  // contador para devolver el valor de cuantos objetos hay
			}
		}
	}

	return cont_trampa;
}
void dibuja_trampa(Trampa t[15], int conttrampa)
{

	int i = 0; // contador

	for (i = 0; i < conttrampa; i++)
	{ // ciclo for que usa el cont_items para dibujar los objetos hasta la cantidad de objetos que existen en el mapa

		if (t[i].toca == false)
		{ // Si no es colisionado por el jugador

			draw_sprite(fondo, trampaimg, t[i].posx, t[i].posy); // se dibuja en la posicion correspondiente
		}
	}
}

void col_jugador_trampa(Jugador *player, Trampa trampa[15], char mapa_matriz[MAXFILAS][MAXCOL], int conttrampa)
{

	int i = 0, j = 0; // contador
	int cont_enemigos;

	for (i = 0; i < conttrampa; i++)
	{

		// Entra al if si la posicion del jugador es la misma que la del objeto y si este objeto no fue tocado con anterioridad
		if (player->posx / 30 == trampa[i].posx / 30 && player->posy / 30 == trampa[i].posy / 30 && trampa[i].toca == false)
		{

			player->pts -= 2; // suma puntos

			player->vidas--;
			trampa[i].toca = true; // cambia a tocado el objeto
		}
		if (player->posx / 30 == trampa[i].posx / 30 && (player->posy + 15) / 30 == trampa[i].posy / 30 && trampa[i].toca == false)
		{

			player->pts -= 2; // suma puntos

			player->vidas--;
			trampa[i].toca = true; // cambia a tocado el objeto
		}
		if ((player->posx + 15) / 30 == trampa[i].posx / 30 && player->posy / 30 == trampa[i].posy / 30 && trampa[i].toca == false)
		{

			player->pts -= 2; // suma puntos

			player->vidas--;
			trampa[i].toca = true; // cambia a tocado el objeto
		}
	}
	if (player->pts <= 0)
	{ // limite de reduccion de puntos que sean 0
		player->pts = 0;
	}
}

void dibuja_tiempo(int tiempo, int auxtiempo, int auxtiempo2, int auxtiempo3)
{

	draw_sprite(fondo, tiempoimg, 870, 300);
	num1 = create_bitmap(30, 30);
	num2 = create_bitmap(30, 30);
	num3 = create_bitmap(30, 30);

	switch (auxtiempo)
	{
	case 0:
		num1 = load_bitmap("media/num/0.bmp", NULL);
		;
		break;
	case 1:
		num1 = load_bitmap("media/num/1.bmp", NULL);
		;
		break;
	case 2:
		num1 = load_bitmap("media/num/2.bmp", NULL);
		;
		break;
	case 3:
		num1 = load_bitmap("media/num/3.bmp", NULL);
		;
		break;
	case 4:
		num1 = load_bitmap("media/num/4.bmp", NULL);
		;
		break;
	case 5:
		num1 = load_bitmap("media/num/5.bmp", NULL);
		;
		break;
	case 6:
		num1 = load_bitmap("media/num/6.bmp", NULL);
		;
		break;
	case 7:
		num1 = load_bitmap("media/num/7.bmp", NULL);
		;
		break;
	case 8:
		num1 = load_bitmap("media/num/8.bmp", NULL);
		;
		break;
	case 9:
		num1 = load_bitmap("media/num/9.bmp", NULL);
		;
		break;
	default:
		num1 = load_bitmap("media/num/0.bmp", NULL);
		;
		break;
	}

	switch (auxtiempo2)
	{
	case 0:
		num2 = load_bitmap("media/num/0.bmp", NULL);
		;
		break;
	case 1:
		num2 = load_bitmap("media/num/1.bmp", NULL);
		;
		break;
	case 2:
		num2 = load_bitmap("media/num/2.bmp", NULL);
		;
		break;
	case 3:
		num2 = load_bitmap("media/num/3.bmp", NULL);
		;
		break;
	case 4:
		num2 = load_bitmap("media/num/4.bmp", NULL);
		;
		break;
	case 5:
		num2 = load_bitmap("media/num/5.bmp", NULL);
		;
		break;
	case 6:
		num2 = load_bitmap("media/num/6.bmp", NULL);
		;
		break;
	case 7:
		num2 = load_bitmap("media/num/7.bmp", NULL);
		;
		break;
	case 8:
		num2 = load_bitmap("media/num/8.bmp", NULL);
		;
		break;
	case 9:
		num2 = load_bitmap("media/num/9.bmp", NULL);
		;
		break;
	default:
		num2 = load_bitmap("media/num/0.bmp", NULL);
		;
		break;
	}

	switch (auxtiempo3)
	{
	case 0:
		num3 = load_bitmap("media/num/0.bmp", NULL);
		;
		break;
	case 1:
		num3 = load_bitmap("media/num/1.bmp", NULL);
		;
		break;
	case 2:
		num3 = load_bitmap("media/num/2.bmp", NULL);
		;
		break;
	case 3:
		num3 = load_bitmap("media/num/3.bmp", NULL);
		;
		break;
	case 4:
		num3 = load_bitmap("media/num/4.bmp", NULL);
		;
		break;
	case 5:
		num3 = load_bitmap("media/num/5.bmp", NULL);
		;
		break;
	case 6:
		num3 = load_bitmap("media/num/6.bmp", NULL);
		;
		break;
	case 7:
		num3 = load_bitmap("media/num/7.bmp", NULL);
		;
		break;
	case 8:
		num3 = load_bitmap("media/num/8.bmp", NULL);
		;
		break;
	case 9:
		num3 = load_bitmap("media/num/9.bmp", NULL);
		;
		break;
	default:
		num3 = load_bitmap("media/num/0.bmp", NULL);
		;
		break;
	}

	draw_sprite(fondo, num3, 950, 310);
	draw_sprite(fondo, num2, 1000, 310);
	draw_sprite(fondo, num1, 1050, 310);

	clear_bitmap(num1);
	clear_bitmap(num2);
	clear_bitmap(num3);
}

void fin(Jugador player)
{

	fondo = load_bitmap("media/over.bmp", NULL);
	blit(fondo, screen, 0, 0, 0, 0, P_AN, P_AL);
}

int leer_enemigo_int(EnemigoINT npcINT[3], char mapa[MAXFILAS][MAXCOL])
{

	int i, j;
	int cont = 0;

	for (i = 0; i < MAXFILAS; i++)
	{
		for (j = 0; j < MAXCOL; j++)
		{

			if (mapa[i][j] == 'i')
			{ // Si detecta una 'e' en el mapa comienza la inicializacion de los enemigos

				npcINT[cont].posx = j * 30; // posicion inicial enemigo X
				npcINT[cont].posy = i * 30; // posicion inicial enemigo y
				npcINT[cont].vel = 1;		// velocidad del enemigo
				npcINT[cont].direccion = 1; // direccion donde iniciara moviendose el enemigo segun la colision
				cont++; // incremento
			}
		}
	}

	return cont;
}

void movimiento_enemigo_int(Jugador player, EnemigoINT npcINT[3], int cont_enemigo_int, char mapa_matriz[MAXFILAS][MAXCOL])
{

	int i = 0;

	// direccion 1 derecha 2 izquierda 3 arriba 4 abajo

	for (i = 0; i < cont_enemigo_int; i++)
	{

		if (player.posx < npcINT[i].posx)
		{
			npcINT[i].posx -= npcINT[i].direccion * npcINT[i].vel;

			if (mapa_matriz[(npcINT[i].posy / 30)][(npcINT[i].posx / 30)] == 'x' || mapa_matriz[(npcINT[i].posy + 29) / 30][(npcINT[i].posx / 30)] == 'x' || mapa_matriz[(npcINT[i].posy / 30)][((npcINT[i].posx / 30))] == 'b' || mapa_matriz[(npcINT[i].posy + 29) / 30][(npcINT[i].posx / 30)] == 'b')
			{

				npcINT[i].posx += npcINT[i].vel;
			}
		}

		if (player.posx > npcINT[i].posx)
		{
			npcINT[i].posx += npcINT[i].direccion * npcINT[i].vel;

			if (mapa_matriz[(npcINT[i].posy / 30)][((npcINT[i].posx) / 30) + 1] == 'x' || mapa_matriz[(npcINT[i].posy + 29) / 30][(npcINT[i].posx / 30) + 1] == 'x' || mapa_matriz[(npcINT[i].posy / 30)][((npcINT[i].posx) / 30) + 1] == 'b' || mapa_matriz[(npcINT[i].posy + 29) / 30][(npcINT[i].posx / 30) + 1] == 'b')
			{

				npcINT[i].posx -= npcINT[i].vel;
			}
		}

		if (player.posy < npcINT[i].posy)
		{
			npcINT[i].posy -= npcINT[i].direccion * npcINT[i].vel;

			if (mapa_matriz[(npcINT[i].posy / 30)][(npcINT[i].posx / 30)] == 'x' || mapa_matriz[(npcINT[i].posy) / 30][(npcINT[i].posx + 29) / 30] == 'x' || mapa_matriz[(npcINT[i].posy / 30)][(npcINT[i].posx / 30)] == 'b' || mapa_matriz[(npcINT[i].posy) / 30][(npcINT[i].posx + 29) / 30] == 'b')
				npcINT[i].posy += npcINT[i].vel;
		}
		if (player.posy > npcINT[i].posy)
		{

			npcINT[i].posy += npcINT[i].direccion * npcINT[i].vel;

			if (mapa_matriz[(npcINT[i].posy / 30) + 1][(npcINT[i].posx / 30)] == 'x' || mapa_matriz[(npcINT[i].posy / 30) + 1][(npcINT[i].posx + 29) / 30] == 'x' || mapa_matriz[(npcINT[i].posy / 30) + 1][(npcINT[i].posx / 30)] == 'b' || mapa_matriz[(npcINT[i].posy / 30) + 1][(npcINT[i].posx + 29) / 30] == 'b')
				npcINT[i].posy -= npcINT[i].vel;
		}

		draw_sprite(fondo, maloINT, npcINT[i].posx, npcINT[i].posy);
	}
}

void col_jugador_enemigoINT(Jugador *player, EnemigoINT npc[3])
{

	int i;

	for (i = 0; i < 3; i++)
	{ // Ciclo for encargado de comprobar las colisiones del enemigo y el jugador

		if (player->posx / 30 == npc[i].posx / 30 && player->posy / 30 == npc[i].posy / 30)
		{ // Si el jugador colisiona con algun enemigo

			// Vuelve a la posicion de X e Y iniciales
			player->posx = 60;
			player->posy = 660;

			player->vidas--; // Se descuentan vidas

			player->pts = player->pts - 1; // Colisionar con enemigos reduce puntos
		}
		if (player->posx / 30 == npc[i].posx / 30 && (player->posy + 25) / 30 == npc[i].posy / 30)
		{ // Si el jugador colisiona con algun enemigo

			// Vuelve a la posicion de X e Y iniciales
			player->posx = 60;
			player->posy = 660;

			player->vidas--; // Se descuentan vidas

			player->pts = player->pts - 1; // Colisionar con enemigos reduce puntos
		}
		if ((player->posx + 15) / 30 == npc[i].posx / 30 && player->posy / 30 == npc[i].posy / 30)
		{ // Si el jugador colisiona con algun enemigo

			// Vuelve a la posicion de X e Y iniciales
			player->posx = 60;
			player->posy = 660;

			player->vidas--; // Se descuentan vidas

			player->pts = player->pts - 1; // Colisionar con enemigos reduce puntos
		}
	}

	if (player->pts <= 0)
	{ // limite de reduccion de puntos que sean 0
		player->pts = 0;
	}
}

////////////////////DESTROYS
void destroys()
{

	destroy_bitmap(fondo);
	destroy_bitmap(pj);
	destroy_bitmap(malo);
	destroy_bitmap(maloINT);
	destroy_bitmap(bush);
	destroy_bitmap(borde);
	destroy_bitmap(item1);
	destroy_bitmap(vida);
	destroy_bitmap(vida_menos);
	destroy_bitmap(pausa);
	destroy_bitmap(trampaimg);
	destroy_bitmap(tiempoimg);
	destroy_bitmap(num1);
	destroy_bitmap(num2);
	destroy_bitmap(num3);
}
////////////////////
