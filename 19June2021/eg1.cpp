#include<stdio.h>
#include<sys/stat.h>
int main()
{
struct stat s;
int x;
x=stat("/Bro",&s); // x is zero if path exists else -1
printf("%d\n",x);
printf("%d\n",s.st_mode & S_IFDIR); // if this returns >0 that means it is folder else file
//code to determine file size
FILE *f;
f=fopen("/Bro/19June2021/Bro.cpp","rb");
fseek(f,0,SEEK_END);
long fileSize;
fileSize=ftell(f);
printf("%ld",fileSize);
return 0;
}