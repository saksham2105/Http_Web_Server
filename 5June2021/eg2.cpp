#include<iostream>
using namespace std;
int main()
{
char *requestData=new char[11];
printf("%s\n",requestData);    
char *p=requestData;
int i=0;
char q[10]="SakshamSo";
while(i<10)
{
*p=q[i];
p++;    
i++;    
}
*p='\0';
printf("%s\n",requestData);    
delete [] requestData;
return 0;
}