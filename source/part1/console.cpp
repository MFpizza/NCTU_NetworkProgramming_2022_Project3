#include "htmlFormat.h"



int main(){
    string QS = string(getenv("QUERY_STRING"));
    // cout<<QS<<endl;
    
    vector<string> parameter = SeperateQueryString(QS);

    for(size_t i=0;i<parameter.size();i+=3){
        npshell newShell;
        newShell.host = parameter[i];
        newShell.port = parameter[i+1];
        newShell.file = parameter[i+2];
        shells.push_back(newShell);
    }

    printHTML(shells);

    serverToNP myServer;
    io_context.run();
}