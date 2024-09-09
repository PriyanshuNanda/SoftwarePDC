// #include <bits/stdc++.h>
#include "structures.h"
#include "cmdframe.h"
#include <fstream>
#include "utilities.h"
#include <winsock2.h>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>
#include <iomanip>
#include <sstream>
#include <unordered_map>
#include <cassert>
#include <mutex>
#include <mysql.h>
// #undef max
constexpr double MAX_DOUBLE = 0x1.fffffffffffffp+1023;

bool closeOrNot(bool &isClose,std::mutex &mtx){
        mtx.lock();
        bool close = isClose;
        mtx.unlock();
        return close;
}

std::string removeSubstring(std::string str)
{
    std::string result = str;
    std::string substr = "ffffff";
    size_t pos = result.find(substr);
    while (pos != std::string::npos)
    {
        result.erase(pos, substr.size());
        pos = result.find(substr);
    }
    return result;
}
std::string returnHex(char buffer[], int start, int end)
{
    std::stringstream ss;
    ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(buffer[start]);
    for (int i = start + 1; i <= end; i++)
    {
        ss << std::setw(2) << static_cast<int>(buffer[i]);
    }
    return ss.str();
}
std::string hexValue(char buffer[], int start, int end)
{
    return removeSubstring(returnHex(buffer, start, end));
}
std::string removeSpaces(std::string str)
{
    std::string noSpaces = str;
    noSpaces.erase(remove(noSpaces.begin(), noSpaces.end(), ' '), noSpaces.end());
    return noSpaces;
}

/*function that converts binary to decimal*/
int binaryToDecimal(std::string binaryString)
{
    int decimalValue = 0;
    int base = 1;
    int length = binaryString.length();

    for (int i = length - 1; i >= 0; i--)
    {
        if (binaryString[i] == '1')
        {
            decimalValue += base;
        }
        base = base * 2;
    }
    return decimalValue;
}

std::string removeTrailingZeros(std::string str)
{
    int len = str.length();
    while (len > 0 && str[len - 1] == '0' && len > 1 && str[len - 2] == '0')
    {
        str.pop_back();
        str.pop_back();
        len -= 2;
    }
    return str;
}

std::string hexToAscii(std::string hex)
{
    std::stringstream ss;
    for (size_t i = 0; i < hex.length(); i += 2)
    {
        unsigned char byte = static_cast<unsigned char>(std::stoul(hex.substr(i, 2), nullptr, 16));
        if (byte != '\0')
        {
            ss << byte;
        }
    }
    return ss.str();
}

