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
    ifstream stream;
    public:
    static string getCmd(string pid);
    static vector<string> getPidList();
    static string getVmSize(string pid);
    static string getCpuPercent(string pid);
    static long int getSysUpTime();
    static string getProcUpTime(string pid);
    static string getProcUser(string pid);
    static vector<string> getSysCpuPercent(string coreNumber = "");
    static float getSysRamPercent();
    static string getSysKernelVersion();
    static int getNumberOfCores();
    static int getTotalThreads();
    static int getTotalNumberOfProcesses();
    static int getNumberOfRunningProcesses();
    static string getOSName();
    static string PrintCpuStats(vector<string> values1, vector<string>values2);
    static bool isPidExisting(string pid);
};

// TODO: Define all of the above functions below:

//formulate path, get stream, and convert to line
string ProcessParser::getCmd(string pid){
    string path, line;
    path = Path::basePath() + pid + Path::cmdPath();
    ifstream stream = Util::getStream(path);
    getline(stream, line);

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
        if(!all_of(dirp->d_name, dirp->d_name + std::strlen(dirp->d_name),
            [](char c){ return std::isdigit(c); }))
                continue;
        //if a pid then save to list
        list.push_back(dirp->d_name);       
    }
    return list;
}

//TODO: VmData vs VmSize
string ProcessParser::getVmSize(string pid){
    string path = Path::basePath() + pid + Path::statusPath();
    ifstream stream(Util::getStream(path));
    string str;
    size_t found;
    string vmsize;
   
    while (getline(stream, str))
    {   
        found=str.find("VmSize");
        if (found!=std::string::npos){
            for (int i = str.find(":")+1; i <= str.length(); i++){
                if (isdigit(char(str[i])))
                    vmsize += str[i];
            }
            vmsize = to_string(stof(vmsize)/float(1024*1024));
            break;
        }
    }   
    return vmsize;
}

string ProcessParser::getProcUpTime(string pid){
    vector<string> values;
    //TODO: ifstream cannot be copied, is there a better way to pass by reference?
    values = Util::getVecFromStream(Util::getStream((Path::basePath() + pid + Path::statPath())));

    float procuptime = stof(values[13]);
    float freq = sysconf(_SC_CLK_TCK);
    return to_string(procuptime/freq);
}

long int ProcessParser::getSysUpTime(){
    
    string line;
    vector<string> values;
    values = Util::getVecFromStream(Util::getStream(Path::basePath() + Path::upTimePath()));
    
    return stoi(values[0]);
}

string ProcessParser::getCpuPercent(string pid)
{

    float result;
    vector<string> values;
    values = Util::getVecFromStream(Util::getStream((Path::basePath()+ pid + "/" + Path::statPath())));
    
    float utime = stof(ProcessParser::getProcUpTime(pid));
    float stime = stof(values[14]);
    float cutime = stof(values[15]);
    float cstime = stof(values[16]);
    float starttime = stof(values[21]);
    float uptime = ProcessParser::getSysUpTime();
    //get system frequency (clock ticks)
    float freq = sysconf(_SC_CLK_TCK);
    float total_time = utime + stime + cutime + cstime;
    float seconds = uptime - (starttime/freq);
    result = 100.0*((total_time/freq)/seconds);
    return to_string(result);
}

#endif