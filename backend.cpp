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

// std::mutex mtx;
// std::vector<std::pair<SOCKET, int>> sockets;
// std::unordered_map<int, cfg2_frame> cfgMap;

bool isNotAlphaNumOrSpace(char c) {
    return !(std::isalnum(c));
}
//|| std::isspace(c)
std::string deleteSpecialChar(std::string str)
{
    std::string columnName = str;
    std::replace(columnName.begin(), columnName.end(), ' ', '_');
    std::replace_if(columnName.begin(), columnName.end(), isNotAlphaNumOrSpace, '_');

    // Replace spaces with underscores


    return columnName;
}

/*
bool isNotAlphaNumOrSpace(char c) {
    return !(std::isalnum(c) || std::isspace(c));
}

std::string deleteSpecialChar(std::string str) {
    std::string columnName = str;
    std::replace_if(columnName.begin(), columnName.end(), isNotAlphaNumOrSpace, '_');
    return columnName;
}
*/

std::string tableNameFunc() {
    std::time_t now = std::time(nullptr);
    std::tm* local_now = std::localtime(&now);
    char buffer[16];
    std::strftime(buffer, sizeof(buffer), "%Y%m%d_%H%M", local_now);
    std::string date_time_str(buffer);
    std::string tableName = date_time_str + "_data";
    return tableName;
}
void connectPMU(std::string ip, int port, int pmuId, bool& isClose, std::vector<data_frame>& dfBuffer, std::mutex& mtx, std::unordered_map<int, data_frame>& dfMap, std::vector<std::pair<SOCKET, int>>& sockets, std::unordered_map<int, cfg2_frame>& cfgMap)
{
    SOCKET clientSocket;
    clientSocket = connection(ip, port);
    mtx.lock();
    sockets.push_back({ clientSocket, pmuId });
    mtx.unlock();
    if (clientSocket != 1)
    {
        startDataStream(clientSocket, pmuId, isClose, dfBuffer, cfgMap, mtx, dfMap);
    }
}


/*
static bool customSort(data_frame df1, data_frame df2)
{

    double time1 = (df1.soc) + (df1.fracsec / 1000000.00);
    double time2 = (df2.soc) + (df2.fracsec / 1000000.00);
    // double time1 = (df1.soc) + (df1.fracsec / ((double)(cfgMap[df1.idcode]).timeBase));
    // double time2 = (df2.soc) + (df2.fracsec / ((double)(cfgMap[df2.idcode]).timeBase));

    return time1 < time2;

}
*/

void writeDataframe2Db(MYSQL* dbConnection, data_frame dataFrame, std::string tableName, std::unordered_map<int, cfg2_frame>& cfgMap) {
    // Assuming the table structure matches the data_frame structure
    // and the table has columns for framesize, idcode, soc, fracsec, stat, freq, dfreq, and separate columns for phasors and analogs.

    // Construct the SQL query
    std::ostringstream sql;
    sql << "INSERT INTO " << tableName << " (framesize, idcode, soc, fracsec, freq, dfreq, ";

    // Assuming phasors and analogs are stored as separate rows or columns in the database
    // Adjust the query according to your actual database schema
    int headerIdx = 0;
    //mtx.lock();
    for (; headerIdx < cfgMap[dataFrame.idcode].phnmr; headerIdx++)
    {
        // Ensure there's a space between the column name and its data type

        sql << deleteSpecialChar(cfgMap[dataFrame.idcode].chnam[headerIdx]) << "_Mag,";
        sql << deleteSpecialChar(cfgMap[dataFrame.idcode].chnam[headerIdx]) << "_Angle,";
    }
    for (; headerIdx < cfgMap[dataFrame.idcode].annmr + cfgMap[dataFrame.idcode].phnmr; headerIdx++) {
        // Ensure there's a space between the column name and its data type
        sql << "`" << cfgMap[dataFrame.idcode].chnam[headerIdx] << "`" << ",";
    }
    //mtx.unlock();
    std::string query1 = sql.str();
    query1 = query1.substr(0, query1.length() - 1);
    // Complete the CREATE TABLE statement
    //std::ostringstream sql;

    sql.str("");
    sql.clear();
    sql << ") VALUES (";

    // Bind values
    sql << dataFrame.framesize << ",";
    sql << dataFrame.idcode << ",";
    sql << dataFrame.soc << ",";
    sql << dataFrame.fracsec << ",";
    //sql << "'" << dataFrame.stat << "',"; // Assuming stat is a string
    sql << dataFrame.freq << ",";
    sql << dataFrame.dfreq << ",";

    for (int i = 0; i < dataFrame.phasors.size(); i++) {
        sql << dataFrame.phasors[i].first << ",";
        sql << dataFrame.phasors[i].second << ",";
    }
    for (int i = 0; i < dataFrame.analogs.size(); i++) {
        sql << dataFrame.analogs[i] << ",";
    }

    /*
    for (int i = 0; i < dataFrame.digital.size(); i++) {
        sql << dataFrame.digital[i] << ",";
    }*/
    std::string query2 = sql.str();
    query2 = query2.substr(0, query2.length() - 1);
    std::string query = query1 + query2 + ")";

    //std::cout << query << std::endl;
    // Execute the query
    if (mysql_query(dbConnection, query.c_str())) {
        std::cerr << "Failed to insert data: " << mysql_error(dbConnection) << std::endl;
    }

    else {
        //std::cout << "Data inserted successfully." << std::endl;
    }
}

