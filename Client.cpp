#include "client.h"
using namespace std;
Message::Message()
{
    sender = "";
    receiver = "";
    subject = "";
    body = "";
    ipAddress = "";
}

void Message::requestType(int inputMode)
{
    if (inputMode == 1)
        sendMessage();
    else
        fetchMessage();
}

void Message::sendMessage()
{
    string input = "";
    
    cout << "Enter the receipent's name:" << endl;
    getline(cin,receiver);
    
    cout << "Please enter your name:" << endl;
    getline(cin,sender);
    
    cout << "Enter the subject of your message:" << endl;
    getline(cin,subject);
    
    cout << "Finally, enter the content of your message. When you're done, enter an empty line:" << endl;
    do{
        getline(cin, input);
        if (input.empty())
            break;
        body += input;
        input = "";
    } while (!input.empty());
    formSendFile();
}

void Message::fetchMessage()
{
    cout << "Please enter your name:" << endl;
    getline(cin,receiver);
    formFetchFile();
}

Message::~Message()
{
    sender = "";
    receiver = "";
    subject = "";
    body = "";
    ipAddress = "";
}

string Message::sendToApache()
{
    string command = "/usr/bin/curl -s --data-binary \"@";
    command += textFileName;
    command += "\" -H \"Content-Type: application/x-www-form-urlencoded\" -X POST http://";
    command += ipAddress;
    command += "/echo.php";
    FILE *pf = popen(command.c_str(),"r");
    
    if(pf){
        char data[10240];
        fread(data,1, 10240 , pf);
        pclose(pf);
        command = data;
    } else command = "error";
    return command;
    //curl -d "@/Users/kmsobh/work/AUC/AUC_Adjunct/Courses/Spring_2018/CSCE-110/code/files/test.txt" -H "Content-Type: application/x-www-form-urlencoded" -X POST http://192.168.56.102/echo.php
    
}

void Message::formSendFile()
{
    messageContent.open(textFileName,ios::out);
    
    messageContent << "s" << '\n';
    messageContent << "To:" << receiver << '\n';
    messageContent << "From:" << sender << '\n';
    messageContent << "Subject:" << subject << '\n';
    messageContent << "Message:" << body;
    
    messageContent.close();
    sendToApache();
}

void Message::formFetchFile()
{
    messageContent.open(textFileName,ios::out);
    
    messageContent << "f" << '\n';
    messageContent << "Receiver:" << receiver;
    
    messageContent.close();
    sendToApache();
}

int main()
{
    cout << "Hello, and Welcome To MQS." << endl;
    
    Message m;
    int inputMode;
    
    cout << "Before we start, please enter your IP address:" << endl;
    cin >> m.ipAddress;
    
    cout << "Do you want to send or fetch a message? If send, enter 1; if fetch, enter 2:" << endl;
    cin >> inputMode;
    while((inputMode != 1)&&(inputMode != 2))
    {
        cout << "Invalid input. Please re-enter either 1 for send or 2 for fetch:" << endl;
        cin >> inputMode;
    }
}
   // m.requestType(inputMode);
    
                        // cout << sendToApache("192.168.56.101","C:Users\eyads\source\repos\Multithreading messaging queue\Multithreading messaging queue\text.txt") << endl;
                         //C:\Users\eyads\Desktop\curl
                         //return 0;
                         //}

