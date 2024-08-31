#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BUFFER_SIZE 1024
#define VOCALS "aeiou"
#define CONSONANTS "bcdfghjklmnpqrstvwxyz"
#define ALPHANUMERIC "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"

void generate_username(char* buffer, int length) {
    char* vocals = VOCALS;
    char* consonants = CONSONANTS;
    int start_with_vocal = rand() % 2;
    int i;

    for (i = 0; i < length; i++) {
        if (start_with_vocal) {
            buffer[i] = vocals[rand() % strlen(vocals)];
            start_with_vocal = 0;
        } else {
            buffer[i] = consonants[rand() % strlen(consonants)];
            start_with_vocal = 1;
        }
    }
    buffer[length] = '\0';
}

void generate_password(char* buffer, int length) {
    char* alphanumeric = ALPHANUMERIC;
    int i;

    for (i = 0; i < length; i++) {
        buffer[i] = alphanumeric[rand() % strlen(alphanumeric)];
    }
    buffer[length] = '\0';
}

int main() {
    //Inicializar Variables
    WSADATA wsaData;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    int addrlen = sizeof(client_addr);
    char buffer[BUFFER_SIZE];
    int username_length, password_length;

    //Inicializar Winsock
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    //Crear socket servidor
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    //Configurar dirección servidor
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    //Bind socket servidor
    bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));

    //Listen socket servidor
    listen(server_socket, 1);

    printf("Servidor iniciado. Esperando conexiones...\n");

    //Aceptar conexión cliente
    client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addrlen);
    printf("Conexion establecida con cliente\n");

    while (1) {
        //Recibir opción del cliente
        int option;
        recv(client_socket, (char*)&option, sizeof(int), 0);

        switch (option) {
            case 1:
                //Recibir longitud de nombre de usuario
                recv(client_socket, (char*)&username_length, sizeof(int), 0);
                if (username_length < 5 || username_length > 15) {
                    char error_message[] = "Longitud de nombre de usuario invalida";
                    send(client_socket, error_message, strlen(error_message), 0);
                    memset(error_message, 0, sizeof(error_message)); //Reset error_message
                    continue;
                }
                //Generar nombre de usuario
                generate_username(buffer, username_length);
                send(client_socket, buffer, username_length, 0);
                memset(buffer, 0, sizeof(buffer)); // Reset buffer
                break;
            case 2:
                //Recibir longitud de contraseña
                recv(client_socket, (char*)&password_length, sizeof(int), 0);
                if (password_length < 8 || password_length > 49) {
                    char error_message[] = "Longitud de contrasena invalida";
                    send(client_socket, error_message, strlen(error_message), 0);
                    memset(error_message, 0, sizeof(error_message)); //Reset error_message
                    continue;
                }
                //Generar contraseña
                generate_password(buffer, password_length);
                send(client_socket, buffer, password_length, 0);
                memset(buffer, 0, sizeof(buffer)); //Reset buffer
                break;

            case 3:
                //Cerrar Socket
                closesocket(client_socket);
                break;
            default:
                printf("Opcion invalida\n");
                char error_message[] = "Opcion invalida";
                send(client_socket, error_message, strlen(error_message), 0);
                memset(error_message, 0, sizeof(error_message)); //Reset error_message
                continue;
        }

        if (option == 3) {
            break;
        }
    }

    closesocket(server_socket);
    WSACleanup();
    return 0;
}
