// IMPORTANT: Winsock Library ("ws2_32") should be linked

#include <stdio.h>
#include <winsock2.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6000

char menu()
{
	printf("\n");
	printf("Elige la OPCION que desees: \n");
	printf("1. Sumar (2 + 3 + 5) \n");
	printf("2. Mostrar productos \n");
	printf("3. Obtener IP del servidor \n");
	printf("4. Añadir stock a un producto \n"); // NUEVA OPCIÓN
	printf("5. Compra \n"); // NUEVA OPCIÓN
	printf("6. Salir \n\n");
	printf("Opcion: ");
	char opcion = getchar();

	char c; while ((c = getchar()) != '\n' && c != EOF); // Vac�a el buffer de entrada

	return opcion;
}


int main(int argc, char *argv[])
{

	WSADATA wsaData;
	SOCKET s;
	struct sockaddr_in server;
	char sendBuff[512], recvBuff[512];

	printf("\nInitialising Winsock...\n");
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		return -1;
	}

	printf("Initialised.\n");

	//SOCKET creation
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
		WSACleanup();
		return -1;
	}

	printf("Socket created.\n");

	server.sin_addr.s_addr = inet_addr(SERVER_IP);
	server.sin_family = AF_INET;
	server.sin_port = htons(SERVER_PORT);

	//CONNECT to remote server
	if (connect(s, (struct sockaddr*) &server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("Connection error: %d", WSAGetLastError());
		closesocket(s);
		WSACleanup();
		return -1;
	}

	printf("Connection stablished with: %s (%d)\n", inet_ntoa(server.sin_addr),
			ntohs(server.sin_port));

	//SEND and RECEIVE data (CLIENT/SERVER PROTOCOL)
	char c;
	do
	{
		c = menu();
		if (c == '1')
		{
			// SENDING command SUMAR and parameters to the server
			strcpy(sendBuff, "SUMAR"); // el comando que quieres que el servido ejecute
			send(s, sendBuff, sizeof(sendBuff), 0);// lo envias
			strcpy(sendBuff, "2"); //los datos que quieras enviar
			send(s, sendBuff, sizeof(sendBuff), 0);
			strcpy(sendBuff, "3");
			send(s, sendBuff, sizeof(sendBuff), 0);
			strcpy(sendBuff, "5");
			send(s, sendBuff, sizeof(sendBuff), 0);
			strcpy(sendBuff, "SUMAR-END");// la señal de que ya has terminado de enviar datos
			send(s, sendBuff, sizeof(sendBuff), 0);

			// RECEIVING response to command SUMAR from the server
			recv(s, recvBuff, sizeof(recvBuff), 0);// lo que recives del servidor
			printf("Suma = %s \n", recvBuff);
		}

		if (c == '2')
		{
			// MOSTRAR los profudusctos
			strcpy(sendBuff, "MOSTRAR");
			send(s, sendBuff, sizeof(sendBuff), 0);
			do {
				memset(recvBuff, 0, sizeof(recvBuff));
			    recv(s, recvBuff, sizeof(recvBuff), 0);
			    if (strcmp(recvBuff, "MOSTRAR-END") != 0) {
			    	printf("%s", recvBuff);
			                }
			    } while (strcmp(recvBuff, "MOSTRAR-END") != 0);
		}

		if (c == '3')
		{
			// SENDING command IP
			strcpy(sendBuff, "IP");
			send(s, sendBuff, sizeof(sendBuff), 0);
			strcpy(sendBuff, "IP-END");
			send(s, sendBuff, sizeof(sendBuff), 0);

			// RECEIVING response to command IP from the server
			recv(s, recvBuff, sizeof(recvBuff), 0);
			printf("IP del servidor = %s \n", recvBuff);
		}

		if(c == '4') {
			char idStr[20], cantidadStr[20];

			// Leer ID de producto
			printf("Introduce el ID del producto: ");
			fgets(idStr, sizeof(idStr), stdin);
			idStr[strcspn(idStr, "\n")] = '\0'; // Quitar salto de línea

			// Leer cantidad a añadir
			printf("Introduce la cantidad a añadir: ");
			fgets(cantidadStr, sizeof(cantidadStr), stdin);
			cantidadStr[strcspn(cantidadStr, "\n")] = '\0';

			// Enviar comando al servidor
			strcpy(sendBuff, "Anadir stock");
			send(s, sendBuff, sizeof(sendBuff), 0);

			// Enviar ID de producto
			strcpy(sendBuff, idStr);
			send(s, sendBuff, sizeof(sendBuff), 0);

			// Enviar cantidad
			strcpy(sendBuff, cantidadStr);
			send(s, sendBuff, sizeof(sendBuff), 0);

			// Recibir confirmación
			memset(recvBuff, 0, sizeof(recvBuff));
			recv(s, recvBuff, sizeof(recvBuff), 0);
			printf("%s\n", recvBuff);
		}

		if(c == '5') {
					// Enviar comando al servidor
					strcpy(sendBuff, "REALIZAR COMPRA");
					send(s, sendBuff, sizeof(sendBuff), 0);

					while (1) {
						// Recibir petición de ID de producto
						memset(recvBuff, 0, sizeof(recvBuff));
						recv(s, recvBuff, sizeof(recvBuff), 0);
						printf("%s ", recvBuff);

						// Enviar ID de producto
						fgets(sendBuff, sizeof(sendBuff), stdin);
						sendBuff[strcspn(sendBuff, "\n")] = '\0';
						send(s, sendBuff, sizeof(sendBuff), 0);

						// Recibir petición de cantidad
						memset(recvBuff, 0, sizeof(recvBuff));
						recv(s, recvBuff, sizeof(recvBuff), 0);
						printf("%s ", recvBuff);

						// Enviar cantidad
						fgets(sendBuff, sizeof(sendBuff), stdin);
						sendBuff[strcspn(sendBuff, "\n")] = '\0';
						send(s, sendBuff, sizeof(sendBuff), 0);

						// Recibir confirmación de producto añadido o error
						memset(recvBuff, 0, sizeof(recvBuff));
						recv(s, recvBuff, sizeof(recvBuff), 0);
						printf("%s\n", recvBuff);

						// Preguntar si desea continuar
						memset(recvBuff, 0, sizeof(recvBuff));
						recv(s, recvBuff, sizeof(recvBuff), 0);
						printf("%s ", recvBuff);

						fgets(sendBuff, sizeof(sendBuff), stdin);
						sendBuff[strcspn(sendBuff, "\n")] = '\0';
						send(s, sendBuff, sizeof(sendBuff), 0);

						if (sendBuff[0] == 'n' || sendBuff[0] == 'N') {
							// Recibir mensaje final
							memset(recvBuff, 0, sizeof(recvBuff));
							recv(s, recvBuff, sizeof(recvBuff), 0);
							printf("%s\n", recvBuff);
							break;
						}
					}
				}

		if (c == '6')
		{
			// SENDING command EXIT
			strcpy(sendBuff, "EXIT");
			send(s, sendBuff, sizeof(sendBuff), 0);
		}


	}while(c != '6');

	// CLOSING the socket and cleaning Winsock...
	closesocket(s);
	WSACleanup();

	return 0;
}