void sortQVector(std::queue<std::vector<data_frame>>& taskQueue, bool& isClose, MYSQL* dbConnection, std::string tableName, bool csvOrDb, std::mutex& mtx, std::unordered_map<int, cfg2_frame>& cfgMap)
{
    std::vector<data_frame> curVector;
    std::string fileName = "sortedData.csv";
    double lastPush = 0;
    while (!closeOrNot(isClose, mtx))
    {
        mtx.lock();
        if (!taskQueue.empty())
        {
            curVector = taskQueue.front();
            std::sort(curVector.begin(), curVector.end(), [&cfgMap](const data_frame& df1, const data_frame& df2) {
                double time1 = (df1.soc) + (df1.fracsec / ((double)(cfgMap[df1.idcode]).timeBase));
                double time2 = (df2.soc) + (df2.fracsec / ((double)(cfgMap[df2.idcode]).timeBase));
                return time1 < time2;
                });
            //sort(curVector.begin(), curVector.end(), customSort);
            // mtx.lock();
            //double timeBase = (double)cfgMap[curVector[0].idcode].timeBase;
            // std::cout<<timeBase<<std::endl;
            // mtx.unlock();
            double timeBase;

            if (curVector.size() == 0) { timeBase = 1000000.00; }
            else {
                //std::cout << cfgMap[curVector[0].idcode].timeBase << std::endl;
                timeBase = cfgMap[curVector[0].idcode].timeBase;
            }
            while ((curVector.size() > 0) && (curVector[0].fracsec / timeBase + curVector[0].soc) < lastPush)
            {
                curVector.erase(curVector.begin());
            }
            for (auto df : curVector)
            {
                if (csvOrDb == 0)
                    writeDataframe2File(fileName, df);
                else
                    writeDataframe2Db(dbConnection, df, tableName, cfgMap);

            }
            if (curVector.size() > 0)
            {
                lastPush = curVector.back().fracsec / timeBase + curVector.back().soc;
            }
            // std::cout<<lastPush<<std::endl;
            taskQueue.pop();
        }
        mtx.unlock();
    }
}
void runTimer(std::queue<std::vector<data_frame>>& taskQueue, std::vector<data_frame>& dfBuffer, bool& isClose, std::mutex& mtx, std::unordered_map<int, cfg2_frame>& cfgMap)
// {
//     while (isClose != 1)
//     {
//         auto start = std::chrono::steady_clock::now();
//         while (std::chrono::steady_clock::now() - start < std::chrono::milliseconds(110))
//         {
//             // Do nothing, just wait for  110ms to pass
//         }
//         taskQueue.push(dfBuffer);
//         dfBuffer.clear();
//         // std::cout << "110ms elapsed\n";
//     }
// }

