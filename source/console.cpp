#include "htmlFormat.h"

vector<string> SeperateQueryString(string QS);

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

    server myServer;
    io_context.run();
}

vector<string> SeperateQueryString(string QS){
    vector<string> parameter;
    string delimiter = "=";
    string delimiter2 = "&";
    size_t pos = 0,pos2=0;
    string token;
    while ((pos = QS.find(delimiter)) != string::npos)
    {
        QS.erase(0, pos + delimiter.length());
        pos2 = QS.find(delimiter2);
        token = QS.substr(0,pos2);
        if(token!=""){
            // cout<<token<<endl;
            parameter.push_back(token);
        }
        QS.erase(0, pos2 + delimiter2.length());
    }
    return parameter;
}

