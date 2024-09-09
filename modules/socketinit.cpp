// #include <bits/stdc++.h>
#include<iostream>
#include <winsock2.h>
#include <iomanip>
#include "socketinit.h"
SOCKET socketInit(const char* iPAddress, u_short portNum){
    // This function initializes the socket and returns a connected socket to the user.
    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cerr << "Failed to initialize Winsock." << std::endl;
        return 1;
    }

    // Create a socket
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET)
    {
        std::cerr << "Failed to create socket." << std::endl;
        WSACleanup();
        return 1;
    }

    // Set up the server address and port
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(iPAddress);
    serverAddress.sin_port = htons(portNum);
    // Connect to the server
    if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
    {
        int error = WSAGetLastError();
        std::cerr << "Failed to connect to the server. Error code: " << error << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }
    //std::cout << "connected\n" ;
    return clientSocket;
}

