#include <iostream>
#include <string>
#include <stdio.h>
#include <fstream>

using namespace std;

class Message
{
private:
    string sender;
    string receiver;
    string subject;
    string body;
    char textFileName[20] = "apacheMessage.txt";
    char finalFileName[20] = "apacheMessage.out";
    ofstream messageContent;
    void sendMessage();
    void fetchMessage();
    string sendToApache();
    void formSendFile();
    void formFetchFile();
    bool resultMessage(char** argv[]);
    bool parsing(string fileName);
    void copyToIn(string fileName);
    
public:
    string ipAddress;
    Message();
    void requestType(int inputMode);
    void requestType(string fileName);
    ~Message();
};
