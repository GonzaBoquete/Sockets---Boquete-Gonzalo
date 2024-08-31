#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LENGTH 256

//Funcion para verificar si una cadena es un numero
int is_number(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) {
            return 0;
        }
    }
    return 1;
}

int main() {
    //Inicializar Variables
    WSADATA wsaData;
    SOCKET sock;
    struct sockaddr_in server_addr;
    char buffer[MAX_LENGTH];
    char input[MAX_LENGTH];
    int choice, length;

    //Inicializar Winsock
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    //Crear socket
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    //Configurar dirección del servidor
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    //Conectar al servidor
    connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));

    while (1) {
        printf("Menu:\n");
        printf("1. Generar nombre de usuario\n");
        printf("2. Generar contrasena\n");
        printf("3. Salir\n");
        printf("Ingrese su eleccion: ");

        //Leer la entrada del usuario
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';  //Remover el salto de línea

        //Validar que la entrada es un numero
        if (!is_number(input)) {
            printf("Entrada invalida. Por favor ingrese un numero.\n");
            continue;
        }

        choice = atoi(input);
        send(sock, (char*)&choice, sizeof(int), 0);

        switch (choice) {
            case 1:
                printf("Ingrese la longitud del nombre de usuario: ");
                fgets(input, sizeof(input), stdin);
                input[strcspn(input, "\n")] = '\0';  //Remover el salto de línea

                //Validar que la entrada es un numero
                if (!is_number(input)) {
                    printf("Entrada invalida. Por favor ingrese un numero.\n");
                    continue;
                }
                length = atoi(input);
                send(sock, (char*)&length, sizeof(int), 0);
                break;
            case 2:
                printf("Ingrese la longitud de la contrasena: ");
                fgets(input, sizeof(input), stdin);
                input[strcspn(input, "\n")] = '\0';  //Remover el salto de línea

                //Validar que la entrada es un numero
                if (!is_number(input)) {
                    printf("Entrada invalida. Por favor ingrese un numero.\n");
                    continue;
                }
                length = atoi(input);
                send(sock, (char*)&length, sizeof(int), 0);
                break;
            case 3:
                closesocket(sock);
                WSACleanup();
                return 0;
            default:
                printf("Eleccion invalida\n");
                continue;
        }

        //Recibir respuesta del servidor
        recv(sock, buffer, MAX_LENGTH, 0);
        printf("Respuesta del servidor: %s\n", buffer);
        memset(buffer, 0, sizeof(buffer));
    }

    return 0;
}
