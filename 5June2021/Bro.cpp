#include<iostream>
#include<map>
#include<stdio.h>
#include<unistd.h>
#include<forward_list>
#include<string.h>
#ifdef linux
#include<arpa/inet.h>
#include<sys/socket.h>
#endif
#ifdef _WIN32
#include<windows.h>
#endif
using namespace std;
//Amit - Bro [The Bro Programmer]
class Validator
{
private:
Validator(){}
public:
static bool isValidMIMEType(string &mimeType)
{
//right now, do nothing
return true;
}
static bool isValidPath(string &path)
{
//right now, do nothing
return true;
}
static bool isValidURLFormat(string &url)
{
//right now, do nothing
return true;
}
};
class Error
{
string error;
public:
Error(string error)
{
this->error=error;
}
bool hasError()
{
return this->error.length()>0;
}
string getError()
{
return this->error;
}
};
class Request
{
};
class Response
{
private:
forward_list<string> content;
forward_list<string>::iterator contentIterator;
unsigned long contentLength;
string contentType;
public:
Response()
{
this->contentIterator=this->content.before_begin();
this->contentLength=0;
}
~Response()
{
//not yet decided
}
void setContentType(string contentType)
{
//Validator class having static method isValidMIMEType(string)
if(Validator::isValidMIMEType(contentType)) 
{
this->contentType=contentType;
}
}
Response & operator<<(string content)
{
this->contentLength+=content.length();
this->contentIterator=this->content.insert_after(this->contentIterator,content);
return *this;
}
};
class Bro
{
private:
string staticResourcesFolder;
//we will check if the request arrived exist in map if it doesn't then search in static resources folder
map<string,void (*) (Request &,Response &)> urlMappings;
public:
Bro()
{
//not yet decided
}
~Bro()
{
//not yet decided
}
void setStaticResourcesFolder(string staticResourcesFolder)
{
if(Validator::isValidPath(staticResourcesFolder))
{
this->staticResourcesFolder=staticResourcesFolder;
}
else
{
//not yet decided
}
}
void get(string url,void (*callBack)(Request &,Response &))
{
if(Validator::isValidURLFormat(url))
{
if(!(urlMappings.find(url)==urlMappings.end()))
{
urlMappings.insert(pair<string,void (*)(Request &,Response &)>(url,callBack));
}
}
}
void listen(int portNumber,void (*callBack)(Error &))
{
#ifdef _WIN32
WSADATA wsaData;
WORD ver;
ver=MAKEWORD(1,1);
WSAStartup(ver,&wsaData);
#endif
int serverSocketDescriptor;
char requestBuffer[4097]; //one extra byte for \0
int requestLength;
int x;
serverSocketDescriptor=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
if(serverSocketDescriptor<0)
{
#ifdef _WIN32
WSACleanup();
#endif
Error error("Unable to create socket");
callBack(error);
return;
}
struct sockaddr_in serverSocketInformation;
serverSocketInformation.sin_family=AF_INET;
serverSocketInformation.sin_port=htons(portNumber);
serverSocketInformation.sin_addr.s_addr=htonl(INADDR_ANY); //using INADDR_ANY we are specifying that our server is open for all available ips
int successCode=bind(serverSocketDescriptor,(struct sockaddr *)&serverSocketInformation,sizeof(serverSocketInformation));
if(successCode<0)
{
close(serverSocketDescriptor);
#ifdef _WIN32
WSACleanup();
#endif
char a[101];
sprintf(a,"Unable to bind socket to port : %d",portNumber);
Error error(a);
callBack(error);
return;
}
//::listen is global listen from windows.h
//10 is queue size
successCode=::listen(serverSocketDescriptor,10);
if(successCode<0)
{
close(serverSocketDescriptor);
#ifdef _WIN32
WSACleanup();
#endif
Error error("Unable to accept client connections");
callBack(error);
return;
}
Error error("");
callBack(error);
struct sockaddr_in clientSocketInformation;
#ifdef linux
socklen_t len=sizeof(clientSocketInformation);
#endif
#ifdef _WIN32
int len=sizeof(clientSocketInformation);
#endif
int clientSocketDescriptor;
while(1)
{
clientSocketDescriptor=accept(serverSocketDescriptor,(struct sockaddr *)&clientSocketInformation,&len);
if(clientSocketDescriptor<0)
{
//not yet decided
}
//recv will give how many bytes successfully extracted
forward_list<string> requestBufferDS;
forward_list<string>::iterator requestBufferDSIterator;
requestBufferDSIterator=requestBufferDS.before_begin();
int requestBufferDSSize=0;
int requestDataCount=0;
//each time we are reading 4096 bytes max and pushing it in forward_list
while(1)
{
requestLength=recv(clientSocketDescriptor,requestBuffer,sizeof(requestBuffer)-sizeof(char),0);
if(requestLength==0) break;
requestBuffer[requestLength]='\0';
requestBufferDSIterator=requestBufferDS.insert_after(requestBufferDSIterator,string(requestBuffer));
requestBufferDSSize++;
requestDataCount+=requestLength;
}
if(requestBufferDSSize>0)
{
char *requestData=new char[requestDataCount+1];
char *p=requestData;
const char *q;
requestBufferDSIterator=requestBufferDS.begin();
//iterating on forward list
while(requestBufferDSIterator!=requestBufferDS.end())
{
q=(*requestBufferDSIterator).c_str(); //this will give base address each time (const char *)
while(*q) // q will end with \0
{
*p=*q;
p++;
q++;    
}
++requestBufferDSIterator;
}
*p='\0';
requestBufferDS.clear(); //clearing memory allocated by forwad list
printf("------- request data begin ------\n");
printf("%s\n",requestData);
printf("------- request data ends ------\n");
delete [] requestData;
//code to parse the request goes here
//lot of code will be written here
} //if ends
else
{
//something if no data was recieved
}
close(clientSocketDescriptor);
}//infinite loop ends
#ifdef _WIN32
WSACleanup();
#endif
}
};

//Bobby [The Web Application Developer - User of Bro Server]
int main()
{
Bro bro;
bro.setStaticResourcesFolder("whatever"); //bobby will decide the folder name
bro.get("/",[](Request &request,Response &response) void {
//dispatching some html
const char *html=R""""(
<!DOCTYPE HTML>
<html lang='en'>
<head>
<meta charset='utf-8'>
<title>Whatever</title>
</head>
<body>
<h1>Welcome</h1>
<h3>Some Text</h3>
<a href='getCustomers'>Customers List</a>
</body>
</html>
)"""";
response.setContentType("text/html");
response<<html; //necessary operator overloading should be done
}); //2nd param is similar like lambda expression

bro.get("/getCustomers",[](Request &request,Response &response)void {
const char *html=R""""(
<!DOCTYPE HTML>
<html lang='en'>
<head>
<meta charset='utf-8'>
<title>Whatever</title>
</head>
<body>
<h1>List of Customers</h1>
<ul>
<li>Ramesh</li>
<li>Suresh</li>
<li>Mohan</li>
</ul>
<a href='/'>Home</a>
</body>
</html>
)"""";
response.setContentType("text/html");
response<<html;
});
bro.listen(6060,[](Error &error)void {
if(error.hasError())
{
cout<<error.getError();
return;
}
cout<<"Bro Server is Ready to accept request on port 6060"<<endl;
});
return 0;
}