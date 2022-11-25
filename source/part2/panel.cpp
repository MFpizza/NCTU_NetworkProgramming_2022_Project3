#include "header/panel.h"

string panelHTML()
{
    string test_cases[5] = {"t1.txt", "t2.txt", "t3.txt", "t4.txt", "t5.txt"};
    string FORM_METHOD = "GET";
    string FORM_ACTION = "console.cgi";
    string test_case_menu[5] = {"", "", "", "", ""};
    for (int i = 0; i < 5; i++)
    {
        test_case_menu[i] = "<option value=\"" + test_cases[i] + "\">" + test_cases[i] + "</option>";
    }

    string domain = "cs.nctu.edu.tw";
    string hosts[12];
    for (int i = 0; i < 12; i++)
    {
        hosts[i] = "<option value=\"nplinux" + to_string(i + 1) + "." + domain + "\">nplinux" + to_string(i + 1) + "</option>";
    }
    string returnBody ="Content-type: text/html\r\n\r\n";
   

    returnBody+="<!DOCTYPE html>\
<html lang=\"en\">\
  <head>\
    <title>NP Project 3 Panel</title>\
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
      href=\"https://cdn4.iconfinder.com/data/icons/iconsimple-setting-time/512/dashboard-512.png\"\
    />\
    <style>\
      * {\
        font-family: 'Source Code Pro', monospace;\
      }\
    </style>\
  </head>\
  <body class=\"bg-secondary pt-5\">";
    returnBody+="<form action=\"console.cgi\" method=\"GET\">\
      <table class=\"table mx-auto bg-light\" style=\"width: inherit\">\
        <thead class=\"thead-dark\">\
          <tr>\
            <th scope=\"col\">#</th>\
            <th scope=\"col\">Host</th>\
            <th scope=\"col\">Port</th>\
            <th scope=\"col\">Input File</th>\
          </tr>\
        </thead>\
        <tbody>";
    for (int i = 0; i < 5; i++)
    {
        returnBody+="<tr>\
            <th scope=\"row\" class=\"align-middle\">Session " +
                    to_string(i + 1) + "</th>\
            <td>\
              <div class=\"input-group\">\
                <select name=\"h" +
                    to_string(i) + "\" class=\"custom-select\">\
                  <option></option>";
        for (int j = 0; j < 12; j++){
            returnBody+=hosts[j];
        }
        returnBody+="</select>\
                <div class=\"input-group-append\">\
                  <span class=\"input-group-text\">.cs.nctu.edu.tw</span>\
                </div>\
              </div>\
            </td>\
            <td>\
              <input name=\"p" +
                    to_string(i) + "\" type=\"text\" class=\"form-control\" size=\"5\" />\
            </td>\
            <td>\
              <select name=\"f" +
                    to_string(i) + "\" class=\"custom-select\">\
                <option></option>";
        for(int j = 0; j <5;j++)
            returnBody+=test_case_menu[j];
        returnBody+="</select>\
            </td>\
          </tr>";
    }
    returnBody+="<tr>\
            <td colspan=\"3\"></td>\
            <td>\
              <button type=\"submit\" class=\"btn btn-info btn-block\">Run</button>\
            </td>\
          </tr>\
        </tbody>\
      </table>\
    </form>\
  </body>\
</html>";
    return returnBody;
}