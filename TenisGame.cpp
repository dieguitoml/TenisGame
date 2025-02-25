#include <iostream>
#include <cmath>
#include <string>
#include <climits>
#include <cstdlib>
#include <ctime>
#include <cctype>
#include <iomanip>
#include <fstream>
enum tPuntosJuego {
	NADA, QUINCE, TREINTA, CUARENTA, VENTAJA,
	GANADOR
};
const int ANCHO_PISTA = 7;
const int LIM_INF_HAB = 1;
const int LIM_SUP_HAB = 4;
const int LIM_INF_VEL = 1;
const int LIM_SUP_VEL = 4;
const int POS_INIC = (ANCHO_PISTA / 2) + 1;
const int LARGO_PISTA = 3;
const int JUEGOS_SET = 3;
const bool MODO_ALEATORIO = true;
const bool MODO_DEBUG = false;
const int TAM_INICIALES = 3;
const int DIM_ARRAY_TENISTAS = 15;
using tConteoGolpes = int[ANCHO_PISTA + 2];
tPuntosJuego operator+(tPuntosJuego a, int b) {
	return tPuntosJuego((int(a) + b) % 6);

}
struct tPlayer {

	std::string iniciales;
	int habilidad;
	int velocidad;
	tPuntosJuego puntuacion;
	int posicion;
	int juegos;
	int golpes;
	tConteoGolpes destinos;
	int GolpesGanadores;
	int PG;
	int PP;

};
struct tListaPlayers {

	tPlayer jugadores[DIM_ARRAY_TENISTAS];
	int cont;

};

