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
#include <fstream>
#include <vector>
#include <map>
#include<unordered_map>
#pragma comment(lib, "ws2_32.lib")
#include<mutex>

SOCKET connection(std::string ipAddress, int PORT)
{
    const char *cString = ipAddress.c_str();
    SOCKET clientSocket = socketInit(cString, PORT);
    // closesocket(clientSocket);
    // WSACleanup();

    return clientSocket;
}
void startDataStream(SOCKET clientSocket, int pmuId,bool &isClose,std::vector<data_frame> & dfBuffer,std::unordered_map<int,cfg2_frame>& cfgMap,std::mutex &mtx,std::unordered_map<int,data_frame> &dfMap)
{   
    bool ifCFGAsked =0;
    sendCFGCmdFrame(clientSocket, pmuId);
    cfg2_frame cfg = makeCFG2(clientSocket, pmuId);

    mtx.lock();
    cfgMap[pmuId] = cfg;
    mtx.unlock();

    sendDFCmdFrame(clientSocket, pmuId);
    std::string fileName = writeDataFrameFileHeader(cfg,pmuId);
   
    while (!closeOrNot(isClose,mtx))
    {
        data_frame df = makeDF(clientSocket, cfg, fileName, ifCFGAsked, pmuId,mtx, dfMap);
        mtx.lock();
        dfBuffer.push_back(df);
        mtx.unlock();
        
    }
}
void closeDataStream(std::vector<std::pair<SOCKET,int>> & sockets, bool & isClose,std::mutex &mtx){
    // this check input is for terminal usecases only. Please remove this block till the specified line below.
    int check;
    std::cin>>check;
    if(check==1){ 
        isClose = 1;
    }
    // till here//


    while (!closeOrNot(isClose, mtx)) {}
    for (int i = 0; i < sockets.size(); i++) {
        sendCloseStreamCmdFrame(sockets[i].first, sockets[i].second);
        closesocket(sockets[i].first);
        //std::cout << "socket closed"  << std::endl;
        WSACleanup();
    }
}

