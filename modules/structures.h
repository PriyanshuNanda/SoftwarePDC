#ifndef structures_h
#define structures_h
#include <vector>
#include <string>

class PMU
{
public:
    std::string pmuIp;
    int pmuId;
    int port;

    PMU(const std::string &ip, int pt, int id)
    {
        pmuIp = ip;
        pmuId = id;
        port = pt;
    }
};
class dbCred {
public:
    std::string serverAddress;
    std::string userName;
    std::string password;
    int port;
    dbCred(std::string serverAddress_input, std::string userName_input, std::string password_input, int port_input) {
        serverAddress = serverAddress_input;
        userName = userName_input;
        password = password_input;
        port = port_input;
    }
};

class cfg2_frame
{
public:
    int framesize;
    int idcode;
    long int soc;
    long int fracsec;
    long int timeBase;
    int numPmu;
    std::string stn;
    std::string format;
    int phnmr;
    int annmr;
    int dgnmr;
    std::vector<std::string> chnam;
    std::vector<std::pair<std::string, long double>> phunit;
    std::vector<std::pair<std::string, long double>> anunit;
    // vector<vector<string>> digunit; // Two 16-bit words are provided for each digital word.
    int fnom;
    int dataRate;
};
class data_frame
{
public:
    int framesize;
    int idcode;
    long int soc;
    long int fracsec;
    std::string stat;
    std::vector<std::pair<double, double>> phasors;
    double freq;
    double dfreq;
    std::vector<double> analogs;
    std::vector<int> digital;
};
#endif
