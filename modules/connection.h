#ifndef CONNECTION_H // include guard
#define CONNECTION_H

#include <winsock2.h>
#include <iomanip>
// #include <bits/stdc++.h>
#include <chrono>
#include "cmdframe.h"
#include "socketinit.h"
#include "structures.h"
#include "utilities.h"
#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include<map>
#include<unordered_map>
# include <mutex>
// using namespace std;
#pragma comment(lib, "ws2_32.lib")

//std::vector<char> convertToHex(const char *arr, int size);
SOCKET connection(std::string ipAddress, int PORT);
void startDataStream(SOCKET clientSocket, int pmuId, bool &isClose, std::vector<data_frame> &dfBuffer,std::unordered_map<int,cfg2_frame>& cfgMap,std::mutex &mtx, std::unordered_map<int, data_frame> &dfMap);//,std::map<int,cfg2_frame>& cfgMap
void closeDataStream(std::vector<std::pair<SOCKET, int>> &sockets, bool &isClose,std::mutex &mtx);
#endif /* CONNECTION_H */