std::string toHex(std::string data)
{
    char const hex_chars[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    std::string str = "";
    for (int i = 0; i < data.length(); i++)
    {
        char const byte = data[i];

        str += hex_chars[(byte & 0xF0) >> 4];
        str += hex_chars[(byte & 0x0F) >> 0];
    }
    return str;
}
int hexToDecimal(std::string str)
{
    return std::stoll(str, nullptr, 16);
}

std::string hexToBinary(std::string hex)
{
    std::unordered_map<char, std::string> hexToBinMap{
        {'0', "0000"}, {'1', "0001"}, {'2', "0010"}, {'3', "0011"}, {'4', "0100"}, {'5', "0101"}, {'6', "0110"}, {'7', "0111"}, {'8', "1000"}, {'9', "1001"}, {'A', "1010"}, {'B', "1011"}, {'C', "1100"}, {'D', "1101"}, {'E', "1110"}, {'F', "1111"}};

    std::string binary;
    for (char digit : hex)
    {
        binary += hexToBinMap[toupper(digit)];
    }
    return binary;
}

double binToFloatMag(std::string binaryStr, int flag)
{
    if (flag == 3)
    {
        unsigned long src = std::stoul(binaryStr, nullptr, 2);
        int s, e;
        long f;
        double value;

        s = (src & 0x80000000UL) >> 31;
        e = (src & 0x7F800000UL) >> 23;
        f = (src & 0x007FFFFFUL);

        if (e == 255 && f != 0)
        {

            /* NaN (Not a Number) */
            value = MAX_DOUBLE;
        }
        else if (e == 255 && f == 0 && s == 1)
        {
            /* Negative infinity */
            value = -MAX_DOUBLE;
        }
        else if (e == 255 && f == 0 && s == 0)
        {
            /* Positive infinity */
            value = MAX_DOUBLE;
        }
        else if (e > 0 && e < 255)
        {
            /* Normal number */
            f += 0x00800000UL;
            if (s)
                f = -f;
            value = std::ldexp(f, e - 150);
        }
        else if (e == 0 && f != 0)
        {
            /* Denormal number */
            if (s)
                f = -f;
            value = std::ldexp(f, -149);
        }
        else if (e == 0 && f == 0 && s == 1)
        {
            /* Negative zero */
            value = 0;
        }
        else if (e == 0 && f == 0 && s == 0)
        {
            /* Positive zero */
            value = 0;
        }
        else
        {
            /* Never happens */
            assert(false && "Unhandled case in decode_ieee_single()");
        }

        return value;
    }

    else if (flag == 1)
    {
        int sign = binaryStr[0] - '0';
        int exponent = (binaryStr[1] - '0') * 8 + (binaryStr[2] - '0') * 4 + (binaryStr[3] - '0') * 2 + (binaryStr[4] - '0');
        int fraction = (binaryStr[5] - '0') * 512 + (binaryStr[6] - '0') * 256 + (binaryStr[7] - '0') * 128 + (binaryStr[8] - '0') * 64 +
                       (binaryStr[9] - '0') * 32 + (binaryStr[10] - '0') * 16 + (binaryStr[11] - '0') * 8 + (binaryStr[12] - '0') * 4 +
                       (binaryStr[13] - '0') * 2 + (binaryStr[14] - '0');

        // Handle special cases
        if (exponent == 31 && fraction != 0)
        {
            return std::nan(""); // NaN
        }
        else if (exponent == 31)
        {
            return sign ? -std::numeric_limits<float>::infinity() : std::numeric_limits<float>::infinity(); // Inf
        }
        else if (exponent == 0 && fraction == 0)
        {
            return sign ? -0.0f : 0.0f; // Zero
        }

        // Calculate the actual value
        float value = static_cast<float>(fraction) / pow(2, 10);
        value += pow(2, exponent - 15);
        return sign ? -value : value;
    }

    return 0.0;
}
void printHex(char *arr, int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("%02x ", (unsigned char)arr[i]);
        if ((i + 1) % 16 == 0)
        {
            printf("\n");
        }
    }
    printf("\n");
}

