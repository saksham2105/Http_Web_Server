#include<iostream>
using namespace std;
int main()
{
//printf(_WIN32); Predefined for windows
#ifdef linux
printf("Linux");
#endif
#ifdef _WIN32
printf("Windows");
#endif
return 0;
}