{
    int lastPackageArrivalTime = 0;
    int firstPackageArrivalTime = 1000;
    mtx.lock();
    for (auto i : cfgMap)
    {
        // std::cout<<"in the run timer and can see i as " <<i.first<< "and " <<i.second.dataRate<<std::endl;
        int packageTime = 1000.0 / (i.second.dataRate);
        // std::cout<<packageTime<<std::endl;
        if (packageTime > lastPackageArrivalTime)
        {
            lastPackageArrivalTime = packageTime;
        }
        if (packageTime < firstPackageArrivalTime)
        {
            firstPackageArrivalTime = packageTime;
        }
    }
    mtx.unlock();
    int product = lastPackageArrivalTime / firstPackageArrivalTime + (lastPackageArrivalTime % firstPackageArrivalTime != 0);
    int waitTime = (product * firstPackageArrivalTime - lastPackageArrivalTime) / 2;
    if (lastPackageArrivalTime == firstPackageArrivalTime)
        waitTime = 10;
    //std::cout << std::endl<< "wait time is -> " << waitTime << std::endl;

    while (!closeOrNot(isClose, mtx))
    {
        auto start = std::chrono::steady_clock::now();
        bool firstTime = true;
        if (firstTime)
        {

            while (std::chrono::steady_clock::now() - start < std::chrono::milliseconds(lastPackageArrivalTime + waitTime))
            {
            }
            mtx.lock();
            taskQueue.push(dfBuffer);
            dfBuffer.clear();
            mtx.unlock();
            firstTime = false;
        }
        else
        {
            while (std::chrono::steady_clock::now() - start < std::chrono::milliseconds(lastPackageArrivalTime))
            {
            }
            mtx.lock();
            taskQueue.push(dfBuffer);
            dfBuffer.clear();
            mtx.unlock();
        }

    }
}


MYSQL* DbSetup(const char* DbAddress, const char* UserName, const char* Password, int port)
{
    MYSQL* db, * conn;
    MYSQL_RES* res_set;
    MYSQL_ROW row;

    db = mysql_init(NULL);
    if (!db) return NULL; // Return NULL if db initialization fails

    conn = mysql_real_connect(db, DbAddress, UserName, Password, NULL, port, NULL, 0);
    if (!conn) {
        fprintf(stderr, "Failed to connect to database: Error: %s\n", mysql_error(db));
        mysql_close(conn);
        return NULL; // Return NULL if connection fails
    }

    // SQL query to create a database
    std::string sql = "CREATE DATABASE IF NOT EXISTS RPDC";
    if (mysql_query(conn, sql.c_str())) {
        fprintf(stderr, "Failed to create database: Error: %s\n", mysql_error(conn));
        mysql_close(conn);
        return NULL; // Return NULL if database creation fails
    }

    // Use the database
    sql = "USE RPDC";
    if (mysql_query(conn, sql.c_str())) {
        fprintf(stderr, "Failed to use database: Error: %s\n", mysql_error(conn));
        mysql_close(conn);
        return NULL; // Return NULL if database usage fails
    }
    // Return the connection pointer
    //std::cout << "database setup and connection complete" << std::endl;
    return conn;
}


