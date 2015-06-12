#include <iostream>
#include <winsock2.h>
#include <string>
//#include <Windows.h>
using namespace std;

//bool connectToHost(SOCKET& mySocket, int portNum, char* IPAddress); // Connects to a remote host
bool listenForRequestsOnPort(SOCKET& mySocket, int portNo); // Listens for an incoming connection request on port parameter
void closeConnection(SOCKET s); // Close the socket ands cleanup winsock

int main() {
	WSAData version;        // We need to check the version.
	WORD ver = MAKEWORD(2, 2); // Version of winsock required
	int checkVer = WSAStartup(ver, &version);
	if (checkVer != 0) {
		cout << "This version is not supported! - " << WSAGetLastError() << endl;
	} else {
		cout << "Good - Everything fine!" << endl;
	}

	SOCKET mySock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (mySock == INVALID_SOCKET)
		cout << "Socket creation failed!" << endl;
	else
		cout << "Socket creation successful!" << endl;

	//Socket address info
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("10.34.11.41");
	addr.sin_port = htons(6000);
	// Socket info completed!

	// Time to connect!
	int connection = connect(mySock, (SOCKADDR*)&addr, sizeof(addr));
	if (connection == SOCKET_ERROR) {
		cout << "Error - when connecting: " << WSAGetLastError() << endl;
		closesocket(mySock);
		WSACleanup();
	} else
		cout << "Connected!" << endl;
	cout << "This is the Socket Program!" << endl;

	//connectToHost(mySocketC, 6000, "127.0.0.1");
	//listenForRequestsOnPort(mySocketS, 6000);
	//string messageS = "GET / HTTP / 1.1\r\n\r\n";
	string messageS = "TEST";
	//string messageS = "GET ws://echo.websocket.org/?encoding=text HTTP/1.1\r\n\r\n";
	//string messageS = "GET ws://echo.websocket.org/?encoding=text HTTP/1.1\r\nHost: echo.websocket.org\r\nConnection : Upgrade\r\nPragma : no - cache\r\nCache - Control : no - cache\r\nUpgrade : websocket\r\nOrigin : http : \r\nSec - WebSocket - Version : 13\r\nAccept - Encoding : gzip, deflate, sdch\r\nAccept - Language : en - US, en;q = 0.8\r\nSec - WebSocket - Key: EHRh2zCIlsvI2lC4k7 + tdA == \r\nSec - WebSocket - Extensions : permessage - deflate; client_max_window_bits\r\n\r\n";
									 //  ";//"GET / chat HTTP / 1.1\r\n\r\n";
	char messageR[512];
	int msg = send(mySock, messageS.c_str(), sizeof(messageS), 0);
	cout << "Sending..." << endl;
	listenForRequestsOnPort(mySock, 8080);
	int get = recv(mySock, messageR, sizeof(messageR), 0);
	cout << "Receiving..." << endl;
	cout << messageR << endl;
	closeConnection(mySock);
	return 0;
}

void closeConnection(SOCKET s) {
	if (s) // If socket s exists
		closesocket(s); // close the socket
	WSACleanup(); // clean up winsock
}

//bool connectToHost(SOCKET &s, int portNum, char* IPAddress) {
//
//	WORD version = MAKEWORD(2, 2); // Version of winsock
//	int verCor = WSAStartup(version, &wsadataC); // Checking winsock version
//	if (verCor != 0) { // If the version is incorrect
//		WSACleanup(); // Clean up winsock!
//		cout << "Incorrect Version of Winsock" << endl;
//		return false;
//	} else
//		cout << "Version is good!" << endl;
//
//	// Fill out socket info needed to initialize socket
//	SOCKADDR_IN target; // Socket address info structure
//	target.sin_family = AF_INET; // Address is IPv4
//	target.sin_port = htons(portNum); // Port to connect on
//	target.sin_addr.s_addr = inet_addr(IPAddress); // Target ip
//
//
//	s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // Create socket
//	if (s == INVALID_SOCKET) // Socket creation failed
//		return false;
//	//time to try connecting!
//	if (connect(s, (SOCKADDR*)&target, sizeof(target)) == SOCKET_ERROR)
//		return false; //connection failed
//	else
//		return true; // connection successfull
//}
bool listenForRequestsOnPort(SOCKET &s, int portNum) {
	WORD version = MAKEWORD(2, 2); // Version of winsock
	int verCor = WSAStartup(version, &wsadataC); // Checking winsock version
	if (verCor != 0) { // If the version is incorrect
		WSACleanup(); // Clean up winsock!
		cout << "Incorrect Version of Winsock" << endl;
		return false;
	} else
		cout << "Version is good!" << endl;

	SOCKADDR_IN addr; // The structure for an incoming TCP socket
	addr.sin_family = AF_INET; // Address is IPv4
	addr.sin_port = htons(portNum); // Assigns a port to this socket

	addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Listen for requestsa only on loopback address(eventually change to htonl(INADDR_ANY) for any ip address)
	s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //create the socket

	if (s == INVALID_SOCKET) { // Socket creation failed;
		cout << "Socket creation failed!" << endl;
		return false; // stop if the socket couldn't create
	}
	//if (bind(s, (LPSOCKADDR)&addr, sizeof(addr)) == SOCKET_ERROR) { // Can't bind to the same socket more than once
	//cout << "Something is already bound to this port!" << endl;
	//	return false;
	//}
	listen(s, SOMAXCONN); // start listening
}