using tTenista = tPlayer[2];
bool cargar(tListaPlayers& listaT) {

	std::ifstream file;
	bool ok = true;
	file.open("tenistas.txt");
	if (file.is_open()) {

		for (listaT.cont = 0; listaT.cont < DIM_ARRAY_TENISTAS

			&& !file.eof(); ++listaT.cont) {
			file >>

				listaT.jugadores[listaT.cont].iniciales >>
				listaT.jugadores[listaT.cont].habilidad >>
				listaT.jugadores[listaT.cont].velocidad >>
				listaT.jugadores[listaT.cont].PG >>
				listaT.jugadores[listaT.cont].PP;

		}

	}
	else ok = false;
	file.close();
	return ok;

}
void guardar(const tListaPlayers& listaT) {

	std::ofstream file;
	file.open("tenistas.txt");
	for (int i = 0; i < listaT.cont; ++i) {

		file << listaT.jugadores[i].iniciales << " " <<

			listaT.jugadores[i].habilidad << " " <<
			listaT.jugadores[i].velocidad << " " << listaT.jugadores[i].PG << " " << listaT.jugadores[i].PP;

			if (i < listaT.cont - 1) {
				file << '\n';

			}

	}
	file.close();

}
void mostrar(const tListaPlayers& listaT) {

	std::cout << " INI HAB VEL PG PP" << '\n';
	for (int i = 0; i < listaT.cont; ++i) {

		std::cout << std::setfill(' ') << std::setw(5) <<

			listaT.jugadores[i].iniciales << std::setw(5) <<
			listaT.jugadores[i].habilidad << std::setw(5) <<
			listaT.jugadores[i].velocidad << std::setw(5) <<
			listaT.jugadores[i].PG << std::setw(5) << listaT.jugadores[i].PP <<
			'\n';
	}

}
void mostrarIniciales(const tListaPlayers& listaT) {
	std::cout << "Iniciales de los tenistas: ";
	for (int i = 0; i < listaT.cont; ++i) {
		if (i < listaT.cont - 1) {

			std::cout << listaT.jugadores[i].iniciales

				<< " - ";

		}
		else {

			std::cout << listaT.jugadores[i].iniciales

				<< " \n";

		}

	}

}
int buscarIniciales(const tListaPlayers& listaT, std::string
	iniciales) {
	int i;
	for (i = 0; i < listaT.cont &&

		listaT.jugadores[i].iniciales != iniciales; ++i);

	return i;

}
void eliminarTenista(tListaPlayers& listaT, std::string iniciales) {

	int numTen = buscarIniciales(listaT, iniciales);
	if (numTen < listaT.cont) {

		for (int i = numTen; i < listaT.cont - 1; i++) {
			listaT.jugadores[i] = listaT.jugadores[i +

				1];

		}
		std::cout << "Tenista elimindo correctamente\n";
		listaT.cont--;
		guardar(listaT);

	}
	else {

		std::cout << "No existe ningun tenista con esas iniciales\n";
	}

}
tPlayer leerDatosTenistas(const tListaPlayers& listaT);
void introducirTenista(tListaPlayers& listaT) {
	if (listaT.cont == DIM_ARRAY_TENISTAS) {

		std::cout << "No hay espacio para crear un nuevo tenista\n";
	}
	else {

		listaT.jugadores[listaT.cont] =

			leerDatosTenistas(listaT);

		listaT.jugadores[listaT.cont].PP = 0;
		listaT.jugadores[listaT.cont].PG = 0;
		listaT.cont++;
		std::cout << "Nuevo tenista creado\n";
		guardar(listaT);

	}

}
bool nombreCorrecto(std::string iniciales) {

	if (iniciales.size() != TAM_INICIALES) return true;
	int i = 0;
	for (i; (i < 3) && (isalpha(iniciales[i])); ++i);
	return i < 3;

}
void inicializarTenista(tPlayer& jugador) {

	jugador.golpes = 0;
	jugador.juegos = 0;
	jugador.GolpesGanadores = 0;
	for (int i = 0; i < ANCHO_PISTA + 2; i++) {

		jugador.destinos[i] = 0;

	}

}
tPlayer leerDatosTenistas(const tListaPlayers& listaT) {

	tPlayer jugador;
	inicializarTenista(jugador);
	std::cout << "- Introduce sus iniciales (" << TAM_INICIALES << " letras):";

	std::cin >> jugador.iniciales;
	while (nombreCorrecto(jugador.iniciales) ||

		(buscarIniciales(listaT, jugador.iniciales) < listaT.cont)) {
		if (nombreCorrecto(jugador.iniciales)) {

			std::cout << "Error: las iniciales tienen que ser tres letras.Por favor, vuelva a introcudir las iniciales :";

			std::cin >> jugador.iniciales;

		}
		else {

			std::cout << "Error: ya existe este jugador.Por favor vuelva a introducir las iniciales : ";
			std::cin >> jugador.iniciales;

		}

	}
	std::cout << "- Introduzca su habilidad (valor entre " <<

		LIM_INF_HAB << " y " << LIM_SUP_HAB << "): ";
	std::cin >> jugador.habilidad;
	while ((jugador.habilidad < LIM_INF_HAB) ||

		(jugador.habilidad > LIM_SUP_HAB)) {

		std::cout << "Error: introduzca un valor adecuado para la habilidad(entre " << LIM_INF_HAB << " y " << LIM_SUP_HAB << "): ";

		std::cin >> jugador.habilidad;

	}
	std::cout << "- Introduzca su velocidad (valor entre " <<

		LIM_INF_VEL << " y " << LIM_SUP_VEL << "): ";
	std::cin >> jugador.velocidad;
	while ((jugador.velocidad < LIM_INF_VEL) ||

		(jugador.velocidad > LIM_SUP_VEL)) {

		std::cout << "Error: introduzca un valor adecuado para la velocidad(entre " << LIM_INF_VEL << " y " << LIM_SUP_VEL <<"): ";
		std::cin >> jugador.velocidad;

	}
	return jugador;

}
void escribeNombreJugador(tTenista a, int activo) {

	std::cout << a[activo].iniciales;

}
std::string marcador(tPuntosJuego p) {

	switch (p) {
	case NADA: return "00";
	case QUINCE: return "15";
	case TREINTA: return "30";
	case CUARENTA: return "40";
	case VENTAJA: return "Ad";
	}

}
void escribeMarcador(tTenista a) {

	std::cout << " ";
	escribeNombreJugador(a, 0);
	std::cout << " " << marcador(a[0].puntuacion) << " - " <<

		marcador(a[1].puntuacion) << " ";
	escribeNombreJugador(a, 1);
	std::cout << "\n";

}
void pintarMarcadorTenista(const tTenista a, int jugador) {
	std::cout << " " << a[jugador].iniciales << " " <<
		a[jugador].juegos << " : " << marcador(a[jugador].puntuacion);
}
void pintarMarcador(tTenista a, int jugador) {

	pintarMarcadorTenista(a, 0);
	if (jugador == 0) std::cout << " *";
	std::cout << "\n";
	pintarMarcadorTenista(a, 1);
	if (jugador == 1) std::cout << " *";
	std::cout << "\n";

}
void pintarJugador0(tTenista a) {

	std::cout << std::setfill(' ') << std::setw((a[0].posicion

		- 1) * 2 + TAM_INICIALES + 1) << a[0].iniciales;

	std::cout << '\n';

}
void pintarLineaFondo() {
	std::cout << " ";
	for (int i = 0; i < ANCHO_PISTA; ++i) {

		std::cout << " -";

	}
	std::cout << '\n';

}

