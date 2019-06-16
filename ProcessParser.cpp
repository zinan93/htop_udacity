#include <iostream>
#include "ProcessParser.h"
#include <sstream>
using namespace std;

int main(){
    string pid = "1";
    string out;
    out = ProcessParser::getCmd(pid);
    cout<<"getcmd: "<<out<<endl;
    
    vector<string> list;
    list = ProcessParser::getPidList();
    cout<<"pid list: ";
    for (auto &item: list) {
        cout<<item<<" ";
    }
    cout<<endl;


    string vmsize = ProcessParser::getVmSize("1");
    cout<<"vmsize: "<<vmsize<<endl;

    string procuptime = ProcessParser::getProcUpTime("1");
    cout<<"procuptime: "<<procuptime<<endl;
    string cpupercent = ProcessParser::getCpuPercent("1");
    cout<<"cpu percent: "<<cpupercent<<endl;
}