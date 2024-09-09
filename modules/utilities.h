#ifndef utilities_h
#define utilities_h
#include <winsock2.h>
#include<mutex>
#include<unordered_map>
bool closeOrNot(bool &isClose,std::mutex &mtx);
std::string removeSpaces(std::string str);
std::string removeSubstring(std::string str);
int binaryToDecimal(std::string binaryString);
std::string hexToAscii(std::string Str);
std::string hexValue(char buffer[], int start, int end);
int hexToDecimal(std::string hexStr);
std::string hexToBinary(std::string hexStr);
double binToFloatMag(std::string binary,int flag);
double binToFloatAngle(std::string binary,int flag);
void writeCFG2File(std::string fileName, cfg2_frame *cfgFrame);
cfg2_frame makeCFG2(SOCKET clientSocket,int pmuId);
void writeDataframe2File(std::string fileName, data_frame dataFrame);
std::string writeDataFrameFileHeader(cfg2_frame cfg,int pmuId);
data_frame makeDF(SOCKET clientSocket, cfg2_frame& cfg, std::string fileName, bool& ifCFGAsked ,int pmuId,std:: mutex &mtx, std::unordered_map<int, data_frame> &dfMap);
#endif // //