double binToFloatAngle(std::string binary, int flag)
{
    if (flag == 3)
    {
        unsigned long src = std::stoul(binary, nullptr, 2);
        int s, e;
        long f;
        double value;

        s = (src & 0x80000000UL) >> 31;
        e = (src & 0x7F800000UL) >> 23;
        f = (src & 0x007FFFFFUL);

        double temp = MAX_DOUBLE;

        if (e == 255 && f != 0)
        {
            /* NaN (Not a Number) */
            value = temp;
        }
        else if (e == 255 && f == 0 && s == 1)
        {
            /* Negative infinity */
            value = -temp;
        }
        else if (e == 255 && f == 0 && s == 0)
        {
            /* Positive infinity */
            value = temp;
        }
        else if (e > 0 && e < 255)
        {
            /* Normal number */
            f += 0x00800000UL;
            if (s)
                f = -f;
            value = std::ldexp(f, e - 150);
        }
        else if (e == 0 && f != 0)
        {
            /* Denormal number */
            if (s)
                f = -f;
            value = std::ldexp(f, -149);
        }
        else if (e == 0 && f == 0 && s == 1)
        {
            /* Negative zero */
            value = 0;
        }
        else if (e == 0 && f == 0 && s == 0)
        {
            /* Positive zero */
            value = 0;
        }
        else
        {
            /* Never happens */
            assert(false && "Unhandled case in decode_ieee_single()");
        }

        return value * 180.0 / 3.14;
    }

    else if (flag == 1)
    {
        int sign = binary[0] - '0';
        int exponent = (binary[1] - '0') * 8 + (binary[2] - '0') * 4 + (binary[3] - '0') * 2 + (binary[4] - '0');
        int fraction = (binary[5] - '0') * 512 + (binary[6] - '0') * 256 + (binary[7] - '0') * 128 + (binary[8] - '0') * 64 +
                       (binary[9] - '0') * 32 + (binary[10] - '0') * 16 + (binary[11] - '0') * 8 + (binary[12] - '0') * 4 +
                       (binary[13] - '0') * 2 + (binary[14] - '0');

        // Handle special cases
        if (exponent == 31 && fraction != 0)
        {
            return std::nan(""); // NaN
        }
        else if (exponent == 31)
        {
            return sign ? -std::numeric_limits<float>::infinity() : std::numeric_limits<float>::infinity(); // Inf
        }
        else if (exponent == 0 && fraction == 0)
        {
            return sign ? -0.0f : 0.0f; // Zero
        }

        // Calculate the actual value
        float value = static_cast<float>(fraction) / pow(2, 10);
        value += pow(2, exponent - 15);
        // return sign ? -value : value;
        if (sign == 1)
        {
            return -value * 180.0 / 3.14;
        }
        else
        {
            return value * 180.0 / 3.14;
        }
    }

    return 0;
}