void pintarLineaBola0(int actual, int posicionbola) {
	for (int i = 0; i < posicionbola; ++i) {

		std::cout << " |";

	}
	if (actual == 0) {
		std::cout << "o";

	}
	else std::cout << " ";
	for (int i = 0; i < (ANCHO_PISTA - posicionbola) + 1; ++i)

	{

		std::cout << "| ";

	}
	std::cout << '\n';

}
void pintarPista() {

	for (int i = 0; i < ANCHO_PISTA + 1; ++i) {

		std::cout << " |";

	}
	std::cout << '\n';

}
void pintarRed() {
	std::cout << "-";
	for (int i = 1; i <= ANCHO_PISTA; ++i) {

		std::cout << "-" << i;

	}
	std::cout << "--\n";

}

void pintarLineaBola1(int actual, int posicionbola) {
	for (int i = 0; i < posicionbola; ++i) {

		std::cout << " |";

	}
	if (actual == 1) {
		std::cout << "o";

	}
	else std::cout << " ";
	for (int i = 0; i < (ANCHO_PISTA - posicionbola) + 1; ++i)

	{

		std::cout << "| ";

	}
	std::cout << '\n';

}
void pintarJugador1(tTenista a) {

	std::cout << std::setfill(' ') << std::setw((a[1].posicion

		- 1) * 2 + TAM_INICIALES + 1) << a[1].iniciales;

	std::cout << '\n';

}

void pintarPeloteo(tTenista a, int JugadorActual, int bola) {

	pintarJugador0(a);
	pintarLineaFondo();
	pintarLineaBola0(JugadorActual, bola);
	for (int i = 1; i < LARGO_PISTA; i++) {

		pintarPista();

	}
	pintarRed();
	for (int i = 1; i < LARGO_PISTA; i++) {

		pintarPista();

	}
	pintarLineaBola1(JugadorActual, bola);
	pintarLineaFondo();
	pintarJugador1(a);

}
void mostrarEstadisticas(tPlayer const& jugador) {

	std::cout << "Estadisticas de " << jugador.iniciales <<

		"\n";

	std::cout << " Golpes totales: " << jugador.golpes <<

		"\n";

	std::cout << " Golpes ganadores: " <<

		jugador.GolpesGanadores << "\n";

	std::cout << " Errores no forzados: " <<

		jugador.destinos[0] + jugador.destinos[ANCHO_PISTA + 1] << "\n";
	std::cout << " Distribucion de los golpes en la pista\n";
	std::cout << " Calle";
	for (int i = 0; i < ANCHO_PISTA + 2; i++) {

		std::cout << std::setfill(' ') << std::setw(7) <<

			i;

	}
	std::cout << "\n";
	std::cout << " %";
	for (int i = 0; i < ANCHO_PISTA + 2; i++) {

		std::cout << " ";
		float porcentaje = float(jugador.destinos[i] *

			100) / jugador.golpes;

		if (porcentaje < 10) {

			std::cout << std::fixed <<

				std::setprecision(2) << " " << porcentaje;

		}
		else if (porcentaje < 99) {

			std::cout << std::fixed <<

				std::setprecision(2) << " " << porcentaje;

		}
		else {

			std::cout << std::fixed <<

				std::setprecision(2) << porcentaje;

		}

	}
	std::cout << "\n\n";

}
int distancia_a_destino(int posicion, int destino) {

	return abs(posicion - destino);

}
int golpeoBola(int posicion_tenista, int habilidad) {

	int destino_bola;
	if (MODO_ALEATORIO) {

		destino_bola = (rand() % ANCHO_PISTA + 1);

	}
	else {

		std::cout << "- Introduzca la casilla de destino de la bola : ";

		std::cin >> destino_bola;

	}
	if (MODO_DEBUG) {

		std::cout << " El jugador dispara hacia la calle " << destino_bola << '\n';

	}
	if (distancia_a_destino(posicion_tenista, destino_bola) <=

		habilidad) {

		if (MODO_DEBUG) {

			std::cout << " Ese ha sido un tiro sencillo" << '\n';
		}
		return destino_bola;

	}
	int probabilidad = (100 -

		(((distancia_a_destino(posicion_tenista, destino_bola) - habilidad)
			* 100) / float((ANCHO_PISTA - 1) - LIM_INF_HAB)));

	int golpeo;
	golpeo = rand() % 99;
	if (MODO_DEBUG) {

		std::cout << " Tiro complicado que... (probabilidad = " << probabilidad << " y resultado = " << golpeo << ") ";

	}
	if (golpeo < probabilidad) {
		if (MODO_DEBUG) {

			std::cout << "LLega a su destino!!" <<

				'\n';

		}
		return destino_bola;

	}
	if ((rand() % 2) == 1) {
		if (MODO_DEBUG) {

			std::cout << "No ha sido preciso!!" <<

				'\n';

		}
		return destino_bola + 1;

	}
	if (MODO_DEBUG) {

		std::cout << "No ha sido preciso!!" << '\n';

	}
	return destino_bola - 1;

}

