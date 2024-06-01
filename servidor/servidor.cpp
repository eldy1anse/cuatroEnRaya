#include <iostream>
#include <vector>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <thread>
#include <mutex>
#include <algorithm>
#include <random>

class Juego {
public:
    Juego(int clienteSocket) : clienteSocket(clienteSocket), tablero(6, std::vector<char>(7, ' ')) {}

    void enviarMensaje(const std::string &mensaje) {
        send(clienteSocket, mensaje.c_str(), mensaje.size(), 0);
    }

    void enviarMensajeBienvenida() {
        enviarMensaje("¡Bienvenido a cuatro en línea!\n");
    }

    bool verificarJugada(int columna) {
        if (columna < 0 || columna >= 7) {
            return false;
        }
        return tablero[0][columna] == ' ';
    }

    void realizarJugada(int columna, char ficha) {
        for (int i = 5; i >= 0; --i) {
            if (tablero[i][columna] == ' ') {
                tablero[i][columna] = ficha;
                break;
            }
        }
    }

    bool verificarEmpate() {
        for (int j = 0; j < 7; ++j) {
            if (tablero[0][j] == ' ') {
                return false;
            }
        }
        return true;
    }

    bool verificarGanador(char ficha) {
        // Verificar horizontalmente
        for (int i = 0; i < 6; ++i) {
            for (int j = 0; j < 4; ++j) {
                if (tablero[i][j] == ficha && tablero[i][j + 1] == ficha && tablero[i][j + 2] == ficha && tablero[i][j + 3] == ficha) {
                    return true;
                }
            }
        }
        // Verificar verticalmente
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 7; ++j) {
                if (tablero[i][j] == ficha && tablero[i + 1][j] == ficha && tablero[i + 2][j] == ficha && tablero[i + 3][j] == ficha) {
                    return true;
                }
            }
        }
        // Verificar en diagonal
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 4; ++j) {
                if (tablero[i][j] == ficha && tablero[i + 1][j + 1] == ficha && tablero[i + 2][j + 2] == ficha && tablero[i + 3][j + 3] == ficha) {
                    return true;
                }
            }
        }
        // Verificar en diagonal (derecha-izquierda)
        for (int i = 0; i < 3; ++i) {
            for (int j = 3; j < 7; ++j) {
                if (tablero[i][j] == ficha && tablero[i + 1][j - 1] == ficha && tablero[i + 2][j - 2] == ficha && tablero[i + 3][j - 3] == ficha) {
                    return true;
                }
            }
        }
        return false;
    }

    int obtenerColumnaMaquina() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 6);
        int columna;
        do {
            columna = dis(gen);
        } while (!verificarJugada(columna));
        return columna;
    }

    void enviarTablero() {
        std::string mensajeTablero = "\nTablero:\n--------------------------\n\n";

        for (size_t i = 0; i < tablero.size(); ++i) {
            for (size_t j = 0; j < tablero[i].size(); ++j) {
                mensajeTablero += tablero[i][j];
                mensajeTablero += ' ';
            }
            mensajeTablero += '\n';
        }

        // Agregar la numeración de las columnas debajo del tablero
        mensajeTablero += "-----------------\n";
        mensajeTablero += "1 2 3 4 5 6 7\n";

        enviarMensaje(mensajeTablero);
    }

    void manejarJuego() {
        enviarMensajeBienvenida();

        bool clienteComienza = std::rand() % 2 == 0;

        if (clienteComienza) {
            enviarMensaje("Empiezas tú (Cliente)\n");
        } else {
            enviarMensaje("Empieza el Servidor\n");
            int columnaInicial = obtenerColumnaMaquina();
            realizarJugada(columnaInicial, 'S');
            std::cout << "Servidor juega con el cliente en la columna " << columnaInicial + 1 << std::endl << std::flush;
            enviarMensaje("El Servidor juega en la columna " + std::to_string(columnaInicial + 1) + "\n");
            enviarTablero();
        }

        bool finJuego = false;
        while (!finJuego) {
            enviarMensaje("Es tu turno. Introduce el número de la columna (1-7): ");

            char columna_buffer[1024];
            int bytesRecibidos = recv(clienteSocket, columna_buffer, sizeof(columna_buffer), 0);
            if (bytesRecibidos <= 0) {
                std::cerr << "El cliente se desconectó o ocurrió un error." << std::endl;
                break;
            }
            columna_buffer[bytesRecibidos] = '\0';

            int columna = std::stoi(columna_buffer) - 1;
            if (columna < 0 || columna >= 7 || !verificarJugada(columna)) {
                enviarMensaje("Columna inválida. Inténtelo de nuevo.\n");
                continue;
            }
            realizarJugada(columna, 'C');
            std::cout << "El cliente juega en la columna " << columna + 1 << std::endl;
            enviarTablero();

            if (verificarGanador('C')) {
                enviarTablero();
                enviarMensaje("\n¡Felicidades! ¡Has ganado!\nFin del juego.\n");
                std::cout << "El cliente ha ganado la partida y se desconectó." << std::endl;
                finJuego = true;
                break;
            }

            if (verificarEmpate()) {
                enviarTablero();
                enviarMensaje("\n¡El juego ha terminado en empate!\nFin del juego.\n");
                std::cout << "El cliente ha empatado con el servidor y se desconectó." << std::endl;
                finJuego = true;
                break;
            }

            int columnaMaquina = obtenerColumnaMaquina();
            realizarJugada(columnaMaquina, 'S');
            std::cout << "El servidor juega con el cliente en la columna " << columnaMaquina + 1 << std::endl;
            enviarMensaje("\nEl Servidor juega en la columna: " + std::to_string(columnaMaquina + 1) + "\n");
            enviarTablero();

            if (verificarGanador('S')) {
                enviarTablero();
                enviarMensaje("\n¡El servidor ha ganado!\nFin del juego.\n");
                std::cout << "El cliente ha perdido la partida y se desconectó." << std::endl;
                finJuego = true;
                break;
            }
        }

        close(clienteSocket);
    }

