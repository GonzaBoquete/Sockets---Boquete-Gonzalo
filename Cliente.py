import socket

def main():
    #Definicion de direccion y puerto del servidor
    server_address = '127.0.0.1'
    server_port = 8080

    #Creacion de Socket
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    try:
        #Conectar al Servidor
        sock.connect((server_address, server_port))

        while True:
            print("Menu:")
            print("1. Generar nombre de usuario")
            print("2. Generar contrasena")
            print("3. Salir")

            #Validar que la elección sea un numero
            while True:
                choice_input = input("Ingrese su eleccion: ")
                if choice_input.isdigit():
                    choice = int(choice_input)
                    break
                else:
                    print("Entrada invalida. Por favor ingrese un numero.")

            #Se envia la eleccion al servidor
            sock.sendall(choice.to_bytes(4, byteorder='little'))

            if choice == 1:
                while True:
                    length_input = input("Ingrese la longitud del nombre de usuario: ")
                    if length_input.isdigit():
                        length = int(length_input)
                        break
                    else:
                        print("Entrada invalida. Por favor ingrese un numero.")
                sock.sendall(length.to_bytes(4, byteorder='little'))
            elif choice == 2:
                while True:
                    length_input = input("Ingrese la longitud de la contrasena: ")
                    if length_input.isdigit():
                        length = int(length_input)
                        break
                    else:
                        print("Entrada invalida. Por favor ingrese un numero.")
                sock.sendall(length.to_bytes(4, byteorder='little'))
            elif choice == 3:
                break
            else:
                print("Eleccion invalida")
                continue

            #Se recibe respuesta del servidor
            data = sock.recv(1024)
            print(f"Respuesta del servidor: {data.decode('utf-8')}")

    finally:
        print("Desconectando...")
        sock.close()

if __name__ == "__main__":
    main()