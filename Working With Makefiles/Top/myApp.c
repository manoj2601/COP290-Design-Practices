#include <stdio.h>
extern mySquare();
extern addition();
extern sumseries();
extern modulo10();
int main(int argc, char *argv[]){
int a=3,b;
b= mySquare(a);
printf("Value of a is : %d\n",a);
printf("Manoj Kumar\n");
int c = addition(a, b);
int d = a+b+c;
int e = modulo10(d);
int count = sumseries(a, b, c, e);
printf("value of a : %d\n value of b : %d\n value of c : %d\n vlaue of d :  %d\n sum of a,b,c and e : %d\n", a, b, c, d, count);
}
