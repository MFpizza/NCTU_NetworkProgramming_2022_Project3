#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/bind/bind.hpp>
using boost::asio::ip::tcp;
using boost::asio::io_service;
using namespace std;

struct npshell{
    string host;
    string port;
    string file;
};

vector<npshell> shells;
boost::asio::io_context io_context;
void print_header();
void print_endHeader();
void output_shell(int session,string content);
void output_command(int session,string content);
vector<string> SeperateQueryString(string QS);

class sessionToNP
: public std::enable_shared_from_this<sessionToNP>
{
	  public:
        sessionToNP(tcp::socket socket_,int index)
          : socket_(std::move(socket_))
        {
            cerr<<"shell "<<index<<" is creating session"<<endl;
            id = index;
            Receive="";
            filename = "./test_case/"+shells[id].file;
            inputFile.open(filename,ios::in);
        }

        void start(){
            do_read();
        }

	private:
        void do_read()
        {
            cerr<<"shell "<<id<<" start do read"<<endl;
			      auto self(shared_from_this());
            socket_.async_read_some(boost::asio::buffer(data_, max_length),
                [this,self](boost::system::error_code ec, std::size_t length){
                if (!ec){
                    Receive += data_;
                    memset(data_,0,length);
                    size_t pos;
                    if((pos = Receive.find("% ")) != string::npos){
                        output_shell(id,Receive);
                        Receive="";
                        do_write();
                    }
                    cerr<<"shell "<<id<<" finish async_read_some"<<endl;
                    do_read();
                }
                else
                    // perror("async_read");
                    socket_.close();
                });
            
            cerr<<"shell "<<id<<" finish do read"<<endl;
		    }

        void do_write()
        {
            cerr<<"shell "<<id<<" start do write"<<endl;
            auto self(shared_from_this());
            string input;
            //if(file_input.eof()) return;
            if(!getline(inputFile,input)){
                cerr << "getline fail" << endl;
            }
            input = input + "\n";
            output_command(id,input);
            cerr<<"shell "<<id<<" start async_write"<<endl;
                
            boost::asio::async_write(socket_, boost::asio::buffer(input.c_str(), input.length()),
                [this,self](boost::system::error_code ec, std::size_t /*length*/){
                if (!ec){
                    
                cerr<<"shell "<<id<<" finish async_write"<<endl;
                }else{
                    perror("async_write");
                }
            });
            cerr<<"shell "<<id<<" finish do write"<<endl;
        }

        tcp::socket socket_;
        enum { max_length = 1024 };
        char data_[max_length];
        ifstream inputFile;
        int id;
        string filename;
        string Receive;
};

class serverToNP{
    public:
        serverToNP():resolve(io_context){
            for(size_t i=0;i<shells.size();i++){
                cerr<<"create query"<<endl;
                tcp::resolver::query query(shells[i].host, shells[i].port);
                resolve.async_resolve(query,
                    boost::bind(&serverToNP::connection, this,i,boost::asio::placeholders::error,boost::asio::placeholders::iterator ));
            }
        }
        void connection(const int i,const boost::system::error_code& err,const tcp::resolver::iterator it){
            if (!err)
            {
                cerr<<"Shell "<<i<<" is prepared to connect"<<endl;
                socket_[i] = new tcp::socket(io_context);
                (*socket_[i]).async_connect(*it,
                boost::bind(&serverToNP::create_session, this,i,boost::asio::placeholders::error,it ));
            }
        }
        void create_session(const int i,const boost::system::error_code& err,const tcp::resolver::iterator it){
            if (!err)
            {
                cerr<<"Shell "<<i<<" is connected to "<<shells[i].host<<":"<<shells[i].port<<endl;
                std::make_shared<sessionToNP>(std::move(*socket_[i]),i)->start();
            }
        }
    
    private:
        tcp::socket *socket_[5];
        tcp::resolver resolve;
};



void printHTML(vector<npshell> shells){
    print_header();
    for(size_t i=0;i<shells.size();i+=2){
        cout<<"<thead align=\"center\">\
        <tr>";
        string th= "<th scope=\"col\">"+shells[i].host+":"+shells[i].port+"</th>";
        cout<<th;
        if(i+1<shells.size()){
            string th= "<th scope=\"col\">"+shells[i+1].host+":"+shells[i+1].port+"</th>";
            cout<<th;
        }
        cout<<"</tr>\
      </thead>\
      <tbody>\
        <tr>";
        string td="<td><pre id=\"s"+to_string(i)+"\" class=\"mb-0\"></pre></td>";
        cout<<td;
        if(i+1<shells.size()){
            string td="<td><pre id=\"s"+to_string(i+1)+"\" class=\"mb-0\"></pre></td>";
            cout<<td;
        }
        cout<<"</tr>\
      </tbody>";
    }
    print_endHeader();
}

void print_header(){
  cout << "Content-type: text/html\r\n\r\n";
  cout << "<!DOCTYPE html>\
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
}

void print_endHeader(){
    cout<<"</table>\
  </body>\
</html>";
}

void output_shell(int session,string content){
    boost::replace_all(content, "\n\r", " ");
    boost::replace_all(content, "\n", "&NewLine;");
    boost::replace_all(content, "\r", " ");
    boost::replace_all(content, "'", "\\'");
    boost::replace_all(content, "<", "&lt;");
    boost::replace_all(content, ">", "&gt;");
    printf("<script>document.getElementById('s%d').innerHTML += '%s';</script>",session,content.c_str());
    fflush(stdout);
}

void output_command(int session,string content){
    boost::replace_all(content, "\n\r", " ");
    boost::replace_all(content, "\n", "&NewLine;");
    boost::replace_all(content, "\r", " ");
    boost::replace_all(content, "'", "\\'");
    boost::replace_all(content, "<", "&lt;");
    boost::replace_all(content, ">", "&gt;");
    printf("<script>document.getElementById('s%d').innerHTML += '<b>%s</b>';</script>",session,content.c_str());
    fflush(stdout);
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