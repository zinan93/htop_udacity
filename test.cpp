    #include <iostream>
    #include <stdio.h>
    #include <stdlib.h>

    using namespace std;

    string GetStdoutFromCommand(string cmd) {
    cout <<"cmd is: "<<cmd<<endl;

    string data;
    FILE * stream;
    const int max_buffer = 256;
    char buffer[max_buffer];
    // cmd.append(" 2>&1");

    stream = popen(cmd.c_str(), "r");
    if (stream) {
    while (!feof(stream))
    if (fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);
    pclose(stream);
    }
    return data;
    }

    int main (){

    string ls = GetStdoutFromCommand("cat /proc/13931/cmdline");


    cout << "LS: " << ls << endl;

    return 0;
    }
