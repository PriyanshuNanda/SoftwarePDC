#pragma once
#ifndef BACKEND_H   
#define BACKEND_H
#include "./modules/structures.h"
#include "./modules/connection.h"
#include "./modules/socketinit.h"
#include "./modules/utilities.h"
#include <winsock2.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <map>
#include <unordered_map>
#include <utility>
#include <string>
#include <sstream>
#include <queue>
#include <algorithm>
#include <cmath>
#include <mutex>
#include <mysql.h>
#include<set>
void backend(bool& isClose, int numThreads, std::vector<PMU> pmuVector, bool csvOrDb, dbCred dbCred, std::unordered_map<int, data_frame>dfMap, std::vector<std::pair<SOCKET, int>>& sockets, std::unordered_map<int, cfg2_frame>& cfgMap);
#endif 