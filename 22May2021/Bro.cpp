#include<iostream>
#include<map>
#include<forward_list>
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
Error(string error)
{
this->error=error;
}
public:
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
//do nothing
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