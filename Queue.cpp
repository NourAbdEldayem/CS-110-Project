#include "queue.h"

using namespace std;

class node2
{
	private:
    	node2 * n;
    	string ip;
    	node2 * p;
    	mutex shared_mtx;
    	node2 * cur;
    	node2 *  head;
    	thread th;
	string directoryName;

	public:
    	node2()
    	{
        	head = NULL;
        	cur = NULL;
    	}
    
	void newNode(string id)
	{
		if (head == NULL)
        	{
        	    node2* n = new node2();
        	    head = n;
        	    
        	    n->ip = id;
        	    n->p = NULL;
        	}
        	else
        	{
        	    
        	    while (cur->p != NULL)
        	    {
        	        cur = cur->p;
        	    }
            
        	    n = new node2;
        	    
        	    n->ip = id;
        	    n->p = NULL;
        	}
	}
    
/*	void transverse(string id)
	{
		while(true)
	        {
			bool flag=false;
	        	node2 * temp = head;
	        
	        	while (temp != NULL)
	        	{
	           	 if (temp->ip == id)
	           	 {
	           	     flag = true;
	          	 }
	       	   	  temp = temp->p;
	       		 }
	        	if (!flag)
	        	{
	         	   newNode(id);
	        	}
			sleep(10);
		}
    	}
*/
	~node2()
	{
	        node2* temp = head->p;
	        while (temp != NULL) // as I am considering tail->next = NULL
	        {
        	    head->p = temp->p;
        	    temp->p = NULL;
        	    delete(temp);
        	    temp = head->p;
        	}
        	delete head;
	}
};

Node::Node()
{
    next = NULL;
    message = NULL;
}


void Node::appendNode(Node* newNode)
{
    newNode->next = next;
    next = newNode;
}

Node::~Node()
{
  
}

Queue::Queue()
{
    head = NULL;
    tail = NULL;
}

Node* Queue::appendQueue(Node* node)
{
    shared_mtx.lock();
    Node* ptr = NULL;
    if((head == NULL)&(tail == NULL))
    {
        head = node;
        tail = node;
        ptr = tail;
    }
    else
    {
        ptr->next = appendQueue(node);
        tail = node;
    }
    shared_mtx.unlock();
    return node;
}

Queue::~Queue()
{
    delete(head);
}

void Message::parsing(string fileName, Message *message)
{
    fstream fileM("fileName");
    string line = "";
    string field = "";
    
    for(int i=0; i<3; i++)
    {
        getline(fileM,line);
        stringstream iss;
        getline(iss, field, ':');
        if((field != "To")||(field != "From")||(field != "Subject"))
        {
            errorMessage();
        }
        else
        {
            switch(i)
            {
                case 0:
                    getline(iss,receiver,'\n');
                    break;
                case 1:
                    getline(iss,sender,'\n');
                    break;
                default:
                    getline(iss,subject,'\n');
            }
        }
        getline(fileM, line);
        getline(iss,field,':');
        if(field != "Message")
            errorMessage();
        else
            getline(iss,body,'\n');
        do
        {
            getline(fileM,field);
            body += field;
        } while(field.empty());
    }
}


void Queue::searchAndReturn(string fileName, Message* message)
{
    Node* ptr = head;
    Node* temp;
    bool flag = true;
    fstream fetchedMessage;
    fstream f("fileName");
    fetchedMessage.open("fetchedMessage.txt");
    getline(f,message->receiver);
    
    if (head == NULL)
    {
        fetchedMessage << "There are no messages to retrieve.";
        fetchedMessage.close();
        copyToIn(fileName);
        return;
    }
    while (flag)
    {
        if (ptr->next == NULL)
        {
            fetchedMessage << "There are no messages for " << message->receiver;
            fetchedMessage.close();
            copyToIn(fileName);
            flag = false;
            return;
        }
        if (ptr->next->message->receiver == message->receiver)
        {
            fetchedMessage << "To:" << ptr->message->receiver << '\n';
            fetchedMessage << "From:" << ptr->message->sender << '\n';
            fetchedMessage << "Subject:" << ptr->message->subject << '\n';
            fetchedMessage << "Message:" << ptr->message->body << '\n';
            
            temp = ptr->next;
            ptr->next = ptr->next->next;
            free(temp);
            flag = false;
        }
        else
        {
            ptr = ptr->next;
        }
    }
    f.close();
    copyToIn("fetchedMessage.txt");
}

static void resultMessage(string fileName,Message* message)
{
    fstream infile;
    infile.open("fileName");
    ofstream outfile("output.in");
    ofstream content("message.txt");
    
    string str = "";
    
    
    if(!infile)
    {
        outfile << "Failure" << '\n';
    
      outfile.close();
      return;
    
    }
    
    while (!infile.eof())
    {
        str += infile.get();
    }
    
    infile.close();
    remove("input.out");
    content << str;
    content.close();
    outfile << "Success" << endl;
    outfile.close();
    requestType(fileName,message);
}

void requestType(string fileName, Message* message)
{
    fstream check;
    string mode;
    
    check.open("fileName",ios::in);
    getline(check, mode);
    
    if(mode == "s")
        message->parsing(fileName, message);
    else if(mode == "f")
        message->searchAndReturn(fileName, message);
    else
        message->errorMessage();
}

void Message::errorMessage()
{
    ofstream error;
    char newName[] = "errorMessage.in";
    char oldName[] = "errorMessage.txt";
    
    error.open(oldName,ios::out);
    error << "Error. This message is invalid.";
    error.close();
    rename(oldName,newName);
}

void Message::copyToIn(string fileName)
{
    char newName[] = "inMessage.in";
    char oldName[20];
    strncpy(oldName,fileName.c_str(),20);
    if(rename(oldName,newName) == 0)
        errorMessage();
    else
        return;
}

void listDirectory(string &directoryName,Message* message)
{
	while(true)
	{
		DIR *dire; 
		struct dirent *ep; 
		dire = opendir(&directoryName); 
		int count = 0;

		if (dire != NULL)  
			ep = readdir(dire); 
		while (ep != NULL)
		{ 
			if(strcmp (ep->d_name,".") != 0) && (strcmp (ep->d_name,"..") != 0)) 
			startThread(ep->d_name);
			std::thread ep->d_name(resultMessage,ep->d_name,message); 
			ep->d_name.join();
			ep = readdir(dire); 
			count++; 
		}
		closedir (dire); 
	}
}


int main()
{
	Queue* queue = new Queue();
	Message* message = new Message();
	string directoryName = "../tmp/spool";

	listDirectory(&directoryName,message);
	return 0;
}