void writeCFG2File(std::string fileName, cfg2_frame *cfgFrame)
{
    std::ofstream fout;
    std::string hashLine = "#################################################################################################################################################################";
    fout.open(fileName);
    while (fout)
    {
        fout << hashLine << std::endl;
        fout << "FRAMESIZE"
             << " " << std::to_string(cfgFrame->framesize) << std::endl;
        fout << hashLine << std::endl;
        fout << "IDCODE"
             << " " << std::to_string(cfgFrame->idcode) << std::endl;
        fout << hashLine << std::endl;
        fout << "SOC"
             << " " << std::to_string(cfgFrame->soc) << std::endl;
        fout << hashLine << std::endl;
        fout << "FRACSEC"
             << " " << std::to_string(cfgFrame->fracsec) << std::endl;
        fout << hashLine << std::endl;
        fout << "TIME_BASE"
             << " " << std::to_string(cfgFrame->timeBase) << std::endl;
        fout << hashLine << std::endl;
        fout << "NUM_PMU"
             << " " << std::to_string(cfgFrame->numPmu) << std::endl;
        fout << hashLine << std::endl;
        fout << "STN"
             << " " << cfgFrame->stn << std::endl;
        fout << hashLine << std::endl;
        fout << "FORMAT"
             << " " << cfgFrame->format << std::endl;
        fout << hashLine << std::endl;
        fout << "PHNMR"
             << " " << cfgFrame->phnmr << std::endl;
        fout << hashLine << std::endl;
        fout << "ANNMR"
             << " " << cfgFrame->annmr << std::endl;
        fout << hashLine << std::endl;
        fout << "DGNMR"
             << " " << cfgFrame->dgnmr << std::endl;
        fout << hashLine << std::endl;
        for (int i = 0; i < cfgFrame->chnam.size(); i++)
        {
            fout << "CHNAM"
                 << " " << cfgFrame->chnam[i] << std::endl;
            fout << hashLine << std::endl;
        }
        for (int i = 0; i < cfgFrame->phunit.size(); i++)
        {
            fout << "PHUNIT"
                 << " " << cfgFrame->phunit[i].first << " " << cfgFrame->phunit[i].second << std::endl;
            fout << hashLine << std::endl;
        }
        for (int i = 0; i < cfgFrame->anunit.size(); i++)
        {
            fout << "ANUNIT"
                 << " " << cfgFrame->anunit[i].first << " " << cfgFrame->anunit[i].second << std::endl;
            fout << hashLine << std::endl;
        }
        fout << "FNOM"
             << " " << cfgFrame->fnom << std::endl;
        fout << hashLine << std::endl;
        fout << "DATARATE"
             << " " << cfgFrame->dataRate << std::endl;
        fout << hashLine << std::endl;

        break;
    }
}
cfg2_frame makeCFG2(SOCKET clientSocket,int pmuId)
{
    cfg2_frame cfg;
    char receive_data_buffer[2048];
    int data_bytes_read = recv(clientSocket, receive_data_buffer, sizeof(receive_data_buffer), 0);
    if (data_bytes_read == -1)
    {
        int err = WSAGetLastError();
        return cfg;
    }

    std::string hex = hexValue(receive_data_buffer, 2, 3);
    cfg.framesize = hexToDecimal(hex);
    cfg.idcode = hexToDecimal(hexValue(receive_data_buffer, 4, 5));
    cfg.soc = hexToDecimal(hexValue(receive_data_buffer, 6, 9));
    cfg.fracsec = hexToDecimal(hexValue(receive_data_buffer, 10, 13));
    cfg.timeBase = hexToDecimal(hexValue(receive_data_buffer, 14, 17));
    cfg.numPmu = hexToDecimal(hexValue(receive_data_buffer, 18, 19));
    cfg.stn = hexToAscii(hexValue(receive_data_buffer, 20, 35));
    cfg.format = hexToBinary(hexValue(receive_data_buffer, 39, 39));
    cfg.phnmr = hexToDecimal(hexValue(receive_data_buffer, 40, 41));
    cfg.annmr = hexToDecimal(hexValue(receive_data_buffer, 42, 43));
    cfg.dgnmr = hexToDecimal(hexValue(receive_data_buffer, 44, 45));
    int curr = 46;
    for (int i = 46; i < (curr + (cfg.phnmr + cfg.annmr + cfg.dgnmr * 16) * 16); i += 16)
    {
        cfg.chnam.push_back(hexToAscii(hexValue(receive_data_buffer, i, i + 15)));
    };
    curr = curr + ((cfg.phnmr + cfg.annmr + cfg.dgnmr * 16) * 16);
    int chIndex = 0;
    for (int i = curr; i < curr + cfg.phnmr * 4; i += 4)
    {
        cfg.phunit.push_back({cfg.chnam[chIndex], hexToDecimal(hexValue(receive_data_buffer, i + 1, i + 3))});
        chIndex++;
    }
    chIndex = cfg.phnmr;
    curr += cfg.phnmr * 4;
    for (int i = curr; i < curr + cfg.annmr * 4; i += 4)
    {
        cfg.anunit.push_back({cfg.chnam[chIndex], hexToDecimal(hexValue(receive_data_buffer, i + 1, i + 3))});
        chIndex++;
    }
    curr += cfg.annmr * 4 + cfg.dgnmr * 4;
    std::string fnomType = hexValue(receive_data_buffer, curr, curr + 1);
    // std::cout<<fnomType<<std::endl;
    if (fnomType.back() == '1')
        cfg.fnom = 50;
    else
        cfg.fnom = 60;
    cfg.dataRate = hexToDecimal(hexValue(receive_data_buffer, curr + 4, curr + 5));
    std::time_t now = std::time(nullptr);
    std::tm *local_now = std::localtime(&now);
    char buffer[16];
    std::strftime(buffer, sizeof(buffer), "_%Y%m%d_%H%M", local_now);
    std::string date_time_str(buffer);
    std::string fileName = "./output_cfg/CFG_"+ std::to_string(pmuId) + date_time_str + ".txt";
    writeCFG2File(fileName, &cfg);
    return cfg;
}

