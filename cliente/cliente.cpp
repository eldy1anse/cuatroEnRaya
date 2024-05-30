#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

void error(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    if (argc < 3) {
        std::cerr << "Uso: " << argv[0] << " <direccion_ip> <puerto>" << std::endl;
        return -1;
    }

    const char *direccionIP = argv[1];
    int puerto = std::stoi(argv[2]);

    int clienteSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clienteSocket == -1)
    {
        error("No se pudo crear el socket");
    }

    struct sockaddr_in servidorDireccion;
    std::memset(&servidorDireccion, 0, sizeof(servidorDireccion));
    servidorDireccion.sin_family = AF_INET;
    servidorDireccion.sin_port = htons(puerto);

    if (inet_pton(AF_INET, direccionIP, &servidorDireccion.sin_addr) <= 0)
    {
        error("Dirección IP no válida");
    }

    if (connect(clienteSocket, (struct sockaddr *)&servidorDireccion, sizeof(servidorDireccion)) < 0)
    {
        error("Error al conectar con el servidor");
    }

    char buffer[1024];
    int bytesRecibidos;

    while ((bytesRecibidos = recv(clienteSocket, buffer, sizeof(buffer) - 1, 0)) > 0)
    {
        buffer[bytesRecibidos] = '\0';
        std::cout << buffer;

        if (strstr(buffer, "Es tu turno. Introduce el número de la columna (1-7): "))
        {
            std::string entrada;
            std::getline(std::cin, entrada);
            send(clienteSocket, entrada.c_str(), entrada.size(), 0);
        }
    }

    if (bytesRecibidos == -1)
    {
        error("Error al recibir datos del servidor");
    }

    close(clienteSocket);
    return 0;
}
