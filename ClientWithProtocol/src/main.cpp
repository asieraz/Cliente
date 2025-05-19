/*
 * main.cpp
 *
 *  Created on: 20 may 2025
 *      Author: j.fernandezde
 */


#include "../includes/main.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string.h>
#include <cstring>


#pragma comment(lib, "ws2_32.lib")

SOCKET clientSocket;

bool conectar_al_servidor(const std::string& host, int puerto) {
    WSADATA wsaData;
    struct sockaddr_in serverAddr;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup falló.\n";
        return false;
    }

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Error creando el socket.\n";
        WSACleanup();
        return false;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(puerto);
    int result = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << std::endl;
        return 1;
    }


    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "No se pudo conectar al servidor.\n";
        closesocket(clientSocket);
        WSACleanup();
        return false;
    }

    std::cout << "✅ Conectado al servidor en " << host << ":" << puerto << "\n";
    return true;
}

void iniciar_cliente() {
    char buffer[2048];
    std::string opcion;

    while (true) {
        // Recibir menú
        memset(buffer, 0, sizeof(buffer));
        int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0) {
            std::cerr << "❌ Conexión cerrada por el servidor.\n";
            break;
        }

        std::cout << buffer;

        // Leer opción del usuario
        std::cout << "> ";
        std::getline(std::cin, opcion);

        // Enviar opción al servidor
        send(clientSocket, opcion.c_str(), opcion.size(), 0);

        // Recibir respuesta
        memset(buffer, 0, sizeof(buffer));
        bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0) {
            std::cerr << "❌ Error recibiendo respuesta.\n";
            break;
        }

        std::cout << buffer << std::endl;

        if (opcion == "3") {
            std::cout << "👋 Desconectando...\n";
            break;
        }
    }

    closesocket(clientSocket);
    WSACleanup();
}


