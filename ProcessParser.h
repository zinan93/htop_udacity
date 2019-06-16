#ifndef PROCESSPARSER_H
#define PROCESSPARSER_H

#include <algorithm>
#include <iostream>
#include <math.h>
#include <thread>
#include <chrono>
#include <iterator>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cerrno>
#include <cstring>
#include <dirent.h>
#include <time.h>
#include <unistd.h>
#include "constants.h"
#include "util.h"


using namespace std;

class ProcessParser{
private:
    std::ifstream stream;
    public:
    static string getCmd(string pid);
    static vector<string> getPidList();
    static std::string getVmSize(string pid);
    static std::string getCpuPercent(string pid);
    static long int getSysUpTime();
    static std::string getProcUpTime(string pid);
    static string getProcUser(string pid);
    static vector<string> getSysCpuPercent(string coreNumber = "");
    static float getSysRamPercent();
    static string getSysKernelVersion();
    static int getNumberOfCores();
    static int getTotalThreads();
    static int getTotalNumberOfProcesses();
    static int getNumberOfRunningProcesses();
    static string getOSName();
    static std::string PrintCpuStats(std::vector<std::string> values1, std::vector<std::string>values2);
    static bool isPidExisting(string pid);
};

// TODO: Define all of the above functions below:

//formulate path, get stream, and convert to line
string ProcessParser::getCmd(string pid){
    string path, line;
    path = Path::basePath() + pid + Path::cmdPath();
    std::ifstream stream = Util::getStream(path);
    std::getline(stream, line);

    return line;
}

//return the list of all pids in /proc/
vector<string> ProcessParser::getPidList(){
    vector<string> list;
    DIR* dir;
    string path = Path::basePath();
    dir = opendir(path.c_str());
    while(dirent* dirp = readdir(dir))
    {
        //if not a directory then start over
        if(dirp->d_type != DT_DIR)
            continue;

        //check if every char is a digit, otherwise start over
        if(!std::all_of(dirp->d_name, dirp->d_name + std::strlen(dirp->d_name),
            [](char c){ return std::isdigit(c); }))
                continue;
        //if a pid then save to list
        list.push_back(dirp->d_name);       
    }
    return list;
}

std::string ProcessParser::getVmSize(string pid){
    std::string path = Path::basePath() + pid + Path::statusPath();
    std::ifstream file(Util::getStream(path));
    std::string str;
    std::size_t found;
    std::string vmsize;
    while (std::getline(file, str))
    {   
        found=str.find("VmSize");
        if (found!=std::string::npos){
            vmsize = Util::extractIntFromString(str);
            return std::string(vmsize);
        }
        // if (found!=std::string::npos){
        //     for (int i = str.find(":"); i <= str.length(); i++){
        //         vmsize += str[i];
        //     }
        //     return vmsize;
        // }
            
    }   
    throw std::runtime_error("No VmSize found");
}

#endif