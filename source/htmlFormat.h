#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sys/wait.h>
#include <unistd.h>
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
void print_header();
void print_endHeader();
void output_shell(int session,string content);
void output_command(int session,string content);

boost::asio::io_context io_context;
vector<npshell> shells;

class session
: public std::enable_shared_from_this<session>
{
	public:
		session(tcp::socket socket_,int index)
			: socket_(std::move(socket_))
		{
            id = index;
            filename = "./test_case/"+shells[id].file;
            inputFile.open(filename,ios::in);
        }

        void start(){
            do_read();
        }

	private:
		void do_read()
		{
			auto self(shared_from_this());
            Receive="";
            socket_.async_read_some(boost::asio::buffer(data_, max_length),
                [this,self](boost::system::error_code ec, std::size_t length){
                if (!ec){
                    Receive += data_;
                    memset(data_,0,length);
                    size_t pos;
                    if((pos = Receive.find("%")) != string::npos){
                        output_shell(id,Receive);
                        do_write();
                    }
                    do_read();
                }
                else
                    perror("async_read");
                });
		}

		void do_write()
		{
			auto self(shared_from_this());
            string input;
            //if(file_input.eof()) return;
            if(!getline(inputFile,input)){
                cerr << "getline fail" << endl;
            }
            input = input + "\n";
            output_command(id,input);
            boost::asio::async_write(socket_, boost::asio::buffer(input.c_str(), input.length()),
                [this,self](boost::system::error_code ec, std::size_t /*length*/){
                if (!ec){
                }else{
                    perror("async_write");
                }
            });
        }

        tcp::socket socket_;
        enum { max_length = 1024 };
        char data_[max_length];
        ifstream inputFile;
        int id;
        string filename;
        string Receive;
};

class server{
    public:
        server():resolve(io_context){
            for(size_t i=0;i<shells.size();i++){
                tcp::resolver::query query(shells[i].host, shells[i].port);
                resolve.async_resolve(query,
                    boost::bind(&server::connection, this,i,boost::asio::placeholders::error,boost::asio::placeholders::iterator ));
            }
        }
        void connection(const int i,const boost::system::error_code& err,const tcp::resolver::iterator it){
            if (!err)
            {
                socket_[i] = new tcp::socket(io_context);
                (*socket_[i]).async_connect(*it,
                boost::bind(&server::create_session, this,i,boost::asio::placeholders::error,it ));
            }
        }
        void create_session(const int i,const boost::system::error_code& err,const tcp::resolver::iterator it){
            if (!err)
            {
                std::make_shared<session>(std::move(*socket_[i]),i)->start();
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