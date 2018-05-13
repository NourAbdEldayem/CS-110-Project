#include <iostream>
#include <string>
#include <stdio.h>
#include <fstream>
#include <functional>
#include <thread>
#include <sstream>
#include <dirent.h>

using namespace std;

class Message
{
//private:
public:
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
    bool resultMessage(string fileName, Message* message);
    void parsing(string fileName, Message* message);
    void copyToIn(string fileName);
    void errorMessage();
    void searchAndReturn(string fileName, Message* message);
    
//public:
    string ipAddress;
    Message();
    void requestType(int inputMode);
    ~Message();
};

class Node
{
//private:
public:
    Message* message;
    
    
public:
    Node* next;
    
    Node();
    void appendNode(Node* newNode);
    Message getMessage();
    ~Node();
    
};

class Queue
{
private:
    Node* head;
    Node* tail;
    mutex shared_mtx;
    
public:
    Queue();
    void copyToIn(string fileName);
    Node* appendQueue(Node* node);
    void searchAndReturn(string fileName, Message* message);
    ~Queue();
};

static void requestType(string fileName, Message* message);
