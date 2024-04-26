#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;

const int FILAS = 6;
const int COLUMNAS = 7;
const char VACIO = ' ';
const char servidor = 'S';
const char cliente = 'C';

vector<vector<char>> tablero(FILAS, vector<char>(COLUMNAS, VACIO));

void imprimirTablero() {
    cout << "   1 2 3 4 5 6 7" << endl; //columnas
    for (int i = 0; i < FILAS; ++i) {
        cout << i + 1 << " "; //numero de fila
        for (int j = 0; j < COLUMNAS; ++j) {
            cout << "|" << tablero[i][j];
        }
        cout << "|" << endl;
    }
    cout << "------------------" << endl;
}


bool verificarGanador(char jugador) {
    //verificar filas
    for (int i = 0; i < FILAS; ++i) {
        for (int j = 0; j <= COLUMNAS - 4; ++j) {
            if (tablero[i][j] == jugador && tablero[i][j+1] == jugador && tablero[i][j+2] == jugador && tablero[i][j+3] == jugador) {
                return true;
            }
        }
    }
    //verificar columnas
    for (int j = 0; j < COLUMNAS; ++j) {
        for (int i = 0; i <= FILAS - 4; ++i) {
            if (tablero[i][j] == jugador && tablero[i+1][j] == jugador && tablero[i+2][j] == jugador && tablero[i+3][j] == jugador) {
                return true;
            }
        }
    }
    //verificar diagonales
    for (int i = 0; i <= FILAS - 4; ++i) {
        for (int j = 0; j <= COLUMNAS - 4; ++j) {
            if (tablero[i][j] == jugador && tablero[i+1][j+1] == jugador && tablero[i+2][j+2] == jugador && tablero[i+3][j+3] == jugador) {
                return true;
            }
            if (tablero[i][j+3] == jugador && tablero[i+1][j+2] == jugador && tablero[i+2][j+1] == jugador && tablero[i+3][j] == jugador) {
                return true;
            }
        }
    }
    return false;
}

void turnoServidor() { //simulado
    int columna;
    srand(time(0));
    columna = rand() % COLUMNAS;
    //buscar la fila disponible
    int fila = FILAS - 1;
    while (fila >= 0 && tablero[fila][columna] != VACIO) {
        fila--;
    }
    //si la columna está llena, elegir otra columna aleatoria
    if (fila < 0) {
        turnoServidor();
        return;
    }
    tablero[fila][columna] = servidor;
}

void turnoCliente(int columna) {
    //validar entrada
    if (columna < 1 || columna > COLUMNAS) {
        cout << "La columna ingresada no es valida. Inténtalo de nuevo." << endl;
        return;
    }
    columna--; //convertir a índice de arreglo
    //buscar la fila disponible
    int fila = FILAS - 1;
    while (fila >= 0 && tablero[fila][columna] != VACIO) {
        fila--;
    }
    
    if (fila < 0) {
        cout << "La columna esta llena. Inténtalo de nuevo." << endl;
        return;
    }
    tablero[fila][columna] = cliente;
}

int main() {
    //inicializar el tablero
    for (int i = 0; i < FILAS; ++i) {
        for (int j = 0; j < COLUMNAS; ++j) {
            tablero[i][j] = VACIO;
        }
    }

    //loop para jugar
    bool juegoTerminado = false;
    bool turnoJugador1 = true;
    while (!juegoTerminado) {
        imprimirTablero();
        if (turnoJugador1) {
            cout << "Turno del servidor." << endl;
            turnoServidor();
        } else {
            cout << "Turno del cliente." << endl;
            int columna;
            cout << "Elige una columna (1-7): ";
            cin >> columna;
            turnoCliente(columna);
        }
        //verificar si hay un ganador
        if (verificarGanador(servidor)) {
            imprimirTablero();
            cout << "¡El servidor ha ganado!" << endl;
            juegoTerminado = true;
        } else if (verificarGanador(cliente)) {
            imprimirTablero();
            cout << "¡El cliente ha ganado!" << endl;
            juegoTerminado = true;
        } else {
            //cambiar turno
            turnoJugador1 = !turnoJugador1;
        }
    }

    return 0;
}
