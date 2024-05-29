#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main()
{
    int clienteSocket;
    struct sockaddr_in servidorDireccion;

    clienteSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clienteSocket == -1)
    {
        std::cerr << "No se pudo crear el socket" << std::endl;
        return -1;
    }

    servidorDireccion.sin_family = AF_INET;
    servidorDireccion.sin_port = htons(8080);
    servidorDireccion.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(clienteSocket, (struct sockaddr *)&servidorDireccion, sizeof(servidorDireccion)) < 0)
    {
        std::cerr << "Error al conectar con el servidor" << std::endl;
        close(clienteSocket);
        return -1;
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

    close(clienteSocket);
    return 0;
}