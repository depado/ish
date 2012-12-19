#include <stdio.h>

int f(int a, int b)
{
   return (a+b);
}

int g(int a, int b, int *diff)
{
   *diff= a-b;
   return (a+b);
}

int main()
{
int x,y, z, d;
    x=12;
    y=7;
    z= f(x,y);
    printf("x=%d y=%d f(x,y)=%d\n",x,y,z);
    z= g(x,y, &d);
    printf("x=%d y=%d g(x,y,d)=%d et d=%d\n",x,y,z,d);
    return 0;

}

