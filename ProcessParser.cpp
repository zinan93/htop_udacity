#include <iostream>
#include "ProcessParser.h"
#include <sstream>
using namespace std;

int main(){
    string pid = "1";
    string out;
    out = ProcessParser::getCmd(pid);
    cout<<out<<endl;
    
    vector<string> list;
    list = ProcessParser::getPidList();
    string vmsize = ProcessParser::getVmSize("1");
    cout<<vmsize<<endl;
}