int correTenista(int posicionTenista, int velocidad, int
	posicion_bola) {

	if (distancia_a_destino(posicionTenista, posicion_bola) <= velocidad) return posicion_bola;

	if (posicionTenista < posicion_bola) return posicionTenista + velocidad;
    else return posicionTenista - velocidad;

}
int punto(tTenista a, int JugadorActual) {

	a[0].posicion = POS_INIC, a[1].posicion = POS_INIC;
	int destino, ganador = -1;
	while (ganador == -1) {

		std::cout << "Golpea ";
		escribeNombreJugador(a, JugadorActual);
		std::cout << "\n\n";
		a[JugadorActual].golpes += 1;
		destino = golpeoBola(a[JugadorActual].posicion,

			a[JugadorActual].habilidad);

		a[JugadorActual].destinos[destino] += 1;
		if ((destino < 1) || (destino > ANCHO_PISTA))

			ganador = (JugadorActual + 1) % 2;

		else {

			a[(JugadorActual + 1) % 2].posicion =
				correTenista(a[(JugadorActual + 1) % 2].posicion, a[(JugadorActual +
					1) % 2].velocidad, destino);

			if (destino != a[(JugadorActual + 1) %

				2].posicion) {

				if (MODO_DEBUG) {

					std::cout << " Su rival no llega a la bola\n";

				}
				ganador = JugadorActual;
				a[JugadorActual].GolpesGanadores

					+= 1;

			}
			else {

				if (MODO_DEBUG) {

					std::cout << " Su rival llega a la bola\n";

				}

			}

		}
		JugadorActual = (JugadorActual + 1) % 2;
		pintarPeloteo(a, JugadorActual, destino);
		std::cout << "\n";
		system("pause");

	}
	return ganador;

}
int juego(tTenista a, int saque) {

	a[0].puntuacion = NADA, a[1].puntuacion = NADA;
	int JugadorActual, ganador;

	JugadorActual = saque;
	pintarMarcador(a, saque);
	std::cout << "\n";
	while ((a[0].puntuacion < GANADOR) && (a[1].puntuacion <

		GANADOR)) {

		ganador = punto(a, JugadorActual);
		std::cout << "Punto para ";
		escribeNombreJugador(a, ganador);
		std::cout << "!!!\n\n";
		mostrarEstadisticas(a[0]);
		mostrarEstadisticas(a[1]);
		if (a[ganador].puntuacion != CUARENTA)
			a[ganador].puntuacion = a[ganador].puntuacion + 1;

		else if (a[(ganador + 1) % 2].puntuacion ==
			CUARENTA) a[ganador].puntuacion = a[ganador].puntuacion + 1;
		else if (a[(ganador + 1) % 2].puntuacion ==

			VENTAJA) {

			a[(ganador + 1) % 2].puntuacion =

				CUARENTA;

			a[ganador].puntuacion = CUARENTA;

		}
		else {

			a[ganador].puntuacion = GANADOR;

		}
		if (a[ganador].puntuacion == GANADOR) {

			std::cout << "El ganador del juego es ";
			escribeNombreJugador(a, ganador);
			std::cout << "\n\n";
			system("pause");

		}
		if ((a[0].puntuacion < GANADOR) && (a[1].puntuacion

			< GANADOR)) {

			pintarMarcador(a, saque);
			a[0].posicion = POS_INIC, a[1].posicion =

				POS_INIC;

			pintarPeloteo(a, saque, POS_INIC);
			std::cout << "\n";
			system("pause");
			std::cout << '\n';

		}

	}
	return ganador;

}
void set(tTenista a) {

	inicializarTenista(a[0]);
	inicializarTenista(a[1]);
	for (int i = 0; i < 2; i++) a[i].juegos = 0;
	int saque = rand() % 2, ganador = -1;
	while (ganador == -1) {

		std::cout << "Servicio para ";
		escribeNombreJugador(a, saque);
		std::cout << "\n\n";
		int GanadorJuego = juego(a, saque);

		a[GanadorJuego].juegos += 1;
		if ((a[0].juegos < JUEGOS_SET && a[1].juegos <

			JUEGOS_SET) || (abs(a[0].juegos - a[1].juegos) < 2)) {
			saque = (saque + 1) % 2;

		}
		else {

			std::cout << "\n " << a[0].iniciales <<

				" " << a[0].juegos << "\n";

			std::cout << " " << a[1].iniciales << " " << a[1].juegos << "\n";

				std::cout << "\n";
			std::cout << a[GanadorJuego].iniciales << " se hizo con el partido\n\n";

			std::cout << "Gracias por jugar\n";
			system("pause");
			ganador = GanadorJuego;
			a[GanadorJuego].PG++;
			a[(GanadorJuego + 1) % 2].PP++;

		}

	}

}
void jugarPartido(tListaPlayers& listaT) {

	mostrarIniciales(listaT);
	std::string inic;
	tTenista a;
	int numTen[2];
	std::cout << "Introduce las iniciales del tenista 1: ";
	std::cin >> inic;
	numTen[0] = buscarIniciales(listaT, inic);
	while (numTen[0] == listaT.cont) {

		std::cout << "No existe ningun tenista con esas iniciales\n";

		std::cout << "Introduce las iniciales del tenista 1: ";

		std::cin >> inic;
		numTen[0] = buscarIniciales(listaT, inic);

	}
	a[0] = listaT.jugadores[numTen[0]];
	std::cout << "Introduce las iniciales del tenista 2: ";
	std::cin >> inic;
	numTen[1] = buscarIniciales(listaT, inic);
	while (numTen[1] == listaT.cont || a[0].iniciales == inic) {

		if (numTen[1] == listaT.cont) {

			std::cout << "No existe ningun tenista con esas iniciales\n";

			std::cout << "Introduce las iniciales del tenista 2: ";

			std::cin >> inic;
			numTen[1] = buscarIniciales(listaT, inic);

		}
		if (a[0].iniciales == inic) {

			std::cout << "Tenista repetido. Elija otro\n";

			std::cout << "Introduce las iniciales del tenista 2: ";

			std::cin >> inic;
			numTen[1] = buscarIniciales(listaT, inic);

		}

	}
	a[1] = listaT.jugadores[numTen[1]];
	set(a);
	for (int i = 0; i < 2; i++) {

		listaT.jugadores[numTen[i]].PG = a[i].PG;
		listaT.jugadores[numTen[i]].PP = a[i].PP;

	}
	guardar(listaT);

}
int menu() {

	int opcion = -1;
	std::cout << std::setfill('-') << std::setw(21) << "-";
	std::cout << "\n\n";
	std::cout << "SIMULADOR DE TENIS V3\n\n";
	std::cout << std::setfill('-') << std::setw(21) << "-" << "\n\n";

	std::cout << "<1> Ver datos tenista\n<2> Nuevo tenista\n<3> Eliminar tenista\n<4> Jugar partido\n<0> Salir\n\n\n";

		std::cout << "Opcion: ";
	std::cin >> opcion;
	while ((opcion < 0) || (opcion > 4)) {

		std::cout << "Opcion no valida. Introduzca una opcion: ";

			std::cout << "Opcion: ";
		std::cin >> opcion;

	}
	return opcion;

}
int main() {

	if (MODO_DEBUG) srand(1);
	else srand(time(NULL));
	tListaPlayers listaT;
	if (!cargar(listaT)) {
		listaT.cont = 0;

	}
	bool salir = false;
	while (!salir) {

		switch (menu()) {
		case 1:mostrar(listaT); break;
		case 2:introducirTenista(listaT); break;
		case 3: {

			std::string iniciales;
			mostrarIniciales(listaT);
			std::cout << "Introduzca las iniciales del tenista a eliminar : ";

				std::cin >> iniciales;
			eliminarTenista(listaT, iniciales);

			break;

		}
		case 4:jugarPartido(listaT); break;
		case 0: salir = true;
		}

	}
	return 0;

}