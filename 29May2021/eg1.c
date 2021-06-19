#include<stdio.h>
#include<windows.h>
int main()
{
int x;
int y;
char *p;
x=2147483647;
p=(char *)&x;
printf("Address %u\n",&x);
printf("Address %u\n",p);
printf("%d\n",*p);
p++;
printf("Address %u\n",p);
printf("%d\n",*p);
p++;
printf("Address %u\n",p);
printf("%d\n",*p);
p++;
printf("Address %u\n",p);
printf("%d\n",*p);
y=htonl(x);
p=(char *)&y;
printf("Address %u\n",&y);
printf("Address %u\n",p);
printf("%d\n",*p);
p++;
printf("Address %u\n",p);
printf("%d\n",*p);
p++;
printf("Address %u\n",p);
printf("%d\n",*p);
p++;
printf("Address %u\n",p);
printf("%d\n",*p);
return 0;
}