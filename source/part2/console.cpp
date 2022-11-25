#include "header/console.h"

vector<npshell> shells;

string printHTML()
{
    string s = "";
    s += print_header();
    for (size_t i = 0; i < shells.size(); i += 2)
    {
        s += "<thead align=\"center\">\
        <tr>";
        string th = "<th scope=\"col\">" + shells[i].host + ":" + shells[i].port + "</th>";
        s += th;
        if (i + 1 < shells.size())
        {
            string th = "<th scope=\"col\">" + shells[i + 1].host + ":" + shells[i + 1].port + "</th>";
            s += th;
        }
        s += "</tr>\
      </thead>\
      <tbody>\
        <tr>";
        string td = "<td><pre id=\"s" + to_string(i) + "\" class=\"mb-0\"></pre></td>";
        s += td;
        if (i + 1 < shells.size())
        {
            string td = "<td><pre id=\"s" + to_string(i + 1) + "\" class=\"mb-0\"></pre></td>";
            s += td;
        }
        s += "</tr>\
      </tbody>";
    }
    s += print_endHeader();
    return s;
}

string print_header()
{
    string s = "";
    s += "Content-type: text/html\r\n\r\n";
    s += "<!DOCTYPE html>\
<html lang=\"en\">\
  <head>\
    <meta charset=\"UTF-8\" />\
    <title>NP Project 3 Console</title>\
    <link\
      rel=\"stylesheet\"\
      href=\"https://cdn.jsdelivr.net/npm/bootstrap@4.5.3/dist/css/bootstrap.min.css\"\
      integrity=\"sha384-TX8t27EcRE3e/ihU7zmQxVncDAy5uIKz4rEkgIXeMed4M0jlfIDPvg6uqKI2xXr2\"\
      crossorigin=\"anonymous\"\
    />\
    <link\
      href=\"https://fonts.googleapis.com/css?family=Source+Code+Pro\"\
      rel=\"stylesheet\"\
    />\
    <link\
      rel=\"icon\"\
      type=\"image/png\"\
      href=\"https://cdn0.iconfinder.com/data/icons/small-n-flat/24/678068-terminal-512.png\"\
    />\
    <style>\
      * {\
        font-family: 'Source Code Pro', monospace;\
        font-size: 1rem !important;\
      }\
      body {\
        background-color: #212529;\
      }\
      pre {\
        color: #cccccc;\
      }\
      b {\
        color: #01b468;\
      }\
    </style>\
  </head>\
  <body>\
    <table class=\"table table-dark table-bordered\">";
    return s;
}

string print_endHeader()
{
    return "</table>\
  </body>\
</html>";
}

string output_shell(int session, string content)
{
    boost::replace_all(content, "\n\r", " ");
    boost::replace_all(content, "\n", "&NewLine;");
    boost::replace_all(content, "\r", " ");
    boost::replace_all(content, "'", "\\'");
    boost::replace_all(content, "<", "&lt;");
    boost::replace_all(content, ">", "&gt;");
    string s = "<script>document.getElementById('s"+to_string(session)+"').innerHTML += '"+content+"';</script>";
    return s;
}

string output_command(int session, string content)
{
    boost::replace_all(content, "\n\r", " ");
    boost::replace_all(content, "\n", "&NewLine;");
    boost::replace_all(content, "\r", " ");
    boost::replace_all(content, "'", "\\'");
    boost::replace_all(content, "<", "&lt;");
    boost::replace_all(content, ">", "&gt;");
    string s = "<script>document.getElementById('s"+to_string(session)+"').innerHTML += '<b>"+content+"</b>';</script>";
    return s;
}

vector<string> SeperateQueryString(string QS)
{
    vector<string> parameter;
    string delimiter = "=";
    string delimiter2 = "&";
    size_t pos = 0, pos2 = 0;
    string token;
    while ((pos = QS.find(delimiter)) != string::npos)
    {
        QS.erase(0, pos + delimiter.length());
        pos2 = QS.find(delimiter2);
        token = QS.substr(0, pos2);
        if (token != "")
        {
            // cout<<token<<endl;
            parameter.push_back(token);
        }
        QS.erase(0, pos2 + delimiter2.length());
    }
    return parameter;
}

void buildShells(string QS){
    shells.clear();
    vector<string> parameter = SeperateQueryString(QS);

    for(size_t i=0;i<parameter.size();i+=3){
        npshell newShell;
        newShell.host = parameter[i];
        newShell.port = parameter[i+1];
        newShell.file = parameter[i+2];
        shells.push_back(newShell);
    }
    // cout<<"shells size: "<<shells.size()<<endl;
}