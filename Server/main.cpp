#include <iostream>
#include <string>
#include <WinSock2.h>

using namespace std;

//에러 함수
void ShowErrorMessage(string message)
{
	cout << "[오류 발생]: " << message << '\n';
	system("pause");
	exit(1);
}

int main() 
{
	WSADATA wsaData;
	SOCKET serverSocket, clientSocket;
	SOCKADDR_IN serverAddress, clientAddress;

	int serverPort = 9876;
	char received[256];

	//starts up winsock 2
	/*fisrt parameter of wsastartup is the version...?*/
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) //Resets Winsock
		ShowErrorMessage("WSAStartup()");

	serverSocket = socket(PF_INET, SOCK_STREAM, 0); //Creates a TCP Socket
	
	if (serverSocket == INVALID_SOCKET)
		ShowErrorMessage("socket()");

	/* memset = 서버 초기화..? */
	/* htonl, htons : l and ls at the ends stand for long and short...
	   INADDR_ANY */
	
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	// serverport was initialized at the beginning
	serverAddress.sin_port = htons(serverPort);
	if (bind(serverSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
		ShowErrorMessage("bind()");
	cout << "[현재 상태] bind() \n";

	if (listen(serverSocket, 5) == SOCKET_ERROR)
		ShowErrorMessage("listen()");
	cout << "[현재 상태] listen() \n)"; 

	int sizeClientAddress = sizeof(clientAddress);
	clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddress, &sizeClientAddress);
	cout << "[현재 상태] accept()";

	if (clientSocket == INVALID_SOCKET)
		ShowErrorMessage("Accept()");

	while (1) {
		int length = recv(clientSocket, received, sizeof(received), 0);
		received[length-1] = NULL;
		cout << "[클라이언트 메시지]: " << received << "\n";
		cout << "[메시지 전송]: " << received << "\n";
		if (strcmp(received, "[exit]") == 0) //if the string received is 0
		{
			send(clientSocket, received, sizeof(received), 0);
			cout << "[서버 종료]\n";
			break;
		}
		send(clientSocket, received, sizeof(received), 0);
	}

	closesocket(clientSocket);
	closesocket(serverSocket);
	WSACleanup();
	system("pause");
	return 0;
}