std::string writeDataFrameFileHeader(cfg2_frame cfg,int pmuId)
{
    // std::cout<<"header writer executed"<<std::endl;
    std::time_t now = std::time(nullptr);
    std::tm *local_now = std::localtime(&now);
    char buffer[16];
    std::strftime(buffer, sizeof(buffer), "_%Y%m%d_%H%M", local_now);
    std::string date_time_str(buffer);
    std::string fileName = "./output_data/df_" +std::to_string(pmuId)+ date_time_str + ".csv";
    std::ofstream fout;
    std::string fileName1 = "sortedData.csv";
    fout.open(fileName1, std::ios::app);
    std::string headings = "";
    headings += "SOC,";
    headings += "FRACSEC,";
    headings += "PMU ID,";
    headings += "STAT,";
    headings += "FREQ,";
    int headerIdx = 0;
    for (;headerIdx < cfg.phnmr; headerIdx++)
    {
        headings += '[' + cfg.chnam[headerIdx] + "](Mag)" + ",";
        headings+= '[' + cfg.chnam[headerIdx] + "](Angle)" + ",";
    }
    for(;headerIdx< cfg.annmr;headerIdx++){
        headings+= '[' + cfg.chnam[headerIdx] + "]" + ",";
    }
    
    headings += "\n";
    fout << headings;
    fout.close();
    return fileName;
};

void writeDataframe2File(std::string fileName, data_frame dataFrame)
{
    std::ofstream fout;
    fout.open(fileName, std::ios::app);
    std::string headingsData = "";
    headingsData += std::to_string((dataFrame.soc)) + ",";
    headingsData += std::to_string((dataFrame.fracsec)) + ",";
    headingsData += std::to_string((dataFrame.idcode)) + ",";
    
    headingsData += (dataFrame.stat) + ",";
    // headingsData += std::to_string((dataFrame.stat)) + ",";
    headingsData += std::to_string((dataFrame.freq)) + ",";
    for (int i = 0; i < dataFrame.phasors.size(); i++)
    {
        headingsData += (std::to_string((dataFrame.phasors[i].first)) + "," + std::to_string((dataFrame.phasors[i].second)) + ",");
    }
    for (int i = 0; i < dataFrame.analogs.size(); i++)
    {
        headingsData += (std::to_string((dataFrame.analogs[i])) + ",");
    }


    headingsData += "\n";
    fout << headingsData;

    fout.close();
}





void writeLiveTxt(data_frame df, cfg2_frame cfg,int pmuId)
{
    std::ofstream fout;
    std::string hashLine = "--------------------------------------------------------------------------------------------------------------------------------------------------";
    fout.open("data_" + std::to_string(pmuId) + ".txt");
    while (fout)
    {
        fout << hashLine << std::endl;
        fout << "SOC"
             << "->" << std::to_string(df.soc) << std::endl;
        fout << hashLine << std::endl;
        fout << "FRACSEC"
             << "->" << std::to_string(df.fracsec) << std::endl;
        fout << hashLine << std::endl;
        fout << "STAT"
             << "->" << df.stat << std::endl;
        fout << hashLine << std::endl;
        fout << "FREQUENCY"
             << "->" << std::to_string(df.freq) << std::endl;
        fout << hashLine << std::endl;
        fout << "PHASORS" << std::endl;
        fout << hashLine << std::endl;
        int chInd=0;
        for (int i = 0; i < cfg.phnmr; i++)
        {
            fout << cfg.chnam[chInd] << "->" << df.phasors[i].first << "∠" << df.phasors[i].second << std::endl;
            chInd++;
        }
        fout << hashLine << std::endl;
        fout << "ANALOGS" << std::endl;
        fout << hashLine << std::endl;
        for (int i = 0; i < cfg.annmr; i++)
        {
            fout << cfg.chnam[chInd] << "->" << df.analogs[i]<< std::endl;
            chInd++;
        }
        fout << hashLine << std::endl;
        break;
    }
}

void updateDfMap(data_frame df,std::unordered_map<int,data_frame>& dfMap) {
    int pmuId = df.idcode;
    dfMap[pmuId] = df;
}

