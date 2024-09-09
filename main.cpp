#include "./backend.h"

int main() {
    bool isClose = false;
    PMU pmu1("172.26.36.98", 4727, 2);
    //PMU pmu2("172.26.37.162", 4727, 2);
    std::vector<PMU> pmuVector = { pmu1 };
    dbCred db("localhost", "root", "AAPR@203", 3305);
    std::unordered_map<int, data_frame> dfMap;
    std::vector<std::pair<SOCKET, int>> sockets;
    std::unordered_map<int, cfg2_frame> cfgMap;
    backend(isClose, 1, pmuVector, 1, db, dfMap, sockets, cfgMap);
    for (auto x : cfgMap) {
        std::cout << x.first << std::endl;
    }
    for (auto x : sockets) {
        std::cout << x.first << std::endl;
    }

    return 0;
}