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
class StringUtility
{
private:
StringUtility(){}
public:
static void toLowerCase(char *str)
{
if(str==NULL) return;
for(;*str;*str++) if(*str>=65 && *str<=90) *str+=32;
}
};
class HttpErrorStatusUtility
{
private:
HttpErrorStatusUtility(){}
public:
static void sendBadRequestError(int clientSocketDescriptor)
{
//right now, do nothing
}
static void sendHttpVersionNotSupportedError(int clientSocketDescriptor,char *version)
{
//right now, do nothing
}
static void sendNotFoundError(int clientSocketDescriptor,char *requestURI)
{
//right now, do nothing
string errorString="<!doctype html><html lang='en'><head><meta charset='utf-8'><title>400 Error</title></head><body><h1>Request Resource "+(string)requestURI+" Not Found</h1></body></html>";
char *content=new char[errorString.length()];
char header[200];
char *response=new char[errorString.length()+200];
sprintf(content,"<!doctype html><html lang='en'><head><meta charset='utf-8'><title>400 Error</title></head><body><h1>Request Resource [%s] Not Found</h1></body></html>",requestURI);
int contentLength=strlen(content);
sprintf(header,"HTTP/1.1 404 Not found\r\n Content-Type:text/html\r\nContent-Length:%d\nConnection:close\r\n\r\n",contentLength);
strcpy(response,header);
strcat(response,content);
send(clientSocketDescriptor,response,strlen(response),0);
}
static void sendMethodNotAllowedError(int clientSocketDescriptor,char *method,char *requestURI)
{
//right now, do nothing
}
};
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
private:
char *method;
char *requestURI;
char *httpVersion;
Request(char *method,char *requestURI,char *httpVersion)
{
this->method=method;
this->requestURI=requestURI;
this->httpVersion=httpVersion;
}
friend class Bro;
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
friend class HttpResponseUtility;
};
class HttpResponseUtility
{
private:
HttpResponseUtility(){}
public:
static void sendResponse(int clientSocketDescriptor,Response &response)
{
char header[200];
int contentLength=response.contentLength;
sprintf(header,"HTTP/1.1 200 OK\r\n Content-Type:text/html\r\nContent-Length:%d\nConnection:close\r\n\r\n",contentLength);
send(clientSocketDescriptor,header,strlen(header),0);
auto contentIterator=response.content.begin();
while(contentIterator!=response.content.end())
{
string str=*contentIterator;
send(clientSocketDescriptor,str.c_str(),str.length(),0);
++contentIterator;
}
}
};
enum __request_method__{__GET__,__POST__,__PUT__,__DELETE__,__CONNECT__,__TRACE__,__HEAD__,__OPTIONS__};
//typedef means making alias of a struct
typedef struct __url__mapping
{
__request_method__ requestMethod;
void (*mappedFunction) (Request &,Response &);
}URLMapping;
class Bro
{
private:
string staticResourcesFolder;
//we will check if the request arrived exist in map if it doesn't then search in static resources folder
map<string,URLMapping> urlMappings;
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
urlMappings.insert(pair<string,URLMapping>(url,{__GET__,callBack}));
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
requestLength=recv(clientSocketDescriptor,requestBuffer,sizeof(requestBuffer)-sizeof(char),0);
if(requestLength==0 || requestLength==-1)
{
close(clientSocketDescriptor);
continue;
}
char *method,*requestURI,*httpVersion;
requestBuffer[requestLength]='\0';
int i;
method=requestBuffer;
i=0;
while(requestBuffer[i] && requestBuffer[i]!=' ') i++;
if(requestBuffer[i]=='\0')
{
//request is not well formed not according to http protocol
HttpErrorStatusUtility::sendBadRequestError(clientSocketDescriptor);
close(clientSocketDescriptor);
continue;
}
requestBuffer[i]='\0';
i++;
if(requestBuffer[i]==' ' || requestBuffer[i]=='\0')
{
//again bad request
HttpErrorStatusUtility::sendBadRequestError(clientSocketDescriptor);
close(clientSocketDescriptor);
continue;
}
StringUtility::toLowerCase(method);
if(!(strcmp(method,"get")==0 || 
strcmp(method,"post")==0 || 
strcmp(method,"put")==0 || 
strcmp(method,"delete")==0 || 
strcmp(method,"head")==0 || 
strcmp(method,"options")==0 || 
strcmp(method,"trace")==0 || 
strcmp(method,"connect")==0))
{
HttpErrorStatusUtility::sendBadRequestError(clientSocketDescriptor);
close(clientSocketDescriptor);
continue;
}
requestURI=requestBuffer+i;
while(requestBuffer[i] && requestBuffer[i]!=' ') i++;
if(requestBuffer[i]=='\0')
{
HttpErrorStatusUtility::sendBadRequestError(clientSocketDescriptor);
close(clientSocketDescriptor);
continue;
}
requestBuffer[i]='\0';
i++;
if(requestBuffer[i]==' ' || requestBuffer[i]=='\0')
{
HttpErrorStatusUtility::sendBadRequestError(clientSocketDescriptor);
close(clientSocketDescriptor);
continue;
}
httpVersion=requestBuffer+i;
while(requestBuffer[i] && requestBuffer[i]!='\r' && requestBuffer[i]!='\n') i++;
if(requestBuffer[i]=='\0')
{
HttpErrorStatusUtility::sendBadRequestError(clientSocketDescriptor);
close(clientSocketDescriptor);
continue;
}
if(requestBuffer[i]=='\r' && requestBuffer[i+1]!='\n')
{
HttpErrorStatusUtility::sendBadRequestError(clientSocketDescriptor);
close(clientSocketDescriptor);
continue;
}
if(requestBuffer[i]=='\r')
{
requestBuffer[i]='\0';
i=i+2;
}
else
{
requestBuffer[i]='\0';
i=i+1;
}
StringUtility::toLowerCase(httpVersion);
if(strcmp(httpVersion,"http/1.1")!=0)
{
HttpErrorStatusUtility::sendHttpVersionNotSupportedError(clientSocketDescriptor,httpVersion);
close(clientSocketDescriptor);
continue;
}
auto urlMappingsIterator=urlMappings.find(requestURI);
if(urlMappingsIterator==urlMappings.end())
{
HttpErrorStatusUtility::sendNotFoundError(clientSocketDescriptor,requestURI);
close(clientSocketDescriptor);
continue;
}
URLMapping urlMapping=urlMappingsIterator->second;
if(urlMapping.requestMethod==__GET__ && strcmp(method,"get")!=0)
{
HttpErrorStatusUtility::sendMethodNotAllowedError(clientSocketDescriptor,method,requestURI);
close(clientSocketDescriptor);
continue;
}
//code to parse the header and the payload if exist starts here
//code to parse the header and the payload if exist ends here
Request request(method,requestURI,httpVersion);
Response response;
urlMapping.mappedFunction(request,response);
HttpResponseUtility::sendResponse(clientSocketDescriptor,response);
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