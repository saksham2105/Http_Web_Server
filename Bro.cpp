#include<iostream>
using namespace std;
//Amit - Bro [The Bro Programmer]
class Error
{
public:
bool hasError()
{
return false;
}
string getError()
{
//do nothing right now
return "";
}
};
class Request
{
};
class Response
{
public:
void setContentType(string contentType)
{
//do nothing
}
Response & operator<<(string content)
{
//do nothing
return *this;
}
};
class Bro
{
public:
void setStaticResourcesFolder(string staticResourcesFolder)
{
//do nothing 
}
void get(string urlPattern,void (*callBack)(Request &,Response &))
{
//do nothing right now
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