std::string tableSetup(MYSQL* dbConnection, std::set<std::string> phasorHeadings, std::set<std::string> analogHeadings) {
    std::string tableName = tableNameFunc(); // Assuming this function returns the table name
    std::ostringstream sql;

    // Start the CREATE TABLE statement
    sql << "CREATE TABLE IF NOT EXISTS " << tableName << " ("
        << "framesize INT, "
        << "idcode INT, "
        << "soc BIGINT, "
        << "fracsec BIGINT, "
        //<< "stat VARCHAR(255), "
        << "freq DOUBLE, "
        << "dfreq DOUBLE, ";

    // Dynamically add columns for phasors
    // Assuming a maximum of 10 phasors for demonstration purposes


    for (auto phasor_elem : phasorHeadings)
    {


        sql << deleteSpecialChar(phasor_elem) << "_Mag DOUBLE,";
        sql << deleteSpecialChar(phasor_elem) << "_Angle DOUBLE,";

    }
    for (auto analog_elem : analogHeadings)
    {


        sql << "`" << analog_elem << "`" << " DOUBLE,";

    }
    // Remove the trailing comma and space
    std::string query = sql.str();
    query = query.substr(0, query.length() - 1);

    // Complete the CREATE TABLE statement
    query += ")";

    // Execute the query
    if (mysql_query(dbConnection, query.c_str())) {
        fprintf(stderr, "Failed to create table: Error: %s\n", mysql_error(dbConnection));
        mysql_close(dbConnection);
        return "NULL"; // Return NULL if table creation fails
    }

    //std::cout << "Table created successfully." << std::endl;
    return tableName;
}

std::unordered_map<int, data_frame> dfMap;


void backend(bool& isClose, int numThreads, std::vector<PMU> pmuVector, bool csvOrDb, dbCred dbCred, std::unordered_map<int, data_frame>dfMap, std::vector<std::pair<SOCKET, int>>& sockets, std::unordered_map<int, cfg2_frame>& cfgMap)
{
    std::mutex mtx;

    MYSQL* dbConnection;
    if (csvOrDb == 1)
        dbConnection = DbSetup(dbCred.serverAddress.c_str(), dbCred.userName.c_str(), dbCred.password.c_str(), dbCred.port);

    std::vector<data_frame> dfBuffer;
    std::queue<std::vector<data_frame>> taskQueue;



    std::vector<std::thread> threads;

    for (int i = 0; i < numThreads; i++)
    {
        PMU pmu = pmuVector[i];
        threads.emplace_back(connectPMU, pmu.pmuIp, pmu.port, pmu.pmuId, std::ref(isClose), std::ref(dfBuffer), std::ref(mtx), std::ref(dfMap), std::ref(sockets), std::ref(cfgMap));
    }

    bool cfgMade = false;
    while (!cfgMade)
    {
        mtx.lock();
        if (cfgMap.size() >= numThreads)
            cfgMade = true;
        mtx.unlock();
    }

    std::string tableName;
    if (csvOrDb == 1)
    {
        std::set<std::string> headings_phasors;
        std::set<std::string> headings_analogs;
        mtx.lock();
        for (auto cur_cfg : cfgMap) {
            int chnam_idx = 0;
            for (; chnam_idx < cur_cfg.second.phnmr; chnam_idx++)
            {
                headings_phasors.insert(cur_cfg.second.chnam[chnam_idx]);
            }
            for (; chnam_idx < cur_cfg.second.phnmr + cur_cfg.second.annmr; chnam_idx++)
            {
                headings_analogs.insert(cur_cfg.second.chnam[chnam_idx]);
            }
        }
        mtx.unlock();
        tableName = tableSetup(dbConnection, headings_phasors, headings_analogs);
    }


    std::thread timerThread(runTimer, std::ref(taskQueue), std::ref(dfBuffer), std::ref(isClose), std::ref(mtx), std::ref(cfgMap));

    std::thread sortingThread(sortQVector, std::ref(taskQueue), std::ref(isClose), dbConnection, tableName, csvOrDb, std::ref(mtx), std::ref(cfgMap));

    closeDataStream(sockets, isClose, mtx);
    for (auto& thread : threads) {
        thread.join();
    }
    timerThread.join();
    sortingThread.join();
}
/*
int main() {
    bool isClose = false;
    PMU pmu1("172.26.37.165", 4712, 1);
    PMU pmu2("172.26.37.162", 4727, 2);
    std::vector<PMU> pmuVector = { pmu1,pmu2 };
    dbCred db("localhost", "root", "AAPR@203", 3305);
    std::unordered_map<int, data_frame> dfMap;
    backend(isClose, 2, pmuVector, 1, db,dfMap);
    return 0;
}
*/