data_frame makeDF(SOCKET clientSocket, cfg2_frame& cfg, std::string fileName, bool& ifCFGAsked ,int pmuId,std::mutex& mtx, std::unordered_map<int, data_frame> &dfMap)
{
    
    
    int delPh = 3, delFr = 3, delAn = 3, delImOrAngle = 3;
    /*
    delFr = (cfg.format[0] == 1) ? 3 : 1;
    delAn = (cfg.format[1] == 1) ? 3 : 1;
    delPh = (cfg.format[2] == 1) ? 3 : 1;
    delImOrAngle = (cfg.format[3] == 1) ? 3 : 1;
    */
    if (cfg.format[4] == 0)
        delFr = 1;
    if (cfg.format[5] == 0)
        delAn = 1;
    if (cfg.format[6] == 0)
        delPh = 1;
    if (cfg.format[7] == 0)
        delImOrAngle = 1;

    data_frame df;
    char receive_data_buffer[1024];
    int data_bytes_read = recv(clientSocket, receive_data_buffer, sizeof(receive_data_buffer), 0);

    df.framesize = hexToDecimal(hexValue(receive_data_buffer, 2, 3));

    df.idcode = hexToDecimal(hexValue(receive_data_buffer, 4, 5));
    df.soc = hexToDecimal(hexValue(receive_data_buffer, 6, 9));
    df.fracsec = hexToDecimal(hexValue(receive_data_buffer, 10, 13));
    df.stat = hexToBinary(hexValue(receive_data_buffer, 14, 15));
    int cur = 16;
    std::string str_mag = "";
    std::string str_ang = "";

    for (int i = 0; i < cfg.phnmr; i++)
    {
        str_mag = hexValue(receive_data_buffer, cur, cur + delPh);
        str_ang = hexValue(receive_data_buffer, cur + delPh + 1, cur + (2 * delPh) + 1);
        cur = cur + 2 * (delPh + 1);
        df.phasors.push_back({binToFloatMag(hexToBinary(str_mag), delPh), binToFloatAngle(hexToBinary(str_ang), delPh)});
    }

    str_mag = hexValue(receive_data_buffer, cur, cur + delFr);
    df.freq = binToFloatMag(hexToBinary(str_mag), delFr);
    cur = cur + delPh + 1;


    str_mag = hexValue(receive_data_buffer, cur, cur + delFr);
    // nomFreq += binToFloatMag(hexToBinary(str_mag))*40/1000;
    df.dfreq = binToFloatMag(hexToBinary(str_mag), delFr);
    cur = cur + delPh + 1;

    for (int i = 0; i < cfg.annmr; i++)
    {
        str_mag = hexValue(receive_data_buffer, cur, cur + delAn);
        // str_ang= hexValue(receive_data_buffer, cur+4, cur+7);;
        cur = cur + delAn+1;
        df.analogs.push_back(binToFloatMag(hexToBinary(str_mag), delAn));
    }
    for (int i = 0; i < cfg.dgnmr; i++)
    {
        str_mag = hexValue(receive_data_buffer, cur, cur + 1);
        cur = cur + 2;
        df.digital.push_back(hexToDecimal(str_mag));
    }
    //---------------------------------------------------------------------
    if(df.freq< (cfg.fnom-10) || df.freq > (cfg.fnom+10)){
        sendCFGCmdFrame(clientSocket, pmuId);
        cfg = makeCFG2(clientSocket,pmuId);
        ifCFGAsked = 1;
        return df;
    }
    else if(df.stat[5]=='1' && !ifCFGAsked)
    {
            sendCFGCmdFrame(clientSocket, pmuId);
            cfg = makeCFG2(clientSocket,pmuId);
            ifCFGAsked = 1;
            return df;
    }
    if(df.stat[5]=='0')ifCFGAsked = 0;

     //-----------------------------------------------------------------------
    writeLiveTxt(df, cfg, pmuId);
    mtx.lock();
    updateDfMap(df,dfMap);
    //std::cout << dfMap[1].framesize<<" " << dfMap[1].freq << std::endl;
    //std::cout << dfMap[2].framesize<<" "<<dfMap[2].freq << std::endl;
    // writeDataframe2File(fileName, &df, cfg); // changed the file from df.txt to df
    mtx.unlock();
    return df;
}
