#include<iostream>
#include<map>
using namespace std;
int main()
{
map<string,int> m;
pair<string,int> p1("good",10);
m.insert(p1);
m.insert(pair<string,int>("Cool",20));
return 0;
}