private:
    int clienteSocket;
    std::vector<std::vector<char>> tablero;
    std::mutex mutexTablero;
};

class Servidor {
public:
    Servidor(int puerto) : puerto(puerto) {}

    void iniciar() {
        int servidorSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (servidorSocket == -1) {
            std::cerr << "Error al crear el socket del servidor" << std::endl;
            return;
        }

        struct sockaddr_in direccion;
        direccion.sin_family = AF_INET;
        direccion.sin_addr.s_addr = INADDR_ANY;
        direccion.sin_port = htons(puerto);

        if (bind(servidorSocket, (struct sockaddr *)&direccion, sizeof(direccion)) == -1) {
            std::cerr << "Error en bind" << std::endl;
            close(servidorSocket);
            return;
        }

        if (listen(servidorSocket, 5) == -1) {
            std::cerr << "Error en listen" << std::endl;
            close(servidorSocket);
            return;
        }

        std::cout << "Servidor escuchando en el puerto " << puerto << std::endl;

        while (true) {
            struct sockaddr_in clienteDireccion;
            socklen_t clienteLen = sizeof(clienteDireccion);
            int clienteSocket = accept(servidorSocket, (struct sockaddr *)&clienteDireccion, &clienteLen);

            if (clienteSocket == -1) {
                std::cerr << "Error en accept" << std::endl;
                continue;
            }

            std::cout << "Cliente conectado desde " << inet_ntoa(clienteDireccion.sin_addr) << ":" << ntohs(clienteDireccion.sin_port) << std::endl;

            std::thread(&Servidor::manejarCliente, this, clienteSocket, clienteDireccion).detach();
        }

        close(servidorSocket);
    }

private:
    void manejarCliente(int clienteSocket, const sockaddr_in &clienteDireccion) {
        Juego juego(clienteSocket);
        juego.manejarJuego();
    }

    int puerto;
};

int main(int argc, char *argv[]) {
    int puerto = 8080;
    if (argc > 1) {
        puerto = std::stoi(argv[1]);
    }

    Servidor servidor(puerto);
    servidor.iniciar();

    return 0;
}
